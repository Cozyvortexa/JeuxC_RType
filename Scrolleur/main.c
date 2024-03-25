#include <stdio.h>
#include <SDL.h>

void initSDL() {
    // init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to load SDL\n");
        return -1;
    }

}

SDL_Window* initWindow() {
    // create SDL window
    SDL_Window* window = SDL_CreateWindow("Scrolleur",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        800, 600, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Failed to create window\n");
        SDL_Quit();
        return -1;
    }
    return window;
}

SDL_Renderer* initRender(SDL_Window* window) {
    // create render 
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("Failed to create renderer\n");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    return renderer;
}


void clearRendu(SDL_Renderer* renderer) {
    // Clear rendu + set white color 
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}

struct Joueurs {
    int id;
    int pv;
};

struct Ennemie {
    int idEnnemie;
    int pv;
    int x;
    int y;
    int size;
};

SDL_Rect draw(SDL_Renderer* renderer, int x, int y, int largeur, int longueur, int R, int G, int B) {
    SDL_Rect gameobj = { x, y, largeur, longueur };
    SDL_SetRenderDrawColor(renderer, R, G, B, 255);
    SDL_RenderFillRect(renderer, &gameobj);
    return gameobj;
}


struct Ennemie degat(struct Ennemie ennemie) {
    ennemie.pv -= 1;
    return ennemie;
}



struct Ennemie initEnnemie(int id, int pv, int x, int y, int size) {
    struct Ennemie newEnnemie;
    int idEnnemie = id;
    newEnnemie.pv = pv;
    newEnnemie.x = x;
    newEnnemie.y = y;
    newEnnemie.size = size;
    return newEnnemie;
}

void freeEnnemie(SDL_Rect test) {
   
}
void free(SDL_Renderer* renderer, SDL_Window* window) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

//void controlleur() {
//    // Controlleur
//    SDL_Event evenement;
//
//    // Récupérer tous les événements en attente
//    while (SDL_PollEvent(&evenement) != 0) {
//        if (evenement.type == SDL_KEYDOWN) {
//            if(evenement.key.keysym.sym == SDLK_z){
//
//        }
//    }

int xCube = 50;
int yCube = 50;
int sizeCube = 100;


int main() {
    initSDL();
    SDL_Window* window = initWindow();
    SDL_Renderer* renderer = initRender(window);


    struct Ennemie cube = initEnnemie(1, 3, 100, 100, 50);

    SDL_Rect test = draw(renderer, xCube, yCube, sizeCube, sizeCube, 255, 255, 255);

    while (1) {
        
        // Code temporaire
        SDL_Event evenement;
        while (SDL_PollEvent(&evenement) != 0) {
            if (evenement.type == SDL_KEYDOWN) {
                if (evenement.key.keysym.sym == SDLK_z) {
                    cube = degat(cube);
                }

            }
        }
        if (cube.pv == 0) {
            freeEnnemie(test);
        }

        //Code temporaire
        
        freeEnnemie(test);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
    free(renderer, window);
	return 0;
}

