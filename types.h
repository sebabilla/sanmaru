#ifndef __TYPES_H__
#define __TYPES_H__

#define TITRE "〇〇〇"
#define LARGEUR_FENETRE 800
#define HAUTEUR_FENETRE 1000
#define FPS 16

typedef struct Animation
{
	int x;
	int y;
	int temps;
	int couleur;
	int nombre_ecrit;
} Animation;

typedef struct Partie
{
	int programme_en_cours;
	int pause;
	int reset;
	int temps_restant;
	int temps_total;
	int temps_total_sauvegarde;
	int score;
	int score_sauvegarde;
	int combo[3][5];
	struct Animation animation[10];
} Partie;

typedef struct Vecteur
{
	double x;
	double y;
} Vecteur;

typedef struct Corps
{
	Vecteur position;
	float masse;
	float rayon;
	Vecteur poids;
	Vecteur repulsion;
	Vecteur resultante;
	Vecteur vitesse;
	Vecteur acceleration;
	struct Corps *suivant;
	struct Corps *precedant;
	int couleur;
	int contacts_sol;
} Corps;

typedef struct Plateau
{
	int x;
	int y;
	int h;
	int l;
} Plateau;

typedef struct Mobiles
{
	Corps *boules;
	Plateau plateau;
} Mobiles;


typedef enum Direction
{
	HAUT, BAS, GAUCHE, DROITE, ENTREE, PAUSE, FERMERFENETRE, SANS, PASDACTION
} Direction;

extern Partie partie;
extern Mobiles mobiles;

#endif
