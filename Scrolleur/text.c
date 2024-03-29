#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <time.h>


#include "text.h"

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

