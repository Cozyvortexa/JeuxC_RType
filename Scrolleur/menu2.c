#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <time.h>
#include <SDL_mixer.h>

#include "menu.h"
#include "menu2.h"




void mouvementMenu(SDL_Event evenement, int* menu_up, int* menu_down,
    int* menu_select, int valeur) {
    switch (evenement.key.keysym.sym) {
    case SDLK_z:
        *menu_up = valeur;
        break;
    case SDLK_s:
        *menu_down = valeur;
        break;
    case SDLK_SPACE:
        *menu_select = valeur;
        break;
    }
}

void directionUpMenu(int* choix, Uint32* lastSelectTime, Uint32 currentTime, int* x, int* y) {
    *choix -= 1;
    if (*choix <= 0) {
        *choix = 3;
    }
    *lastSelectTime = currentTime;

    switch (*choix)
    {
    case 1:
        *x = ((1920 - 300) / 2) - 150;
        *y = 300 + (150 / 2) + 25;
        break;
    case 2:
        *x = ((1920 - 400) / 2) - 200;
        *y = 500 + (150 / 2) + 25;
        break;
    case 3:
        *x = ((1920 - 500) / 2) - 100;
        *y = 700 + (150 / 2) + 25;
        break;
    }
}

void directionDownMenu(int* choix, Uint32* lastSelectTime, Uint32 currentTime, int* x, int* y) {
    *choix += 1;
    if (*choix > 3) {
        *choix = 1;
    }
    *lastSelectTime = currentTime;

    switch (*choix)
    {
    case 1:
        *x = ((1920 - 300) / 2) - 150;
        *y = 300 + (150 / 2) + 25;
        break;
    case 2:
        *x = ((1920 - 400) / 2) - 200;
        *y = 500 + (150 / 2) + 25;
        break;
    case 3:
        *x = ((1920 - 500) / 2) - 100;
        *y = 700 + (150 / 2) + 25;
        break;
    }
}

void controlleurMenu(SDL_Renderer* renderer, int* menu_up, int* menu_down,
    int* menu_select, int* choix, int* continuer, int* continuerMenu,
    int* boucle, int* x, int* y) {
    SDL_Event event;
    static int selectDelay = 100;
    static Uint32 lastSelectTime = 0;

    while (SDL_PollEvent(&event) != 0) {
        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsedTime = currentTime - lastSelectTime;
        if (event.type == SDL_QUIT) {
            *boucle = 0;
            *continuer = 0;
            *continuerMenu = 0;
        }
        else if (event.type == SDL_KEYDOWN) {
            mouvementMenu(event, menu_up, menu_down, menu_select, 1);
        }
        else if (event.type == SDL_KEYUP) {
            mouvementMenu(event, menu_up, menu_down, menu_select, 0);
        }
        if (*menu_up && elapsedTime >= selectDelay) {
            directionUpMenu(choix, &lastSelectTime, currentTime, x, y);
            printf("%d", *choix);
        }
        else if (*menu_down && elapsedTime >= selectDelay) {
            directionDownMenu(choix, &lastSelectTime, currentTime, x, y);
            printf("%d", *choix);
        }
        else if (*menu_select) {
            *boucle = 0;
        }
    }
}