#pragma once
#ifndef BACKGROUND_H
#define BACKGROUND_H

void freeBackground(SDL_Texture* texturefond, SDL_Texture* texturepoudre,
    SDL_Texture* textureplanetloin, SDL_Texture* textureetoile,
    SDL_Texture* texturebigpla);

void tapisBackground(SDL_Renderer* renderer, int* xbackground, int* ybackground
    , int* xbackgroundplanet, int* xbackgroundetoile);

#endif