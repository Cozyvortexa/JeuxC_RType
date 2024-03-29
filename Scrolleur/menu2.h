#pragma once
#ifndef MENU2_H
#define MENU2_H

void mouvementMenu(SDL_Event evenement, int* menu_up, int* menu_down,
    int* menu_select, int valeur);

void directionUpMenu(int* choix, Uint32* lastSelectTime, Uint32 currentTime, int* x, int* y);

void directionDownMenu(int* choix, Uint32* lastSelectTime, Uint32 currentTime, int* x, int* y);

void controlleurMenu(SDL_Renderer* renderer, int* menu_up, int* menu_down,
    int* menu_select, int* choix, int* continuer, int* continuerMenu,
    int* boucle, int* x, int* y);


#endif