#include <SDL.h>
#include <stdlib.h>
#include <SDL_mixer.h>

#include "types.h"
#include "vecteur.h"
#include "physique.h"
#include "musique.h"
#include "affichage.h"
#include "controle.h"
#include "jeu.h"

void InitialisationPartie(void)
{
	partie.programme_en_cours = 1;
	partie.pause = 0;
	partie.reset = 1;
	partie.temps_restant = 0;
	partie.temps_total = 0;
	partie.temps_total_sauvegarde = 0;
	partie.score = 0;
	partie.score_sauvegarde = 0;
	RemplirDeBlanc();
	ResetToutesAnimations();
}

void PartieReset(void)
{
	partie.pause = 0;
	partie.reset = 0;
	partie.temps_restant = 5400;
	partie.temps_total = 0;
	partie.score = 0;
	RemplirDeBlanc();
	ResetToutesAnimations();	
}

void MobilesReset(void)
{
	if (mobiles.boules != NULL)
		LibererLesBoules(mobiles.boules);
	
	Corps *nouveau = malloc(sizeof(Corps));
	if (nouveau == NULL)
		exit(1);
	NouveauCorps(nouveau);
	
	mobiles.boules = nouveau;
	
	mobiles.plateau.x = LARGEUR_FENETRE / 2 - 100;
	mobiles.plateau.y = HAUTEUR_FENETRE - 30;
	mobiles.plateau.h = 30;
	mobiles.plateau.l = 200;
}

void LibererLesBoules(Corps *c)
{
	if (c->suivant != NULL)
		LibererLesBoules(c->suivant);
	
	LibererCorps(c);
}

void AjouterBoule(Corps *c)
{
	if (c->suivant != NULL)
		AjouterBoule(c->suivant);
	
	if (c->suivant == NULL)
	{
		Corps *nouveau = malloc(sizeof(Corps));
		if (nouveau == NULL)
			exit(1);
		NouveauCorps(nouveau);
		
		c->suivant = nouveau;
	}
}

void SupprimerBoule(Corps *c)
{
	if (c->suivant == NULL)
	{
		LibererCorps(c);
		return;
	}	
	if (c->precedant == NULL)
	{
		Corps *temp = c->suivant;
		LibererCorps(c);
		mobiles.boules = temp;
		return;
	}
	c->precedant->suivant = c->suivant;
	c->suivant->precedant = c->precedant;
	LibererCorps(c);
	return;
}

void RemplirCouleur(int couleur, int x, int y)
{
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 3; j++)
		{
			if (partie.combo[j][i] == 5)
			{
				if (j == 0)
				{
					partie.combo[j][i] = couleur;
					return;
				}
				else if (couleur == partie.combo[j - 1][i])
				{
					partie.combo[j][i] = couleur;
					int score = j * (i + 1) * (i + 1) * (i + 1);					
					partie.score += score; 
					AjouterAnimation(x, y - 10, 60, 0, score);
					JouerBruitage(2);
					return;
				}
				else
				{
					RemplirDeBlanc();
					return;
				}
				
			}
		}
		
	partie.score += 333;
	partie.temps_restant += 1980;
	AjouterAnimation(x, y, 60, 0, 333);
	AjouterAnimation(x, y, 60, 1, 33);
	JouerBruitage(3);
	RemplirDeBlanc();
}

void RemplirDeBlanc(void)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 5; j++)
			partie.combo[i][j] = 5;
}

void HorsJeu(void)
{
	Corps *pointeur = mobiles.boules;
	
	while (pointeur != NULL)
	{
		if (pointeur->position.x < - 20 || 
			pointeur->position.x > LARGEUR_FENETRE + 20 ||
			pointeur->position.y > HAUTEUR_FENETRE - 10)
			SupprimerBoule(pointeur);			
		else
			pointeur = pointeur->suivant;
	}
}

void ForcesEtMouvements(void)
{
	for (int cycles = 0; cycles < 5; cycles++)
	{			
		Corps *pointeur = mobiles.boules;
		
		//Collision avec piques
		while (pointeur != NULL)
		{
			if (TestCollisionPiques(pointeur) == 1)
			{
				JouerBruitage(4);
				// Copier Pointeur->Animation
				if (pointeur->couleur > 2)
				{
					partie.temps_restant += 180;
					AjouterAnimation(pointeur->position.x, pointeur->position.y - 10, 60, 1, 3);
					SupprimerBoule(pointeur);
				}
				else
				{
					RemplirCouleur(pointeur->couleur, pointeur->position.x, pointeur->position.y);
					SupprimerBoule(pointeur);
				}
			}
			pointeur = pointeur->suivant;
		}
		
		pointeur = mobiles.boules;
		
		//Collision avec plateau
		while (pointeur != NULL)
		{
			if (TestCollisionPlateau(pointeur) == 1)
			{
				RebondPlateau(pointeur);
				if (pointeur->contacts_sol == 0)
					JouerBruitage(0);
				pointeur->contacts_sol++;
			}
			else
			{
				pointeur->contacts_sol = 0;
			}
			pointeur = pointeur->suivant;
		}
								
		// Collision entre corps	
			// init
		pointeur = mobiles.boules;
		while (pointeur !=NULL)
		{
			pointeur->repulsion = VecteurNul();
			pointeur = pointeur->suivant;
		}				
		
			// double boucle
		Corps *pointeur2 = NULL;
		pointeur = mobiles.boules;
		
		while (pointeur->suivant != NULL)
		{
			pointeur2 = pointeur->suivant;
			while (pointeur2 != NULL)
			{
				if (TestCollisionCorps(pointeur, pointeur2) == 1)
				{
					RepulsionCorps(pointeur, pointeur2);
					JouerCollision(1);
				}
				pointeur2 = pointeur2->suivant;
			}
			pointeur = pointeur->suivant;
		}
			
		// Forces -> acc -> vit -> position
		pointeur = mobiles.boules;
		
		while (pointeur != NULL)
		{
			AppliquerForces(pointeur);
			pointeur = pointeur->suivant;
		}
	}
}

void ActionsEnJeu(void)
{
	Direction bouton = EntreeJoueur();
	
	switch(bouton)
	{
		case FERMERFENETRE:
			partie.programme_en_cours = 0;
			break;
		case ENTREE:
			;
			break;
		case PAUSE:
			partie.pause = 1;
			break;
		case DROITE:
			if (mobiles.plateau.x < LARGEUR_FENETRE - 100)
				mobiles.plateau.x += 15;
			break;
		case GAUCHE:
			if (mobiles.plateau.x > - 100)
				mobiles.plateau.x -= 15;
			break;
		default:
			break;
	}
}

void ActionsALArret(void)
{
	Direction bouton = EntreeJoueur();
	
	switch(bouton)
	{
		case FERMERFENETRE:
			partie.pause = 0;
			partie.programme_en_cours = 0;
			break;			
		case ENTREE:
			partie.pause = 0;
			partie.reset = 1;
			break;
		case PAUSE:
			if (partie.pause == 1)
				partie.pause = 0;
			break;							
		default:
			break;
	}
}
