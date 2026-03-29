#ifndef HISTOGRAMA_H
#define HISTOGRAM_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

typedef struct HIST{
    int *h;
} HIST;

void plotarHistograma(SDL_Surface *surface, HIST *hist);
SDL_Surface* criarImagemHistograma(HIST *hist, int width, int height);

#endif