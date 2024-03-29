#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <time.h>
#include <SDL_mixer.h>

#include "adversaire3.h"
#include "adversaire.h"
#include "adversaire2.h"

#define LONGUEURPLAYER 100
#define LARGEURPLAYER 100
#define LONGUEUR_LASER 40
#define LARGEUR_LASER 10

struct Ennemi {
    int idEnnemie;
    int pv;
    SDL_Rect posEtSize;
    SDL_Texture* spriteTexture;
    int enVie;
    int score;
};
struct Joueurs {
    int x;
    int y;
    int pv;
};


void touche(struct Ennemi* ennemis, struct Joueurs* joueur, Uint32 lasthitTime,
    SDL_Rect* lasers, int* x, int* y, int hitDelay, int* laserCount,
    int nombreEnnemis, int* score) {
    for (int i = 0; i < nombreEnnemis; i++) {
        for (int i2 = 0; i2 < *laserCount; i2++) {
            if (collision(ennemis[i].posEtSize.x, ennemis[i].posEtSize.y,
                ennemis[i].posEtSize.w, ennemis[i].posEtSize.h, lasers[i2].x,
                lasers[i2].y, lasers[i2].w, lasers[i2].h)) {
                lasers[i2] = lasers[*laserCount - 1];
                (*laserCount)--;
                damage(ennemis, i, 1, score);
            }
        }
        if (collision(*x, *y, LONGUEURPLAYER, LARGEUR_LASER,
            ennemis[i].posEtSize.x, ennemis[i].posEtSize.y,
            ennemis[i].posEtSize.w, ennemis[i].posEtSize.h) &&
            ennemis[i].pv > 0) {
            Uint32 currentTime = SDL_GetTicks();
            Uint32 elapsedTime = currentTime - lasthitTime;
            if (elapsedTime >= hitDelay) {
                joueur->pv -= 1;
                *x = joueur->x;
                *y = joueur->y;
                lasthitTime = currentTime;
            }
            if (joueur->pv <= 0) {
                printf("mort");
            }
        }
    }
}

void generateurDeVague(SDL_Renderer* renderer, struct Ennemie* ennemis,
    struct Ennemie* ennemis2, int* triggerVague2, int* nombreEnnemis1,
    int* nombreEnnemis2) {
    mouvementEnnemi(ennemis, *nombreEnnemis1, 2);
    if (ennemis != NULL) {
        display(ennemis, *nombreEnnemis1, renderer);
    }
    if (freeEnnemi(ennemis, *nombreEnnemis1) && *triggerVague2 == 1) {
        *triggerVague2 = 2;
        initVague(ennemis2, renderer, *nombreEnnemis2,
            "../Image/ennemideux.bmp", 500);
    }
    if (*triggerVague2 == 2) {
        mouvementEnnemi(ennemis2, *nombreEnnemis2, 4);
        display(ennemis2, *nombreEnnemis2, renderer);
    }
}



