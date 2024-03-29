#pragma once
#ifndef LASER_H
#define LASER_H

SDL_Rect draw(SDL_Renderer* renderer, int x, int y, int longueurplayer, 
    int largeurplayer, int R, int G, int B);

int creerLaser(int* x, int* y, int laserCount, Uint32* lastShootTime,
    int  player1_shoot, int shootDelay, Uint32 elapsedTime, SDL_Rect* lasers);

void drawLaser(SDL_Renderer* renderer, int* laserCount, SDL_Rect* lasers);
#endif