#pragma once
#ifndef PLAYER_H
#define PLAYER_H

void action(SDL_Event evenement, int* player1_up, int* player1_down,
    int* player1_right, int* player1_left, int* player1_shoot, int valeur);

void controller(int* continuer, int* player1_up, int* player1_down,
    int* player1_left, int* player1_right, int* player1_shoot);

int deplacementDiagonale(int player1_left, int player1_right, int x);

void deplacement(int* y, int* x, int* laserCount, int player1_shoot,
    int player1_up
    , int player1_left, int player1_down, int player1_right, SDL_Rect* lasers,
    Mix_Chunk* laserbeam);

struct Joueurs initJoueur(int x, int y);


#endif