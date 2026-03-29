#include <stdlib.h>
#include <stdio.h>

#include "histograma.h"

void plotarHistograma(SDL_Surface *surface, HIST **hist)
{
    *hist = malloc(sizeof(HIST));
    (*hist)->h = malloc(sizeof(int) * 256);
    memset((*hist)->h, 0, sizeof(int) * 256);
    for (int y = 0; y < surface->h; y++)
    {
        Uint8 *row = (Uint8 *)surface->pixels + y * surface->pitch;

        for (int x = 0; x < surface->w; x++)
        {
            Uint8 *pixel = row + x;
            (*hist)->h[*pixel]++;
        }
    }
}

void equalizarHistograma(HIST *hist)
{
    if (!hist || !hist->h)
        return;

    int total = 0;
    for (int i = 0; i < 256; i++)
        total += hist->h[i]; 

    if (total == 0)
        return; 
  
    float cdf[256] = {0};
    cdf[0] = (float)hist->h[0] / total;
    for (int i = 1; i < 256; i++)
        cdf[i] = cdf[i - 1] + (float)hist->h[i] / total;

    int h_equalizado[256];
    for (int i = 0; i < 256; i++)
        h_equalizado[i] = (int)(255 * cdf[i] + 0.5f);

    int novo_hist[256] = {0};
    for (int i = 0; i < 256; i++)
    {
        int valor = h_equalizado[i];
        if (valor < 0)
            valor = 0;
        if (valor > 255)
            valor = 255;
        novo_hist[valor] += hist->h[i];
    }

    for (int i = 0; i < 256; i++)
        hist->h[i] = novo_hist[i];
}

SDL_Surface *criarImagemHistograma(HIST *hist, int width, int height)
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
