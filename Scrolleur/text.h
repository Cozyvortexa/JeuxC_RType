#pragma once
#ifndef TEXT_H
#define TEXT_H

int initTTF();

TTF_Font* loadFont();


void afficherText(const char* text, SDL_Renderer* renderer, int yText,
	int textWidth, int textHeight);

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);

void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x,
	int y, int width, int height);

#endif