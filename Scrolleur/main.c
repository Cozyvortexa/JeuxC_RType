#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <time.h>


#define LONGUEUR_LASER 40
#define LARGEUR_LASER 10
#define DX_LASER 20
#define MAX_LASER 10
#define LONGUEURPLAYER 100
#define LARGEURPLAYER 100
#define MAX_ENNEMIS 10


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

TTF_Font* loadFont() {
    TTF_Font* font = TTF_OpenFont("../Font/ARCADECLASSIC.TTF", 12);
    if (font == NULL) {
        fprintf(stderr, "Erreur lors du chargement de la police: %s\n",
            TTF_GetError());
        TTF_Quit();
        return NULL;
    }
    return font;
}

void afficherText(const char* text, SDL_Renderer* renderer, int yText, int textWidth, int textHeight) {
    TTF_Font* font = loadFont();
    // Création de la couleur
    SDL_Color noir = { 255, 255, 255 };

    // Rendu du texte
    SDL_Surface* texteSurface = TTF_RenderText_Solid(font, text, noir);
    if (texteSurface == NULL) {
        fprintf(stderr, "Erreur lors du rendu du texte: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, texteSurface);
    if (texture == NULL) {
        fprintf(stderr, "Erreur lors de la création de la texture du texte: %s\n", SDL_GetError());
        SDL_FreeSurface(texteSurface);
        TTF_CloseFont(font);
        return;
    }

    // Positionnement du texte à l'écran (par exemple, au centre)
    SDL_Rect destRect = { (1920 - textWidth) / 2, yText, textWidth, textHeight };

    // Affichage du texte à l'écran
    SDL_RenderCopy(renderer, texture, NULL, &destRect);

    // Libération des ressources
    SDL_FreeSurface(texteSurface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

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
//comme load music mais pour les bruitages
Mix_Chunk* loadSon(const char* path) {
    Mix_Chunk* son = Mix_LoadWAV(path);
    if (!son) {
        printf("Erreur lors du chargement du chunk : %s\n", Mix_GetError());
        return NULL;
    }
    return son;
}


void damage(struct Ennemi* ennemis, int idEnnemi, int degat, int* score) {
    ennemis[idEnnemi].pv -= degat;  // prend des degats
    if (ennemis[idEnnemi].pv <= 0 && ennemis[idEnnemi].idEnnemie == idEnnemi) {  // on suprimme la texture de l'ennemi mort et on passe sa valeur a null pour empécher son affichage dans display
        ennemis[idEnnemi].enVie = 0;
        *score += ennemis[idEnnemi].score;
    }
}

void mouvementRectiligneEnnemis(struct Ennemi* ennemis, int idEnnemie, int vitesse) {
    if (ennemis[idEnnemie].posEtSize.x > 0) {
        ennemis[idEnnemie].posEtSize.x -= vitesse;
    }
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



int creerLaser(int* x, int* y, int laserCount, Uint32* lastShootTime,
    int  player1_shoot, int shootDelay, Uint32 elapsedTime, SDL_Rect* lasers,
    Mix_Chunk* laserbeam) {

    if (player1_shoot) {
        SDL_Rect laser = { *x + LONGUEURPLAYER, *y + (LARGEURPLAYER / 2) -
            (LARGEUR_LASER / 2), LONGUEUR_LASER, LARGEUR_LASER };
        Uint32 currentTime = SDL_GetTicks();
        if (player1_shoot && laserCount < MAX_LASER && elapsedTime >= shootDelay) {
            lasers[laserCount] = laser;
            (laserCount) += 1;
            *lastShootTime = currentTime; // Mettre à jour le temps du dernier tir
            Mix_PlayChannel(-1, laserbeam, 0);
        }
    }// creerLaser prend pas directement l'emplacement memoire de laserCount il prend sa valeur qu'il retourne apres avoir fais son travail
    return laserCount;
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

void drawLaser(SDL_Renderer* renderer, int* laserCount, SDL_Rect* lasers) {
    for (int i = 0; i < *laserCount; ++i) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &lasers[i]);
        lasers[i].x += DX_LASER;
        if (lasers[i].x > 1920) {
            // Supprimer le laser qui a quitté l'écran en le remplaçant par le dernier laser du tableau
            lasers[i] = lasers[*laserCount - 1];
            (*laserCount)--;
        }
    }
}

struct Ennemi initEnnemi(int id, int pv, int x, int y, int w, int h,
    SDL_Texture* spriteTexture, int score) { // création d'un Ennemi 
    struct Ennemi newEnnemi;
    newEnnemi.idEnnemie = id;
    newEnnemi.pv = pv;
    newEnnemi.posEtSize.x = x;
    newEnnemi.posEtSize.y = y;
    newEnnemi.posEtSize.w = w;
    newEnnemi.posEtSize.h = h;
    newEnnemi.spriteTexture = spriteTexture;
    newEnnemi.enVie = 1;
    newEnnemi.score = score;
    return newEnnemi;
}

void display(struct Ennemi* ennemis, int nombreEnnemis, SDL_Renderer* renderer) {
    for (int i = 0; i < nombreEnnemis; i++) {
        if (ennemis[i].enVie != 0) {
            SDL_Rect constructeur = { ennemis[i].posEtSize.x, ennemis[i].posEtSize.y, ennemis[i].posEtSize.w, ennemis[i].posEtSize.h };
            SDL_RenderCopy(renderer, ennemis[i].spriteTexture, NULL, &constructeur);
        }
    }
}

void addEnnemi(struct Ennemi* tableauEnnemis, int id, int pv, int x,
    int y, int w, int h, SDL_Texture* spriteTexture, int score) {
    tableauEnnemis[id] = initEnnemi(id, 3, x, y, w, h, spriteTexture, score);
}


int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    int left1 = x1;
    int right1 = x1 + w1;
    int top1 = y1;
    int bottom1 = y1 + h1;

    int left2 = x2;
    int right2 = x2 + w2;
    int top2 = y2;
    int bottom2 = y2 + h2;

    // Vérifier qu'il n'y a aucune condition 
    if (bottom1 <= top2 || top1 >= bottom2 || right1 <= left2 || left1 >= right2) {
        return 0;
    }
    return 1;
}


SDL_Texture* drawEntite(SDL_Renderer* renderer, const char* lien) {  // Permet de creer une texture a partir d'un lien vers un fichier BMP
    SDL_Surface* spriteSurface = SDL_LoadBMP(lien);
    if (spriteSurface == NULL) {
        printf("Impossible de charger l'image : %s\n", SDL_GetError());
    }
    SDL_Texture* spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
    SDL_FreeSurface(spriteSurface);
    if (spriteTexture == NULL) {
        printf("Impossible de creer la texture : %s\n", SDL_GetError());
    }
    return spriteTexture;
}

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
    SDL_Texture* texturefond = loadTexture
    ("../Image/Background/parallax-space-backgound.png", renderer);
    SDL_Texture* texturepoudre = loadTexture
    ("../Image/Background/parallax-space-stars.png", renderer);
    SDL_Texture* textureplanetloin = loadTexture
    ("../Image/Background/parallax-space-far-planets.png", renderer);
    SDL_Texture* textureetoile = loadTexture
    ("../Image/Background/parallax-space-ring-planet.png", renderer);
    SDL_Texture* textureplanetproche = loadTexture
    ("../Image/Background/parallax-space-big-planet.png", renderer);
    renderTexture(texturefond, renderer, 0, 0, 1920, 1080);
    renderTexture(textureetoile, renderer, *xbackgroundetoile, *ybackground,
        255, 575);
    renderTexture(textureplanetproche, renderer, *xbackgroundetoile + 1000,
        *ybackground + 500, 440, 435);
    renderTexture(textureplanetloin, renderer, *xbackgroundplanet, *ybackground
        , 1920, 1080);
    renderTexture(texturepoudre, renderer, *xbackground, *ybackground,
        1920, 1080);
    renderTexture(texturepoudre, renderer, *xbackground + 1920, *ybackground,
        1920, 1080);
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
    freeBackground(texturefond, texturepoudre, textureplanetloin, textureetoile
        , textureplanetproche);
}


void scoreAffichage(SDL_Renderer* renderer, int* score) {
    char affichageScore[10];
    snprintf(affichageScore, 10, "%d", *score);
    SDL_Rect posTexte = { 500, 500, 0, 0 };

    TTF_Font* font = TTF_OpenFont("../JeuxC_RType/Arial.ttf", 24);
    SDL_Color couleurScore = { 255, 255, 255, 255 };

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, affichageScore, couleurScore);
    if (textSurface == NULL) {
        printf("Impossible de créer la surface texte : %s", TTF_GetError());
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
        printf("Impossible de créer la texture texte : %s", SDL_GetError());
        SDL_FreeSurface(textSurface);
    }

    SDL_QueryTexture(textTexture, NULL, NULL, &posTexte.w, &posTexte.h);
    SDL_RenderCopy(renderer, textTexture, NULL, &posTexte);
    TTF_CloseFont(font);
}


struct Joueurs initJoueur(int x, int y) {
    struct Joueurs joueurs;
    joueurs.pv = 3;
    joueurs.x = x;
    joueurs.y = y;
    return joueurs;
}

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
    int* menu_select, int* choix, int* continuer, int* continuermenu,
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
            *continuermenu = 0;
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
parametre() {
    int boucleParametre = 1;

        while (boucleParametre){
            
        }
}

void traitementChoix(int choix, int* continuermenu, int* continuer) {
    switch (choix) {
    case 1: //lancer le jeu
        printf("choix 1");
        *continuermenu = 0;
        *continuer = 1;
        break;
    case 2: //paramètre
        printf("choix 2");
        /*parametre();*/
        break;
    case 3: //quitter
        printf("choix 3");
        *continuermenu = 0;
        *continuer = 0;
        break;
    }
}

void menu(SDL_Renderer* renderer, SDL_Texture* textureplayer, int* x, int* y,
    int xbackground, int ybackground, int xbackgroundplanet,
    int xbackgroundetoile, int* continuer) {
    Mix_Music* musiqueMenu = loadMusic("../Son/StageSelection.wav");
    Mix_PlayMusic(musiqueMenu, -1);
    int continuermenu = 1;
    int boucle = 1;
    int menu_up = 0;
    int menu_down = 0;
    int menu_select = 0;
    int choix = 1;
    while (continuermenu) {
        while (boucle) {
            clearRenderer(renderer);
            tapisBackground(renderer, &xbackground, &ybackground,
                &xbackgroundplanet, &xbackgroundetoile);
            renderTexture(textureplayer, renderer, *x, *y,
                LONGUEURPLAYER, LARGEURPLAYER); // Utilisation de *x et *y pour obtenir les valeurs
            afficherText("Starforce  Strike", renderer, 100, 1000, 200);
            afficherText("JOUER", renderer, 300, 300, 300);
            afficherText("PARAMETRE", renderer, 500, 500, 300);
            afficherText("QUITTER", renderer, 700, 400, 300);
            controlleurMenu(renderer, &menu_up, &menu_down, &menu_select,
                &choix, continuer, &continuermenu, &boucle, x, y); // Utilisation de x et y
            SDL_RenderPresent(renderer);
            SDL_Delay(10);
        }
        if (continuermenu) {
            traitementChoix(choix, &continuermenu, continuer);
        }
    }
}

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

static int hitDelay = 3000;
static Uint32 lasthitTime = 0;



// Tableau de lasers
SDL_Rect lasers[MAX_LASER];
int laserCount = 0;
// Variable pour la boucle principale
int continuer = 1;

// creation struct
int nombreEnnemis = 1;
struct Ennemi ennemis[MAX_ENNEMIS];


int main() {

   

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
    Mix_Chunk* explosion = loadSon("../Son/explosion.wav"); //bruitages pour l'explosion des vaisseau

    menu(renderer, textureplayer, &x, &y, &xbackground, ybackground, &xbackgroundplanet, &xbackgroundetoile, &continuer);

    SDL_Texture* spriteTexture = drawEntite(renderer,
        "../Image/ennemieVaisseau.bmp");

    struct Joueurs joueur = initJoueur(x, y);

    addEnnemi(ennemis, 0, 3, 800, 100, 100, 100, spriteTexture, 300);


    Mix_PlayMusic(musique, -1);
    // Boucle principale
    while (continuer) {

        // Effacement du rendu
        clearRenderer(renderer);

        tapisBackground(renderer, &xbackground, &ybackground,
            &xbackgroundplanet, &xbackgroundetoile);


        mouvementRectiligneEnnemis(ennemis, 0, 1);



        display(ennemis, nombreEnnemis, renderer);

        // Gestion des événements et des mouvements

        controller(&continuer, &player1_up, &player1_down, &player1_left,
            &player1_right, &player1_shoot);
        deplacement(&y, &x, &laserCount, player1_shoot, player1_up,
            player1_left, player1_down, player1_right, lasers, laserbeam);


        for (int i = 0; i < nombreEnnemis; i++) {
            for (int i2 = 0; i2 < laserCount; i2++) {
                if (collision(ennemis[i].posEtSize.x, ennemis[i].posEtSize.y, ennemis[i].posEtSize.w, ennemis[i].posEtSize.h, lasers[i2].x, lasers[i2].y, lasers[i2].w, lasers[i2].h)) {
                    lasers[i2] = lasers[laserCount - 1];
                    (laserCount)--;
                    damage(ennemis, i, 1, &score);
                }
            }
            if (collision(x, y, LONGUEURPLAYER, LARGEUR_LASER, ennemis[i].posEtSize.x, ennemis[i].posEtSize.y, ennemis[i].posEtSize.w, ennemis[i].posEtSize.h) && ennemis[i].pv > 0) {
                Uint32 currentTime = SDL_GetTicks();
                Uint32 elapsedTime = currentTime - lasthitTime;
                if (elapsedTime >= hitDelay) {
                    joueur.pv -= 1;
                    x = joueur.x;
                    y = joueur.y;
                    lasthitTime = currentTime;
                }

                if (joueur.pv <= 0) {
                    printf("mort");
                }
            }
        }
        /*scoreAffichage(renderer, &score);*/

        // Gestion des lasers
        drawLaser(renderer, &laserCount, lasers);

        // Dessiner le joueur

        renderTexture(textureplayer, renderer, x, y, LONGUEURPLAYER, LARGEURPLAYER);

        // Mettre à jour l'affichage
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    // Libération des ressources
    freeAll(renderer, window, musique);

    return 0;
}