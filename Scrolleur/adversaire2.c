#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <time.h>
#include <SDL_mixer.h>

#include "adversaire2.h"
#include "adversaire.h"

struct Ennemi {
    int idEnnemie;
    int pv;
    SDL_Rect posEtSize;
    SDL_Texture* spriteTexture;
    int enVie;
    int score;
};

int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    int left1 = x1;
    int right1 = x1 + w1;
    int top1 = y1;
    int bottom1 = y1 + h1;

    int left2 = x2;
    int right2 = x2 + w2;
    int top2 = y2;
    int bottom2 = y2 + h2;

    // Vérifier qu'il n'y a aucune condition 
    if (bottom1 <= top2 || top1 >= bottom2 || right1 <= left2 || left1 >= right2) {
        return 0;
    }
    return 1;
}


SDL_Texture* drawEntite(SDL_Renderer* renderer, const char* lien) {  // Permet de creer une texture a partir d'un lien vers un fichier BMP
    SDL_Surface* spriteSurface = SDL_LoadBMP(lien);
    if (spriteSurface == NULL) {
        printf("Impossible de charger l'image : %s\n", SDL_GetError());
    }
    SDL_Texture* spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
    SDL_FreeSurface(spriteSurface);
    if (spriteTexture == NULL) {
        printf("Impossible de creer la texture : %s\n", SDL_GetError());
    }
    return spriteTexture;
}

void mouvementEnnemi(struct Ennemie* ennemis, int nbrEnnemie, int vitesse) {
    for (int i = 0; i < nbrEnnemie; i++) {
        mouvementRectiligneEnnemis(ennemis, i, vitesse);
    }
}

int freeEnnemi(struct Ennemi* ennemis, int nbrEnnemie) {
    int vagueMorte = 1;
    for (int i = 0; i < nbrEnnemie; i++) {
        if (ennemis[i].pv > 0) {
            vagueMorte = 0;
            break;
        }
    }
    if (vagueMorte) {
        //free(ennemis);
        return 1;
    }
    return 0;
}

void initVague(struct Ennemie* ennemis, SDL_Renderer* renderer, int nbrEnnemie, const char* sprite, int score) {
    SDL_Texture* spriteTexture = drawEntite(renderer, sprite);
    int x = 1920;
    int y = 100;

    for (int i = 0; i < nbrEnnemie; i++) {
        x = x + 100 * i;
        y = y + 100 * i;
        addEnnemi(ennemis, i, 3, x, y, 100, 100, spriteTexture, score);
    }
}