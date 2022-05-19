#ifndef __PHYSIQUE_H__
#define __PHYSIQUE_H__

// GRANDEURS PHYSIQUES
#define GRAVITE 0.005
#define KCORPS 0.001
#define LIMVITESSE 3

void NouveauCorps(Corps *c);
void LibererCorps(Corps *c);

//------------Collisions------------------------------------------------
int TestCollisionPiques(Corps *corps);
int TestCollisionPlateau(Corps *corps);
int TestCollisionCorps(Corps *corps1, Corps *corps2);

//------------Forces----------------------------------------------------
void RebondPlateau(Corps *corps);
void RepulsionCorps(Corps *corps1, Corps *corps2);

//------------Bilan des forces -> acc -> vit -> position----------------
void AppliquerForces(Corps *corps);

#endif
