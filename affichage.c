#include <SDL.h>
#include <stdio.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>

#include "types.h"
#include "jeu.h"
#include "affichage.h"

SDL_Window *window;
SDL_Renderer *renderer;
int frame_limit;
TTF_Font *font;
SDL_Texture *texture_illustration[6];


void SDL_ExitWithError(const char *message)
{
	SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
	SDL_Quit();
	exit(EXIT_FAILURE);
}

void limit_fps(void)
{
	unsigned t = SDL_GetTicks();
	
	if (frame_limit < t)
	{
		frame_limit = SDL_GetTicks() + FPS;
		return;
	}
	else if (frame_limit > t + FPS)
		SDL_Delay(FPS);
	else
		SDL_Delay(frame_limit - t);
	frame_limit = SDL_GetTicks() + FPS;
}

void InitialisationAffichage(void)
{
	window = NULL;
	renderer = NULL;

	frame_limit = 0;
		
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
		SDL_ExitWithError("Initiation SDL");
		
	if (SDL_CreateWindowAndRenderer(LARGEUR_FENETRE, HAUTEUR_FENETRE, 0, &window, &renderer))
		SDL_ExitWithError("Impossible de creer la fenetre et le rendu");
	
	SDL_SetWindowTitle(window, TITRE);

	if (TTF_Init() < 0)
		SDL_ExitWithError("Initiation TTF");
		
	font = TTF_OpenFont("res/Orbitron.ttf", 100);
	if (!font)
		SDL_ExitWithError("Chargement de la police");
		
	ChargerImage("res/rouge.bmp", 0);
	ChargerImage("res/vert.bmp", 1);
	ChargerImage("res/bleu.bmp", 2);
	ChargerImage("res/boule_trois.bmp", 3);
	ChargerImage("res/socle_piques.bmp", 4);
	ChargerImage("res/blanc.bmp", 5);
		
}

void ChargerImage(char *l, int i)
{
	SDL_Surface *image = NULL;
	texture_illustration[i] = NULL;	
	if ((image = SDL_LoadBMP(l)) == NULL)
		SDL_ExitWithError("Impossible de charger l'image");
	if ((texture_illustration[i] = SDL_CreateTextureFromSurface(renderer, image)) == NULL)
		SDL_ExitWithError("Impossible de charger l'image");
	SDL_FreeSurface(image);
}

void AfficherLeRendu(void)
{
	SDL_RenderPresent(renderer);
			
	#ifndef __EMSCRIPTEN__
		limit_fps();
	#endif
	
	SDL_Rect fond_fenetre = {.x = 0, .y = 0, .w = LARGEUR_FENETRE, .h = HAUTEUR_FENETRE};
	SDL_RenderClear(renderer);					
	
	if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE) != 0)
		SDL_ExitWithError("Impossible de changer la couleur du rendu");
		
	if (SDL_RenderFillRect(renderer, &fond_fenetre) != 0)
		SDL_ExitWithError("Impossible de dessiner un rectangle");
}

