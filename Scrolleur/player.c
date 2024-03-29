#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <time.h>
#include <SDL_mixer.h>

#include "player.h"
#include "laser.h"

#define LONGUEURPLAYER 100
#define LARGEURPLAYER 100

struct Joueurs {
    int x;
    int y;
    int pv;
};

void action(SDL_Event evenement, int* player1_up, int* player1_down, int* player1_right, int* player1_left, int* player1_shoot, int valeur) {
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

void controller(int* continuer, int* player1_up, int* player1_down,
    int* player1_left, int* player1_right, int* player1_shoot) {
    // Gestion des événements
    SDL_Event event;

    // Récupération de tous les événements en attente
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            *continuer = 0;
        }
        else if (event.type == SDL_KEYDOWN) {
            action(event, player1_up, player1_down, player1_right,
                player1_left, player1_shoot, 1);
        }
        else if (event.type == SDL_KEYUP) {
            action(event, player1_up, player1_down, player1_right,
                player1_left, player1_shoot, 0);
        }
    }
}

int deplacementDiagonale(int player1_left, int player1_right, int x) {
    if (player1_left && x >= 0) {
        x -= 10;
    }
    else if (player1_right && x + LONGUEURPLAYER <= 1920) {
        x += 10;
    }
    return x;
}

void deplacement(int* y, int* x, int* laserCount, int player1_shoot, int player1_up
    , int player1_left, int player1_down, int player1_right, SDL_Rect* lasers,
    Mix_Chunk* laserbeam) {
    static int shootDelay = 300; // Délai entre chaque tir en millisecondes
    static Uint32 lastShootTime = 0; // Temps du dernier tir
    Uint32 currentTime = SDL_GetTicks();  // Calcul du temps écoulé depuis le dernier tir en millisecondes
    Uint32 elapsedTime = currentTime - lastShootTime;

    if (player1_up && *y >= 0) {
        *x = deplacementDiagonale(player1_left, player1_right, *x);
        *laserCount = creerLaser(x, y, *laserCount, &lastShootTime,
            player1_shoot, shootDelay, elapsedTime, lasers, laserbeam);
        *y -= 10;
    }
    else if (player1_down && *y + LARGEURPLAYER <= 1080) {
        *x = deplacementDiagonale(player1_left, player1_right, *x);
        *laserCount = creerLaser(x, y, *laserCount, &lastShootTime,
            player1_shoot, shootDelay, elapsedTime, lasers, laserbeam);
        *y += 10;
    }
    else if (player1_left && *x >= 0) {
        *laserCount = creerLaser(x, y, *laserCount, &lastShootTime,
            player1_shoot, shootDelay, elapsedTime, lasers, laserbeam);
        *x -= 10;
    }
    else if (player1_right && *x + LONGUEURPLAYER <= 1920) {
        *laserCount = creerLaser(x, y, *laserCount, &lastShootTime,
            player1_shoot, shootDelay, elapsedTime, lasers, laserbeam);
        *x += 10;
    }
    else if (player1_shoot) {
        *laserCount = creerLaser(x, y, *laserCount, &lastShootTime,
            player1_shoot, shootDelay, elapsedTime, lasers, laserbeam);
    }
}

struct Joueurs initJoueur(int x, int y) {
    struct Joueurs joueurs;
    joueurs.pv = 3;
    joueurs.x = x;
    joueurs.y = y;
    return joueurs;
}

