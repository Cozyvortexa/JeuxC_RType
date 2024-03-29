#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <time.h>
#include <SDL_mixer.h>

#include "adversaire.h"

struct Ennemi {
    int idEnnemie;
    int pv;
    SDL_Rect posEtSize;
    SDL_Texture* spriteTexture;
    int enVie;
    int score;
};

void damage(struct Ennemi* ennemis, int idEnnemi, int degat, int* score) {
    ennemis[idEnnemi].pv -= degat;  // prend des degats
    if (ennemis[idEnnemi].pv <= 0 && ennemis[idEnnemi].idEnnemie == idEnnemi) {  // on suprimme la texture de l'ennemi mort et on passe sa valeur a null pour empécher son affichage dans display
        ennemis[idEnnemi].enVie = 0;
        ennemis[idEnnemi].posEtSize.y = 3000;
        *score += ennemis[idEnnemi].score;
    }
}


void mouvementRectiligneEnnemis(struct Ennemi* ennemis, int idEnnemie, int vitesse) {
    if (ennemis[idEnnemie].posEtSize.x > 0) {
        ennemis[idEnnemie].posEtSize.x -= vitesse;
    }
}

struct Ennemi initEnnemi(int id, int pv, int x, int y, int w, int h,
    SDL_Texture* spriteTexture, int score) { // création d'un Ennemi 
    struct Ennemi newEnnemi;
    newEnnemi.idEnnemie = id;
    newEnnemi.pv = pv;
    newEnnemi.posEtSize.x = x;
    newEnnemi.posEtSize.y = y;
    newEnnemi.posEtSize.w = w;
    newEnnemi.posEtSize.h = h;
    newEnnemi.spriteTexture = spriteTexture;
    newEnnemi.enVie = 1;
    newEnnemi.score = score;
    return newEnnemi;
}

void display(struct Ennemi* ennemis, int nombreEnnemis, SDL_Renderer* renderer) {
    for (int i = 0; i < nombreEnnemis; i++) {
        if (ennemis[i].enVie != 0) {
            SDL_Rect constructeur = { ennemis[i].posEtSize.x, ennemis[i].posEtSize.y, ennemis[i].posEtSize.w, ennemis[i].posEtSize.h };
            SDL_RenderCopy(renderer, ennemis[i].spriteTexture, NULL, &constructeur);
        }
    }
}

void addEnnemi(struct Ennemi* tableauEnnemis, int id, int pv, int x,
    int y, int w, int h, SDL_Texture* spriteTexture, int score) {
    tableauEnnemis[id] = initEnnemi(id, 3, x, y, w, h, spriteTexture, score);
}