#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <time.h>
#include <SDL_mixer.h>

#include "initgeneral.h"
#include "menu.h"
#include "background.h"
#include "text.h"
#include "menu2.h"
#include "son.h"

#define LONGUEURPLAYER 100
#define LARGEURPLAYER 100


void parametre() {
    int boucleParametre = 1;

    while (boucleParametre) {

    }
}

void traitementChoix(int choix, int* continuer, int* continuerMenu, SDL_Renderer* renderer, SDL_Window* window, Mix_Music* musiqueMenu) {
    switch (choix) {
    case 1: //lancer le jeu
        printf("choix 1");
        *continuer = 1;
        *continuerMenu = 0;                
        break;
    case 2: //paramètre
        printf("choix 2");
        // Appeler la fonction parametre() ici si nécessaire
        break;
    case 3: //quitter
        printf("choix 3");
        freeAll(renderer,window,musiqueMenu);
        break;
    }
}

void menu(SDL_Renderer* renderer, SDL_Texture* textureplayer, int* x, int* y,
    int xbackground, int ybackground, int xbackgroundplanet,
    int xbackgroundetoile, int* continuer, SDL_Window* window) {
    Mix_Music* musiqueMenu = loadMusic("../Son/StageSelection.wav");
    Mix_PlayMusic(musiqueMenu, -1);
    int continuerMenu = 1; // Supprimer la variable locale continuerMenu
    int menu_up = 0;
    int menu_down = 0;
    int menu_select = 0;
    int choix = 1;
        while (continuerMenu) {

            tapisBackground(renderer, &xbackground, &ybackground,
               &xbackgroundplanet, &xbackgroundetoile);
            renderTexture(textureplayer, renderer, *x, *y,
                LONGUEURPLAYER, LARGEURPLAYER);
            afficherText("Starforce  Strike", renderer, 100, 1000, 200);
            afficherText("JOUER", renderer, 300, 300, 300);
            afficherText("PARAMETRE", renderer, 500, 500, 300);
            afficherText("QUITTER", renderer, 700, 400, 300);
            controlleurMenu(renderer, &menu_up, &menu_down, &menu_select,
                &choix, continuer, continuer, &continuerMenu, x, y); // Utilisation du pointeur continuer
            SDL_RenderPresent(renderer);
            SDL_Delay(10);
        }
        if (*continuer == 1) { // Utiliser le pointeur continuer ici
            traitementChoix(choix, &continuer, &continuerMenu, renderer, window, musiqueMenu); // Utiliser le pointeur continuer
        } 

}
