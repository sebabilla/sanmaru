#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL_mixer.h>

#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
#endif

#include "types.h"
#include "vecteur.h"
#include "physique.h"
#include "jeu.h"
#include "affichage.h"
#include "musique.h"

Partie partie;
Mobiles mobiles;

void BouclePrincipale(void);

int main(int argc, char *argv[])
{
	
	srand(time(NULL));

	InitialisationAffichage();
	
	InitialisationSon();
	
	InitialisationPartie();

//------------Boucle principale adaptée à emcc ou gcc-------------------

	#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop(BouclePrincipale, -1, 1);
    #endif
    #ifndef __EMSCRIPTEN__
		while (partie.programme_en_cours)
			BouclePrincipale();
	#endif
	
//------------Sortie de la boucle, on nettoie tout----------------------
	
	DestructionSon();
	
	DestructionAffichage();
	
	LibererLesBoules(mobiles.boules);
	
	#ifdef __EMSCRIPTEN__
		emscripten_cancel_main_loop();
	#endif
	
	return EXIT_SUCCESS;
}

//------------Quand le jeu tourne---------------------------------------

void BouclePrincipale(void)
{
//------------Balles neuves---------------------------------------------

	if (partie.reset == 1)
	{
		PartieReset();			
		MobilesReset();
	}
//------------"Moteur Physique" en action-------------------------------			

	if (partie.pause == 0 && partie.temps_restant > 0)
	{
		partie.temps_restant--;
		partie.temps_total++;
		
		if (partie.temps_total % 50 == 0)
			AjouterBoule(mobiles.boules);
			
		ForcesEtMouvements();
		
		ActionsEnJeu();
		
		if (partie.temps_restant == 0)
		{
			if (partie.score > partie.score_sauvegarde)
				partie.score_sauvegarde = partie.score;
			partie.pause = 2;
		}		
	}
	
//------------Quand le moteur physique est skippé-------------									
	if (partie.pause != 0)		
		ActionsALArret();
		
//------------Toujours affiché------------------------------------------

	AfficherTemps();
	AfficherConsignes();
	AfficherScore();
	AfficherEtatCombo();
	AfficherCorps();
	AfficherPlateau();
	LireToutesAnimations();
	
	if (partie.pause != 0)
	{		
		if (partie.pause == 1)
			AfficherPause();
		if (partie.pause == 2)
			AfficherScoreFinal();
		
		ActionsALArret();
		
	}
	
	AfficherLeRendu();
}





