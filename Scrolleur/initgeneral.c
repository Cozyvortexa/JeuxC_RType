#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <time.h>
#include <SDL_mixer.h>

#include "initgeneral.h"


void initSDL() {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to load SDL\n");
        exit(1);
    }
}

SDL_Window* initWindow() {
    // Création de la fenêtre SDL
    SDL_Window* window = SDL_CreateWindow("PING",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        1920, 1080, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Failed to create window\n");
        SDL_Quit();
        exit(1);
    }
    return window;
}

SDL_Renderer* initRenderer(SDL_Window* window) {
    // Création du rendu SDL
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("Failed to create renderer\n");
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
    return renderer;
}

void clearRenderer(SDL_Renderer* renderer) {
    // Effacement du rendu + couleur de fond blanche
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}

void freeAll(SDL_Renderer* renderer, SDL_Window* window, Mix_Music* musique) {
    // Libération des ressources SDL
    Mix_FreeMusic(musique);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}