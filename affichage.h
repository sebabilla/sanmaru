#ifndef __AFFICHAGE_H__
#define __AFFICHAGE_H__

#define ROUGE 0xFF0000FF
#define VERT 0xFF00FF00
#define BLEU 0xFFFF901E
#define JAUNE 0xFF00FFFF
#define ROSE 0xFFC693FE
#define MARRON 0xFF024EAE
#define TURQUOISE 0xFFC3E617
#define BLANC 0xFFEDEDED

typedef struct Textes
{
		char bravo[10];
		char incorrect[15];
		char pause[10];
		char fleches[60];
		char entree[60];
		char espace[60];
		char but[10];
		char credits[10];
		char auteur[100];
		char sources[100];
		char musique[100];
		char bruitages[100];
} Textes;
	
//------------Gestion de la SDL-----------------------------------------
void SDL_ExitWithError(const char *message);
void limit_fps(void);
void InitialisationAffichage(void);
void ChargerImage(char *l, int i);
void AfficherLeRendu(void);
void DestructionAffichage(void);

//------------Gestion des objets en mouvement---------------------------
void AfficherCorps(void);
void AfficherPlateau(void);

//------------Gestion du texte------------------------------------------
SDL_Texture *TextureTexte(char *texte, int i);
void EcrireTexteProvisoire(char *texte, int X, int Y, int W, int H, int couleur);
void AfficherTemps(void);
void AfficherConsignes(void);
void AfficherScore(void);
void AfficherEtatCombo(void);
void AfficherPause(void);
void AfficherScoreFinal(void);

//------------Animations------------------------------------------------
void ResetAnimation(int i);
void ResetToutesAnimations(void);
void AjouterAnimation(int x, int y, int temps, int couleur, int nombre_ecrit);
void LireAnimation(int i);
void LireToutesAnimations(void);

#endif
