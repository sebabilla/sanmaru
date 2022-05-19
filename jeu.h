#ifndef __JEU_H__
#define __JEU_H__

//------------Initialisation et rechargement----------------------------
void InitialisationPartie(void);
void PartieReset(void);
void MobilesReset(void);

//------------Boules----------------------------------------------------
void LibererLesBoules(Corps *c);
void AjouterBoule(Corps *c);
void SupprimerBoule(Corps *c);

//------------Matrice des points----------------------------------------
void RemplirCouleur(int couleur, int x, int y);
void RemplirDeBlanc(void);

//------------Appelle les fonctions mathématiques et physiques----------
void HorsJeu(void);
void ForcesEtMouvements(void);

//------------Transmet les actions du joueur----------------------------
void ActionsEnJeu(void);
void ActionsALArret(void);
void ActionsFin(void);

#endif
