#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "types.h"
#include "vecteur.h"
#include "physique.h"

void NouveauCorps(Corps *c)
{
	c->position.x = LARGEUR_FENETRE / 2;
	c->position.y = 0;
	c->masse = 1;
	c->rayon = 40;
	c->poids.x = 0;
	c->poids.y = GRAVITE;
	c->repulsion = VecteurNul();
	c->resultante = VecteurNul();
	c->acceleration = VecteurNul();
	c->vitesse.y = 0;
	c->vitesse.x = (double)rand() / RAND_MAX - 0.5;
	c->suivant = NULL;
	c->precedant = NULL;
	c->couleur = rand() % 4;
	c->contacts_sol = 0;
	
	return;	
}

void LibererCorps(Corps *c)
{
	free(c);
	c = NULL;
	
	return;
}

int TestCollisionPiques(Corps *corps)
{
	if (fabsf(corps->position.y - mobiles.plateau.y - 10) < 10)
		if (fabsf(corps->position.x - 20 - (mobiles.plateau.x + 100)) < 30)
		return 1;
	return 0;
}

int TestCollisionPlateau(Corps *corps)
{
	if (fabsf(corps->position.y - mobiles.plateau.y - 20) < 10)
		if (fabsf(corps->position.x - 20 - (mobiles.plateau.x + 100)) < 100)
		return 1;
	return 0;
}

int TestCollisionCorps(Corps *corps1, Corps *corps2)
{
	float distance = fabsf(VecteurNorme(VecteursSoustraire(corps1->position, corps2->position)));
	if (distance <= 40)
		return 1;
	return 0;
}

void RebondPlateau(Corps *corps)
{
	corps->vitesse.y = -0.7 * corps->vitesse.y;
	corps->position.y -= 2;
}

void RepulsionCorps(Corps *corps1, Corps *corps2)
{
	Vecteur difference = VecteursSoustraire(corps1->position, corps2->position);
	float distance = VecteurNorme(difference);
	float penetration = corps1->rayon + corps2->rayon - distance;
	float ressort = KCORPS * (penetration);
	difference = VecteurNormaliser(difference);
	
	if (penetration >= 0)
	{
	corps1->repulsion = VecteursAjouter(corps1->repulsion, VecteurMultiplierScalaire(difference, ressort));
	corps2->repulsion = VecteursAjouter(corps2->repulsion, VecteurMultiplierScalaire(difference, -1*ressort));
	}
	else
	{
	corps1->repulsion = VecteursAjouter(corps1->repulsion, VecteurMultiplierScalaire(difference, -1*ressort));
	corps2->repulsion = VecteursAjouter(corps2->repulsion, VecteurMultiplierScalaire(difference, ressort));
	}

}

void AppliquerForces(Corps *corps)
{
	// Resultante des forces
	corps->resultante = VecteursAjouter(corps->poids, corps->repulsion);
			
	// Acceleration et vitesse
	corps->acceleration = VecteurMultiplierScalaire(corps->resultante, 1/corps->masse);
	corps->vitesse = VecteursAjouter(corps->vitesse, corps->acceleration);
	
	// Eviter qu'un corps allant trop vite en traverse un autre
	if (VecteurNorme(corps->vitesse) > LIMVITESSE)
		corps->vitesse = VecteurAttribuerNorme(corps->vitesse, LIMVITESSE);
	corps->position = VecteursAjouter(corps->position, corps->vitesse);				
}
