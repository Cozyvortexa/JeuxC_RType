#pragma once
#ifndef ADVERSAIRE_H
#define ADVERSAIRE_H

void damage(struct Ennemi* ennemis, int idEnnemi, int degat, int* score);

void mouvementRectiligneEnnemis(struct Ennemi* ennemis, int idEnnemie, int vitesse);

struct Ennemi initEnnemi(int id, int pv, int x, int y, int w, int h,
    SDL_Texture* spriteTexture, int score);

void display(struct Ennemi* ennemis, int nombreEnnemis, SDL_Renderer* renderer);

void addEnnemi(struct Ennemi* tableauEnnemis, int id, int pv, int x,
    int y, int w, int h, SDL_Texture* spriteTexture, int score);

#endif