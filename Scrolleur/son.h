#pragma once
#ifndef SON_H
#define SON_H

int initAudio();

Mix_Music* loadMusic(const char* path);
//comme load music mais pour les bruitages
Mix_Chunk* loadSon(const char* path);

#endif