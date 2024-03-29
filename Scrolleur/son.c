#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <time.h>
#include <SDL_mixer.h>

#include "son.h"

int initAudio() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Erreur lors de l'initialisation de SDL : %s\n", SDL_GetError());
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 3, 2048) < 0) {
        printf("Erreur lors de l'initialisation de SDL Mixer : %s\n", Mix_GetError());
        SDL_Quit(); // Assurez-vous de quitter SDL en cas d'échec
        return 1;
    }
    return 0;
}

Mix_Music* loadMusic(const char* path) {
    Mix_Music* musique = Mix_LoadMUS(path);
    if (!musique) {
        printf("Erreur lors du chargement de la musique : %s\n", Mix_GetError());
        return NULL;
    }
    return musique;
}
//comme load music mais pour les bruitages
Mix_Chunk* loadSon(const char* path) {
    Mix_Chunk* son = Mix_LoadWAV(path);
    if (!son) {
        printf("Erreur lors du chargement du chunk : %s\n", Mix_GetError());
        return NULL;
    }
    return son;
}