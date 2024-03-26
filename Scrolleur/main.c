#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

void initSDL() {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to load SDL\n");
        exit(1);
    }
}

SDL_Window* initWindow() {
    // Création de la fenêtre SDL
    SDL_Window* window = SDL_CreateWindow("PING",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Failed to create window\n");
        SDL_Quit();
        exit(1);
    }
    return window;
}

SDL_Renderer* initRenderer(SDL_Window* window) {
    // Création du rendu SDL
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("Failed to create renderer\n");
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
    return renderer;
}

int initTTF() {
    // Initialisation de SDL_ttf
    if (TTF_Init() == -1) {
        printf("Impossible d'initialiser SDL_ttf : %s\n", TTF_GetError());
        SDL_Quit();
        exit(1);
    }
    return 0;
}

void freeAll(SDL_Renderer* renderer, SDL_Window* window) {
    // Libération des ressources SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void clearRenderer(SDL_Renderer* renderer) {
    // Effacement du rendu + couleur de fond blanche
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}

void mouvement(SDL_Event evenement, int* player1_up, int* player1_down, int* player1_right, int* player1_left, int valeur) {
    switch (evenement.key.keysym.sym) {
    case SDLK_z:
        *player1_up = valeur;
        break;
    case SDLK_s:
        *player1_down = valeur;
        break;
    case SDLK_d:
        *player1_right = valeur;
        break;
    case SDLK_q:
        *player1_left = valeur;
        break;
    //case SDLK_UP:
    //    *player1_up = valeur;
    //    break;
    //case SDLK_DOWN:
    //    *player1_down = valeur;
    //    break;
    //case SDLK_RIGHT:
    //    *player1_right = valeur;
    //    break;
    //case SDLK_LEFT:
    //    *player1_left = valeur;
    //    break;
    }
}

void controller(int* continuer, int* player1_up, int* player1_down,int* player1_left, int* player1_right, int* y,int* x, int longueur, int largeur, int dy, int dx) {
    // Gestion des événements
    SDL_Event event;

    // Récupération de tous les événements en attente
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            *continuer = 0;
        }
        else if (event.type == SDL_KEYDOWN) {
            mouvement(event, player1_up, player1_down, player1_right, player1_left, 1);
        }
        else if (event.type == SDL_KEYUP) {
            mouvement(event, player1_up, player1_down, player1_right, player1_left, 0);
        }
    }

    // Gestion des mouvements des joueurs
    if (*player1_up && *y >= 0) {
        if (*player1_left && *x >= 0){
            *x -= dx;
        }
        else if (*player1_right && *x + largeur <= 800) {
            *x += dx;
        }
        *y -= dy;
    }
    else if (*player1_down && *y + longueur <= 600) {
        if (*player1_left && *x >= 0) {
            *x -= dx;
        }
        else if (*player1_right && *x + largeur <= 800) {
            *x += dx;
        }
        *y += dy;
    }
    else if (*player1_left && *x >= 0) {
        *x -= dx;
    }
    else if (*player1_right && *x + largeur <= 800) {
        *x += dx;
    }
}

int main() {
    // Variables de mouvement des joueurs
    int player1_up = 0;
    int player1_down = 0;
    int player1_left = 0;
    int player1_right = 0;


    // Position et dimension des joueurs
    int x = 200;
    int y = 200;
    int longueur = 100;
    int largeur = 100;

    // Vitesse de déplacement
    int dy = 10;
    int dx = 10;

    // Variable pour la boucle principale
    int continuer = 1;

    // Initialisation de SDL
    initSDL();

    // Création de la fenêtre
    SDL_Window* window = initWindow();

    // Création du rendu
    SDL_Renderer* renderer = initRenderer(window);

    // Initialisation de SDL_ttf
    initTTF();

    // Boucle principale
    while (continuer) {
        // Gestion des événements et des mouvements
        controller(&continuer, &player1_up, &player1_down, &player1_left, &player1_right, &y, &x, largeur, longueur, dy, dx);
        
        // Effacement du rendu
        clearRenderer(renderer);

        // Dessiner les joueurs
        SDL_Rect player1 = { x, y, largeur, longueur };
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &player1);

        // Mettre à jour l'affichage
        SDL_RenderPresent(renderer);

        // Pause courte pour éviter une utilisation excessive du processeur
        SDL_Delay(10);
    }

    // Libération des ressources
    freeAll(renderer, window);

    return 0;
}
