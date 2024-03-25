#include <stdio.h>
#include <SDL.h>

struct Joueurs {
	int id;
	int pv;
};


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



int main() {
	printf("hello");
	return 0;
}

