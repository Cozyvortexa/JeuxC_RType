#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <time.h>
#include <SDL_mixer.h>

#include "background.h"
#include "text.h"




void freeBackground(SDL_Texture* texturefond, SDL_Texture* texturepoudre,
    SDL_Texture* textureplanetloin, SDL_Texture* textureetoile,
    SDL_Texture* textureplanetproche) {
    SDL_DestroyTexture(texturefond);
    SDL_DestroyTexture(texturepoudre);
    SDL_DestroyTexture(textureplanetloin);
    SDL_DestroyTexture(textureetoile);
    SDL_DestroyTexture(textureplanetproche);
}

void tapisBackground(SDL_Renderer* renderer, int* xbackground, int* ybackground
    , int* xbackgroundplanet, int* xbackgroundetoile) {
    int dxstar = -3;
    int dxplanetloin = -2;
    int dxetoile = -1;

    // Update background positions
    *xbackground += dxstar;
    if (*xbackground <= (-1920)) {
        *xbackground = 0;
    }

    *xbackgroundplanet += dxplanetloin;
    if (*xbackgroundplanet <= (-1920)) {
        *xbackgroundplanet = 1920;
    }

    *xbackgroundetoile += dxetoile;
    if (*xbackgroundetoile <= (-1920)) {
        *xbackgroundetoile = 1920;
    }

    // Load and render background textures
    SDL_Texture* texturefond = loadTexture("../Image/Background/parallax-space-backgound.png", renderer);
    SDL_Texture* texturepoudre = loadTexture("../Image/Background/parallax-space-stars.png", renderer);
    SDL_Texture* textureplanetloin = loadTexture("../Image/Background/parallax-space-far-planets.png", renderer);
    SDL_Texture* textureetoile = loadTexture("../Image/Background/parallax-space-ring-planet.png", renderer);
    SDL_Texture* textureplanetproche = loadTexture("../Image/Background/parallax-space-big-planet.png", renderer);

    renderTexture(texturefond, renderer, 0, 0, 1920, 1080);
    renderTexture(textureetoile, renderer, *xbackgroundetoile, *ybackground, 255, 575);
    renderTexture(textureplanetproche, renderer, *xbackgroundetoile, *ybackground + 500, 440, 435);
    renderTexture(textureplanetloin, renderer, *xbackgroundplanet, *ybackground, 1920, 1080);
    renderTexture(texturepoudre, renderer, *xbackground, *ybackground, 1920, 1080);
    renderTexture(texturepoudre, renderer, *xbackground + 1920, *ybackground, 1920, 1080);

    // Free background textures
    freeBackground(texturefond, texturepoudre, textureplanetloin, textureetoile, textureplanetproche);
}

