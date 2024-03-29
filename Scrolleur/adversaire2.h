#pragma once
#ifndef ADVERSAIRE2_H
#define ADVERSAIRE2_H

int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

SDL_Texture* drawEntite(SDL_Renderer* renderer, const char* lien);

void mouvementEnnemi(struct Ennemie* ennemis, int nbrEnnemie, int vitesse);

int freeEnnemi(struct Ennemi* ennemis, int nbrEnnemie);

void initVague(struct Ennemie* ennemis, SDL_Renderer* renderer, int nbrEnnemie,
	const char* sprite, int score);
#endif
