#pragma once
#ifndef MENU_H
#define MENU_H

void parametre();

void traitementChoix(int choix, int* continuer, int* continuerMenu, SDL_Renderer* renderer, SDL_Window* window, Mix_Music* musiqueMenu);

void menu(SDL_Renderer* renderer, SDL_Texture* textureplayer, int* x, int* y,
    int xbackground, int ybackground, int xbackgroundplanet,
    int xbackgroundetoile, int* continuer, SDL_Window* window);

#endif