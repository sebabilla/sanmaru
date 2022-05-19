#include <SDL_mixer.h>

#include "musique.h"

int initted;
Mix_Chunk *bruitages[NB_BRUITAGES];

void InitialisationSon(void)
{
	int flags = MIX_INIT_OGG;
	initted = Mix_Init(flags);
		
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0)
	{
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		exit(EXIT_FAILURE);;
	}
		
	bruitages[0] = ChargementBruitages("res/b0.ogg");
	bruitages[1] = ChargementBruitages("res/b1.ogg");
	bruitages[2] = ChargementBruitages("res/argent1.ogg");
	bruitages[3] = ChargementBruitages("res/caisse.ogg");
	bruitages[4] = ChargementBruitages("res/bulle.ogg");
	bruitages[5] = ChargementBruitages("res/cloche.ogg");
	Mix_AllocateChannels(30);
	
}

void DestructionSon(void)
{
	for (int i = 0; i < NB_BRUITAGES; i++)
		Mix_FreeChunk(bruitages[i]);
	Mix_CloseAudio();
}

Mix_Chunk *ChargementBruitages(const char *lien)
{
	Mix_Chunk *b = Mix_LoadWAV(lien);
	if (b == NULL)
	{
		printf("Mix_LoadWAV: %s\n", Mix_GetError());
		exit(EXIT_FAILURE);
	}
	return b;
}

void JouerBruitage(int b)
{
	if(Mix_PlayChannel(-1, bruitages[b], 0) < 0 ) {
		printf("Mix_PlayChannel: %s\n",Mix_GetError()); 
	}
}
void JouerCollision(int b)
{
	if (Mix_Playing(b + 25) == 0) 
		if(Mix_PlayChannel(b + 25, bruitages[b], 0) < 0 ) {
			printf("Mix_PlayChannel: %s\n",Mix_GetError()); 
		}
}