void DestructionAffichage(void)
{
	for (int i = 0; i < 9; i++)
		SDL_DestroyTexture(texture_illustration[i]);
		
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void AfficherCorps(void)
{
	Corps *pointeur = mobiles.boules;
	
	SDL_Rect rect_illu;
	
	while (pointeur != NULL)
	{
		rect_illu.x = pointeur->position.x - pointeur->rayon; 
		rect_illu.y = pointeur->position.y - pointeur->rayon;
		rect_illu.w = pointeur->rayon;
		rect_illu.h = pointeur->rayon;
		if (SDL_RenderCopy(renderer, texture_illustration[pointeur->couleur], NULL, &rect_illu) != 0)
			SDL_ExitWithError("Impossible d'afficher la texture");
			
		pointeur = pointeur->suivant;
	}
	
}

void AfficherPlateau(void)
{
	SDL_Rect rect_illu = {.x = mobiles.plateau.x, .y = mobiles.plateau.y, .w = mobiles.plateau.l, .h = mobiles.plateau.h};
	if (SDL_RenderCopy(renderer, texture_illustration[4], NULL, &rect_illu) != 0)
			SDL_ExitWithError("Impossible d'afficher la texture");
}

SDL_Texture *TextureTexte(char *texte, int i)
{
	SDL_Color couleur;
	if (i == 0) // blanc
	{
		couleur.r = 250; couleur.g = 250; couleur.b = 250; couleur.a = 0;
	}
	if (i == 1) // jaune
	{
		couleur.r = 250; couleur.g = 250; couleur.b = 0; couleur.a = 0;
	}
										
	SDL_Surface *surface_message = TTF_RenderUTF8_Solid(font, texte, couleur);
	SDL_Texture *message = SDL_CreateTextureFromSurface(renderer, surface_message);
	SDL_FreeSurface(surface_message);
	
	return message;
}

void EcrireTexteProvisoire(char *texte, int X, int Y, int W, int H, int couleur)
{	
	SDL_Texture *message = TextureTexte(texte, couleur);	
	SDL_Rect MessageRect = {.x = X, .y = Y, .w = W, .h = H};
	SDL_RenderCopy(renderer, message, NULL, &MessageRect);
	SDL_DestroyTexture(message);
}

void AfficherTemps(void)
{
	char temps[20];
	sprintf(temps, "%02d:%02d", partie.temps_restant / 3600, partie.temps_restant % 3600 / 60);
	EcrireTexteProvisoire(temps, 10, 10, 75, 25, 1);
}

void AfficherConsignes(void)
{
	char texte[20];
	sprintf(texte, "PAUSE  space");
	if(stringRGBA (renderer, 10, 40, texte, 200, 200, 200, 255) < 0)
		SDL_ExitWithError("Impossible d'afficher les animations");
	sprintf(texte, "RESET  return");	
	if(stringRGBA (renderer, 10, 50, texte, 200, 200, 200, 255) < 0)
		SDL_ExitWithError("Impossible d'afficher les animations");
}

void AfficherScore(void)
{
	char score[20];
	sprintf(score, "%05d", partie.score);
	EcrireTexteProvisoire(score, LARGEUR_FENETRE - 80, 10, 70, 20, 0);
}

void AfficherEtatCombo(void)
{
	SDL_Rect rect_illu;
	
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 5; j++)
		{
			rect_illu.x = LARGEUR_FENETRE - 80 + 25 * i; 
			rect_illu.y = 32 + 27 * j;
			rect_illu.w = 25;
			rect_illu.h = 25;
			if (SDL_RenderCopy(renderer, texture_illustration[partie.combo[i][j]], NULL, &rect_illu) != 0)
				SDL_ExitWithError("Impossible d'afficher la texture");
		}
}

void AfficherPause(void)
{
	char texte[3];
	sprintf(texte, "II");
	EcrireTexteProvisoire(texte, LARGEUR_FENETRE / 2 - 25, HAUTEUR_FENETRE / 2 - 100, 50, 200, 1);
}

void AfficherScoreFinal(void)
{
	char texte1[20];
	char texte2[20];
	char texte3[20];	
	sprintf(texte1, "GAME OVER");
	sprintf(texte2, "BEST %04d", partie.score_sauvegarde);
	sprintf(texte3, "now %04d", partie.score);
	EcrireTexteProvisoire(texte1, LARGEUR_FENETRE / 2 - 100, HAUTEUR_FENETRE / 2 - 100, 200, 50, 1);
	EcrireTexteProvisoire(texte2, LARGEUR_FENETRE / 2 - 50, HAUTEUR_FENETRE / 2, 100, 30, 0);
	EcrireTexteProvisoire(texte3, LARGEUR_FENETRE / 2 - 40, HAUTEUR_FENETRE / 2 + 35, 80, 30, 0);
}

void ResetAnimation(int i)
{
	partie.animation[i].x = 0;
	partie.animation[i].y = 0;
	partie.animation[i].temps = 0;
	partie.animation[i].couleur = 0;
	partie.animation[i].nombre_ecrit = 0;
}

void ResetToutesAnimations(void)
{
	for (int i = 0; i < 10; i++)
		ResetAnimation(i);
}

void AjouterAnimation(int x, int y, int temps, int couleur, int nombre_ecrit)
{
	for (int i = 0; i < 10; i++)
		if (partie.animation[i].temps <= 0)
		{
			partie.animation[i].x = x;
			partie.animation[i].y = y;
			partie.animation[i].temps = temps;
			partie.animation[i].couleur = couleur;
			partie.animation[i].nombre_ecrit = nombre_ecrit;
			return;
		}
}

void LireAnimation(int i)
{
	Uint8 bleu = 255;
	if (partie.animation[i].couleur == 1)
	{
		bleu = 0;
		partie.animation[i].x--;
	}
	else
		partie.animation[i].x++;
	
	partie.animation[i].y--;
	partie.animation[i].temps--;
	char texte[5];
	sprintf(texte, "%d", partie.animation[i].nombre_ecrit);
	
	if(stringRGBA (renderer, partie.animation[i].x, partie.animation[i].y--, texte, 255, 255, bleu, 255) < 0)
		SDL_ExitWithError("Impossible d'afficher les animations");
}

void LireToutesAnimations(void)
{
	for (int i = 0; i < 10; i++)
		if (partie.animation[i].temps > 0)
			LireAnimation(i);
}





