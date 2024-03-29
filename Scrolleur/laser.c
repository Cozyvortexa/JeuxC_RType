#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <time.h>
#include <SDL_mixer.h>

#include "laser.h"

#define LONGUEUR_LASER 40
#define LARGEUR_LASER 10
#define DX_LASER 20
#define MAX_LASER 10
#define LONGUEURPLAYER 100
#define LARGEURPLAYER 100

SDL_Rect draw(SDL_Renderer* renderer, int x, int y, int longueurplayer, int largeurplayer, int R, int G, int B) {
    SDL_Rect gameobj = { x, y, longueurplayer, largeurplayer };
    SDL_SetRenderDrawColor(renderer, R, G, B, 255);
    SDL_RenderFillRect(renderer, &gameobj);
    return gameobj;
}

int creerLaser(int* x, int* y, int laserCount, Uint32* lastShootTime,
    int  player1_shoot, int shootDelay, Uint32 elapsedTime, SDL_Rect* lasers,
    Mix_Chunk* laserbeam) {

    if (player1_shoot) {
        SDL_Rect laser = { *x + LONGUEURPLAYER, *y + (LARGEURPLAYER / 2) -
            (LARGEUR_LASER / 2), LONGUEUR_LASER, LARGEUR_LASER };
        Uint32 currentTime = SDL_GetTicks();
        if (player1_shoot && laserCount < MAX_LASER && elapsedTime >= shootDelay) {
            lasers[laserCount] = laser;
            (laserCount) += 1;
            *lastShootTime = currentTime; // Mettre à jour le temps du dernier tir
            Mix_PlayChannel(-1, laserbeam, 0);
        }
    }// creerLaser prend pas directement l'emplacement memoire de laserCount il prend sa valeur qu'il retourne apres avoir fais son travail
    return laserCount;
}

void drawLaser(SDL_Renderer* renderer, int* laserCount, SDL_Rect* lasers) {
    for (int i = 0; i < *laserCount; ++i) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &lasers[i]);
        lasers[i].x += DX_LASER;
        if (lasers[i].x > 1920) {
            // Supprimer le laser qui a quitté l'écran en le remplaçant par le dernier laser du tableau
            lasers[i] = lasers[*laserCount - 1];
            (*laserCount)--;
        }
    }
}