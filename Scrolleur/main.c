#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <time.h>


#define LONGUEUR_LASER 10
#define LARGEUR_LASER 10
#define DX_LASER 20
#define MAX_LASER 10

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
        1920, 1080, SDL_WINDOW_SHOWN);
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

int initAudio() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Erreur lors de l'initialisation de SDL : %s\n", SDL_GetError());
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 3, 2048) < 0) {
        printf("Erreur lors de l'initialisation de SDL Mixer : %s\n", Mix_GetError());
        SDL_Quit(); // Assurez-vous de quitter SDL en cas d'échec
        return 1;
    }
    return 0;
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

void freeAll(SDL_Renderer* renderer, SDL_Window* window, Mix_Music* musique) {
    // Libération des ressources SDL
    Mix_FreeMusic(musique);
    Mix_CloseAudio();
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
        return NULL; // Retourne NULL pour indiquer une erreur
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface); // Libérer la surface, nous n'en avons plus besoin
    if (!texture) {
        printf("Erreur lors de la création de la texture : %s\n", SDL_GetError());
        return NULL; // Retourne NULL pour indiquer une erreur
    }
    return texture;
}

Mix_Music* loadMusic(const char* path) {
    Mix_Music* musique = Mix_LoadMUS(path);
    if (!musique) {
        printf("Erreur lors du chargement de la musique : %s\n", Mix_GetError());
        return NULL;
    }
    return musique;
}
Mix_Chunk* loadSon(const char* path) {
    Mix_Chunk* son = Mix_LoadWAV(path);
    if (!son) {
        printf("Erreur lors du chargement du chunk : %s\n", Mix_GetError());
        return NULL;
    }
    return son;
}


void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y, int width, int height) {
    SDL_Rect destRect = { x, y, width, height }; // Utilisez les dimensions spécifiées
    SDL_RenderCopy(renderer, texture, NULL, &destRect); // Afficher la texture à la position spécifiée
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
    }
}

void controller(SDL_Renderer* renderer, int* continuer, int* player1_up, int* player1_down, int* player1_left, int* player1_right, int* player1_shoot, int* y, int* x, int longueurplayer, int largeurplayer, int dy, int dx, SDL_Rect* lasers, int* laserCount, Mix_Chunk* laserbeam) {
    static int shootDelay = 300; // Délai entre chaque tir en millisecondes
    static Uint32 lastShootTime = 0; // Temps du dernier tir
    // Gestion des événements
    SDL_Event event;

    // Calcul du temps écoulé depuis le dernier tir en millisecondes
    Uint32 currentTime = SDL_GetTicks();
    Uint32 elapsedTime = currentTime - lastShootTime;
    
    // Récupération de tous les événements en attente
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
        if (*player1_left && *x >= 0) {
            *x -= dx;
        }
        else if (*player1_right && *x + longueurplayer <= 1920) {
            *x += dx;
        }
        // Gestion des tirs de laser
        if (*player1_shoot && *laserCount < MAX_LASER && elapsedTime >= shootDelay) {
            SDL_Rect laser = { *x + longueurplayer, *y + (largeurplayer / 2) - (LONGUEUR_LASER / 2), LONGUEUR_LASER, LARGEUR_LASER };
            lasers[*laserCount] = laser;
            *laserCount += 1;
            lastShootTime = currentTime; // Mettre à jour le temps du dernier tir
            Mix_PlayChannel(-1,laserbeam, 0);
        }
        *y -= dy;
    }
    else if (*player1_down && *y + largeurplayer <= 1080) {
        if (*player1_left && *x >= 0) {
            *x -= dx;
        }
        else if (*player1_right && *x + longueurplayer <= 1920) {
            *x += dx;
        }
        // Gestion des tirs de laser
        if (*player1_shoot && *laserCount < MAX_LASER && elapsedTime >= shootDelay) {
            SDL_Rect laser = { *x + longueurplayer, *y + (largeurplayer / 2) - (LONGUEUR_LASER / 2), LONGUEUR_LASER, LARGEUR_LASER };
            lasers[*laserCount] = laser;
            *laserCount += 1;
            lastShootTime = currentTime; // Mettre à jour le temps du dernier tir
            Mix_PlayChannel(-1,laserbeam, 0);
        }
        *y += dy;
    }
    else if (*player1_left && *x >= 0) {
        // Gestion des tirs de laser
        if (*player1_shoot && *laserCount < MAX_LASER && elapsedTime >= shootDelay) {
            SDL_Rect laser = { *x + longueurplayer, *y + (largeurplayer / 2) - (LONGUEUR_LASER / 2), LONGUEUR_LASER, LARGEUR_LASER };
            lasers[*laserCount] = laser;
            *laserCount += 1;
            lastShootTime = currentTime; // Mettre à jour le temps du dernier tir
            Mix_PlayChannel(-1,laserbeam, 0);
        }
        *x -= dx;
    }
    else if (*player1_right && *x + longueurplayer <= 1920) {
        // Gestion des tirs de laser
        if (*player1_shoot && *laserCount < MAX_LASER && elapsedTime >= shootDelay) {
            SDL_Rect laser = { *x + longueurplayer, *y + (largeurplayer / 2) - (LONGUEUR_LASER / 2), LONGUEUR_LASER, LARGEUR_LASER };
            lasers[*laserCount] = laser;
            *laserCount += 1;
            lastShootTime = currentTime; // Mettre à jour le temps du dernier tir
            Mix_PlayChannel(-1,laserbeam, 0);
        }
        *x += dx;
    }
    // Gestion des tirs de laser
    else if (*player1_shoot && *laserCount < MAX_LASER && elapsedTime >= shootDelay) {
        SDL_Rect laser = { *x + longueurplayer, *y + (largeurplayer / 2) - (LONGUEUR_LASER / 2), LONGUEUR_LASER, LARGEUR_LASER };
        lasers[*laserCount] = laser;
        *laserCount += 1;
        lastShootTime = currentTime; // Mettre à jour le temps du dernier tir
        Mix_PlayChannel(-1,laserbeam, 0);
    }
}


