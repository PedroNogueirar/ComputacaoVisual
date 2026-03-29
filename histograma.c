#include <stdlib.h>
#include<stdio.h>

#include "histograma.h"

void plotarHistograma(SDL_Surface *surface, HIST *hist)
{
    hist = malloc(sizeof(HIST));
    hist->h = malloc(sizeof(int) * 256);
    memset(hist->h, 0, sizeof(int));
    for (int y = 0; y < surface->h; y++)
    {
        Uint8 *row = (Uint8 *)surface->pixels + y * surface->pitch;

        for (int x = 0; x < surface->w; x++)
        {
            Uint8 *pixel = row + x;
            hist->h[*pixel]++;
        }
    }
}

SDL_Surface* criarImagemHistograma(HIST *hist, int width, int height)
{
    SDL_Surface *histSurface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);

    const SDL_PixelFormatDetails *preto = SDL_GetPixelFormatDetails(histSurface->format);
    const SDL_PixelFormatDetails *branco = SDL_GetPixelFormatDetails(histSurface->format);
    SDL_FillSurfaceRect(histSurface, NULL, SDL_MapRGB(preto, NULL, 0, 0, 0));

    int max = 0;
    for (int i = 0; i < 256; i++)
        if (hist->h[i] > max)
            max = hist->h[i];

    
    for (int i = 0; i < 256; i++)
    {
        int bar_height = (hist->h[i] * height) / max;

        SDL_Rect bar = {
            i * 2,
            height - bar_height,
            2,
            bar_height};

        SDL_FillSurfaceRect(
            histSurface,
            &bar,
            SDL_MapRGB(branco, NULL, 255, 255, 255));
        }

        return histSurface;
}

