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
    // Cr�ation de la fen�tre SDL
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
    // Cr�ation du rendu SDL
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
    // Lib�ration des ressources SDL
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

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        printf("Erreur lors du chargement de l'image : %s\n", IMG_GetError());
        exit(1);
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // Lib�rer la surface, nous n'en avons plus besoin
    if (!texture) {
        printf("Erreur lors de la cr�ation de la texture : %s\n", SDL_GetError());
        exit(1);
    }
    return texture;
}

void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y, int width, int height) {
    SDL_Rect destRect = { x, y, width, height }; // Utilisez les dimensions sp�cifi�es
    SDL_RenderCopy(renderer, texture, NULL, &destRect); // Afficher la texture � la position sp�cifi�e
}

SDL_Rect draw(SDL_Renderer* renderer, int x, int y, int longueurplayer, int largeurplayer, int R, int G, int B) {
    SDL_Rect gameobj = { x, y, longueurplayer, largeurplayer };
    SDL_SetRenderDrawColor(renderer, R, G, B, 255);
    SDL_RenderFillRect(renderer, &gameobj);
    return gameobj;
}


void mouvement(SDL_Event evenement, int* player1_up, int* player1_down, int* player1_right, int* player1_left, int* player1_shoot, int valeur) {
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
    case SDLK_SPACE:
        *player1_shoot = valeur;
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

void controller(SDL_Renderer* renderer, int* continuer, int* player1_up, int* player1_down,int* player1_left, int* player1_right,int* player1_shoot, int* y,int* x, int largeurplayer, int longueurplayer, int dy, int dx) {
    // Gestion des �v�nements
    SDL_Event event;

    // R�cup�ration de tous les �v�nements en attente
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            *continuer = 0;
        }
        else if (event.type == SDL_KEYDOWN) {
            mouvement(event, player1_up, player1_down, player1_right, player1_left, player1_shoot, 1);
        }
        else if (event.type == SDL_KEYUP) {
            mouvement(event, player1_up, player1_down, player1_right, player1_left, player1_shoot, 0);
        }
    }

    // Gestion des mouvements des joueurs
    if (*player1_up && *y >= 0) {
        if (*player1_left && *x >= 0){
            *x -= dx;
        }
        else if (*player1_right && *x + longueurplayer <= 800) {
            *x += dx;
        }
        *y -= dy;
    }
    else if (*player1_down && *y + largeurplayer <= 600) {
        if (*player1_left && *x >= 0) {
            *x -= dx;
        }
        else if (*player1_right && *x + longueurplayer <= 800) {
            *x += dx;
        }
        *y += dy;
    }
    else if (*player1_left && *x >= 0) {
        *x -= dx;
    }
    else if (*player1_right && *x + longueurplayer <= 800) {
        *x += dx;
    }

}

int main() {
    // Variables de mouvement des joueurs
    int player1_up = 0;
    int player1_down = 0;
    int player1_left = 0;
    int player1_right = 0;
    int player1_shoot = 0;

    // Position et dimension des joueurs
    int x = 200;
    int y = 200;
    int largeurplayer = 100;
    int longueurplayer = 100;

    int xlaser = x;
    int ylaser = y + 200;
    int longueurlaser = 10;
    int largeurlaser = 10;

    // Vitesse de d�placement
    int dy = 15;
    int dx = 15;
    int dxlaser = 20;

    // Variable pour la boucle principale
    int continuer = 1;

    // Initialisation de SDL
    initSDL();

    // Cr�ation de la fen�tre
    SDL_Window* window = initWindow();

    // Cr�ation du rendu
    SDL_Renderer* renderer = initRenderer(window);

    //Cr�ation de l'image Player
    SDL_Texture* texture = loadTexture("C:/Users/qrecco/Documents/Projet c/JeuxC_RType/Image/Player1.png", renderer);

    // Initialisation de SDL_ttf
    initTTF();

    int laser_active = 0; // Ajout d'une variable pour indiquer si le laser est actif

    // Boucle principale
    while (continuer) {
        // Effacement du rendu
        clearRenderer(renderer);
        // Gestion des �v�nements et des mouvements
        controller(renderer, &continuer, &player1_up, &player1_down, &player1_left, &player1_right, &player1_shoot, &y, &x, longueurplayer, largeurplayer, dy, dx);

        // Si le joueur tire et que le laser n'est pas d�j� actif
        if (player1_shoot && !laser_active) {
            xlaser = x+largeurplayer; // Position initiale du laser (horizontale)
            ylaser = (y + (longueurplayer/2))-(longueurlaser/2); // Position initiale du laser (verticale)
            laser_active = 1; // Activer le laser
        }

        // Si le laser est actif, affichez-le et faites-le avancer
        if (laser_active) {
            SDL_Rect laserRect = draw(renderer, xlaser, ylaser, longueurlaser, largeurlaser, 255, 0, 0); // Affichage du laser
            SDL_RenderFillRect(renderer, &laserRect); // Affichage du rectangle du laser sur le rendu
            xlaser += dxlaser; // D�placement horizontal du laser
            if (xlaser > 800) { // Si le laser sort de l'�cran
                laser_active = 0; // D�sactiver le laser
            }
        }


        // Dessiner les joueurs
        renderTexture(texture, renderer, x, y, longueurplayer, largeurplayer);

        // Mettre � jour l'affichage
        SDL_RenderPresent(renderer);

        // Pause courte pour �viter une utilisation excessive du processeur
        SDL_Delay(10);
    }




    // Lib�ration des ressources
    freeAll(renderer, window);

    return 0;
}