void tapisbackground(SDL_Renderer* renderer, int* xbackground, int* ybackground, int* xbackgroundplanet, int* xbackgroundetoile) {
    int dxstar = -3;
    int dxplanetloin = -2;
    int dxetoile = -1;


    SDL_Texture* texturefond = loadTexture("../Image/Background/parallax-space-backgound.png", renderer);
    SDL_Texture* texturepoudre = loadTexture("../Image/Background/parallax-space-stars.png", renderer);
    SDL_Texture* textureplanetloin = loadTexture("../Image/Background/parallax-space-far-planets.png", renderer);
    SDL_Texture* textureetoile = loadTexture("../Image/Background/parallax-space-ring-planet.png", renderer);
    SDL_Texture* textureplanetproche = loadTexture("../Image/Background/parallax-space-big-planet.png", renderer);


    renderTexture(texturefond, renderer, 0, 0, 1920, 1080);
    renderTexture(textureetoile, renderer, *xbackgroundetoile, *ybackground, 255, 575);
    renderTexture(textureplanetproche, renderer, *xbackgroundetoile+1000, *ybackground+500, 440, 435);
    renderTexture(textureplanetloin, renderer, *xbackgroundplanet, *ybackground, 1920, 1080);
    renderTexture(texturepoudre, renderer, *xbackground, *ybackground, 1920, 1080);
    renderTexture(texturepoudre, renderer, *xbackground+1920, *ybackground, 1920, 1080);
    
    *xbackground += dxstar;
    if (*xbackground<=(- 1920)) {
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
    int y = 500;
    int largeurplayer = 100;
    int longueurplayer = 100;
    // Vitesse de déplacement
    int dy = 10;
    int dx = 10;

    //variable background
    int xbackground = 0;
    int ybackground = 0;
    int xbackgroundplanet = 0;
    int xbackgroundetoile = 2000;
    
    
    // Tableau de lasers
    SDL_Rect lasers[MAX_LASER];
    int laserCount = 0; 
    // Variable pour la boucle principale
    int continuer = 1;
    
    // Initialisation
    initSDL();
    SDL_Window* window = initWindow();
    SDL_Renderer* renderer = initRenderer(window);
    initAudio();
    initTTF();

    // Chargement des textures et des sons
    SDL_Texture* textureplayer = loadTexture("../Image/Player1.png", renderer);
    Mix_Music* musique = loadMusic("../Son/8-Bit Robotics.wav");
    Mix_Chunk* laserbeam = loadSon("../Son/laser-gun.wav");
    Mix_PlayMusic(musique, -1);



    // Boucle principale
    while (continuer) {
        // Effacement du rendu
        clearRenderer(renderer);

        tapisbackground(renderer, &xbackground, &ybackground, &xbackgroundplanet, &xbackgroundetoile);


        // Gestion des événements et des mouvements
        controller(renderer, &continuer, &player1_up, &player1_down, &player1_left, &player1_right, &player1_shoot, &y, &x, longueurplayer, largeurplayer, dy, dx, lasers, &laserCount, laserbeam);

        // Gestion des lasers
        for (int i = 0; i < laserCount; ++i) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &lasers[i]);
            lasers[i].x += DX_LASER;
            if (lasers[i].x > 1920) {
                // Supprimer le laser qui a quitté l'écran en le remplaçant par le dernier laser du tableau
                lasers[i] = lasers[laserCount - 1];
                laserCount--;
            }
        }

        // Dessiner le joueur
        
        renderTexture(textureplayer, renderer, x, y, longueurplayer, largeurplayer);

        // Mettre à jour l'affichage
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    // Libération des ressources
    freeAll(renderer, window, musique);

    return 0;
}