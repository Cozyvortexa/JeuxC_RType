#pragma once
#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>

#ifndef ADVERSAIRE3_H
#define ADVERSAIRE3_H

void touche(struct Ennemi* ennemis, struct Joueurs joueur, Uint32 lasthitTime,
    SDL_Rect* lasers, int* x, int* y, int hitDelay, int* laserCount,
    int nombreEnnemis, int* score);

void generateurDeVague(SDL_Renderer* renderer, struct Ennemie* ennemis,
    struct Ennemie* ennemis2, int* triggerVague2, int* nombreEnnemis1,
    int* nombreEnnemis2);
#endif