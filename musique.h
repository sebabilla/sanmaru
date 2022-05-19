#ifndef __MUSIQUE_H__
#define __MUSIQUE_H__

#define NB_BRUITAGES 6

void InitialisationSon(void);
void DestructionSon(void);

Mix_Chunk *ChargementBruitages(const char *lien);
void JouerBruitage(int b);
void JouerCollision(int b);


#endif
