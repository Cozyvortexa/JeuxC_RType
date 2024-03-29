#pragma once
#ifndef INITGENERAL_H
#define INITGENERAL_H

#include <SDL_mixer.h>

void initSDL();

SDL_Window* initWindow();

SDL_Renderer* initRenderer(SDL_Window* window);

void clearRenderer(SDL_Renderer* renderer);

void freeAll(SDL_Renderer* renderer, SDL_Window* window, Mix_Music* musique);

#endif