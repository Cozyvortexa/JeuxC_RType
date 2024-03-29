#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <time.h>
#include <SDL_mixer.h>

#include "score.h"


void scoreAffichage(SDL_Renderer* renderer, int* score, int* nbrVague) {
    char affichageScore[20];
    int scoreAffiche = *score;
    snprintf(affichageScore, 20, "Vague%d score %d  ", *nbrVague, scoreAffiche);
    SDL_Rect posTexte = { 600, 850, 800, 350 };

    TTF_Font* font = TTF_OpenFont("../Font/ARCADECLASSIC.ttf", 24);
    SDL_Color couleurScore = { 255, 255, 255, 255 };

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, affichageScore,
        couleurScore);
    if (textSurface == NULL) {
        printf("Impossible de créer la surface texte : %s", TTF_GetError());

    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer,
        textSurface);
    if (textTexture == NULL) {
        printf("Impossible de créer la texture texte : %s", SDL_GetError());
        SDL_FreeSurface(textSurface);
    }

    //SDL_QueryTexture(textTexture, NULL, NULL, &posTexte.w, &posTexte.h);
    SDL_RenderCopy(renderer, textTexture, NULL, &posTexte);
    TTF_CloseFont(font);
}