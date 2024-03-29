#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <time.h>
#include <SDL_mixer.h>

#include "adversaire.h"
#include "adversaire2.h"
#include "adversaire3.h"
#include "background.h"
#include "initgeneral.h"
#include "laser.h"
#include "menu.h"
#include "menu2.h"
#include "player.h"
#include "score.h"
#include "son.h"
#include "text.h"


#define LONGUEUR_LASER 40
#define LARGEUR_LASER 10
#define DX_LASER 20
#define MAX_LASER 10
#define LONGUEURPLAYER 100
#define LARGEURPLAYER 100
#define MAX_ENNEMIS 10

struct Joueurs {
    int x;
    int y;
    int pv;
};
struct Ennemi {
    int idEnnemie;
    int pv;
    SDL_Rect posEtSize;
    SDL_Texture* spriteTexture;
    int enVie;
    int score;
};

// Variables de mouvement des joueurs
int player1_up = 0;
int player1_down = 0;
int player1_left = 0;
int player1_right = 0;
int player1_shoot = 0;
// Position et dimension des joueurs
int x = 200;
int y = 500;

// Score
int score = 0;
// Vitesse de déplacement
int dy = 10;
int dx = 10;

//variable background
int xbackground = 0;
int ybackground = 0;
int xbackgroundplanet = 0;
int xbackgroundetoile = 2000;

int nombreEnnemis1 = 3;
int nombreEnnemis2 = 5;

static int hitDelay = 3000;
static Uint32 lasthitTime = 0;

int triggerVague2 = 1;

// Tableau de lasers
SDL_Rect lasers[MAX_LASER];
int laserCount = 0;
// Variable pour la boucle principale
int continuer = 1;

SDL_Texture* textureplayer = NULL;
Mix_Music* musique = NULL;
Mix_Chunk* laserbeam = NULL;
Mix_Chunk* explosion = NULL; //bruitages pour l'explosion des vaisseau


// creation struct
int nombreEnnemis = 1;
struct Ennemi ennemis[MAX_ENNEMIS];
struct Ennemi ennemis2[MAX_ENNEMIS];
void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y,
    int width, int height) {
    SDL_Rect destRect = { x, y, width, height }; // Utilisez les dimensions spécifiées
    SDL_RenderCopy(renderer, texture, NULL, &destRect); // Afficher la texture à la position spécifiée
}


void facto(SDL_Renderer* renderer,int* score, int* triggerVague2,int x,int y,
    SDL_Texture* textureplayer,int* laserCount, SDL_Rect* lasers) {
    scoreAffichage(renderer, score, triggerVague2);
    // Gestion des lasers
    drawLaser(renderer, laserCount, lasers);
    // Dessiner le joueur
    renderTexture(textureplayer, renderer, x, y, LONGUEURPLAYER,
        LARGEURPLAYER);
    // Mettre à jour l'affichage
    SDL_RenderPresent(renderer);
    SDL_Delay(10);
}
void facto2(struct Ennemie* ennemie, SDL_Renderer* renderer, int nombreEnnemis) {
    mouvementRectiligneEnnemis(ennemis, 0, 1);
    display(ennemis, nombreEnnemis, renderer);
}
void initiationGenerale() {
    initSDL();
    initAudio();
    initTTF();
}

int main() {
    // Initialisation
    initiationGenerale();
    SDL_Window* window = initWindow();
    SDL_Renderer* renderer = initRenderer(window);
    // Chargement des textures et des sons
    SDL_Texture* textureplayer = loadTexture("../Image/Player1.png", renderer);
    Mix_Music* musique = loadMusic("../Son/8-Bit Robotics.wav");
    Mix_Chunk* laserbeam = loadSon("../Son/laser-gun.wav");
    menu(renderer, textureplayer, &x, &y, &xbackground, &ybackground,
        &xbackgroundplanet, &xbackgroundetoile, &continuer, window);
    struct Joueurs joueur = initJoueur(x, y);
    initVague(ennemis, renderer, nombreEnnemis1, "../Image/ennemieUn.bmp",300);
    Mix_PlayMusic(musique, -1);
    // Boucle principale
    while (continuer) {
        // Effacement du rendu
        clearRenderer(renderer);
        tapisBackground(renderer, &xbackground, &ybackground,
            &xbackgroundplanet, &xbackgroundetoile);
        generateurDeVague(renderer, ennemis, ennemis2, &triggerVague2,
            &nombreEnnemis1, &nombreEnnemis2);

        // Gestion des événements et des mouvements
        controller(&continuer, &player1_up, &player1_down, &player1_left,
            &player1_right, &player1_shoot);
        deplacement(&y, &x, &laserCount, player1_shoot, player1_up,
            player1_left, player1_down, player1_right, lasers, laserbeam);
        touche(ennemis, joueur, lasthitTime, lasers, &x, &y, hitDelay,
            &laserCount, nombreEnnemis1, &score);
        touche(ennemis2, joueur, lasthitTime, lasers, &x, &y, hitDelay,
            &laserCount, nombreEnnemis2, &score);
        facto(renderer,&score,&triggerVague2,x,y,textureplayer,&laserCount,lasers);
    }freeAll(renderer, window, musique);
    return 0;}