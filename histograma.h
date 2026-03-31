/* Pedro Nogueira Ribeiro - 10324924 */
/* Rafael Santos Lourenço da Silva - 10403588 */
/* Thiago Siqueira - 10390176 */

#ifndef HISTOGRAMA_H
#define HISTOGRAMA_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h> 

typedef struct {
    int freq[256];
    float media;
    float desvio;
    int total;
} Histograma;

/* Calcula o histograma de uma imagem em escala de cinza */
void calcularHistograma(SDL_Surface *surface, Histograma *hist);

/* Equaliza o histograma e retorna nova surface */
SDL_Surface *equalizarHistograma(SDL_Surface *surface, Histograma *histOut);

/* Retorna classificacao da media: "clara", "media" ou "escura" */
const char *classificarMedia(float media);

/* Retorna classificacao do contraste: "alto", "medio" ou "baixo" */
const char *classificarContraste(float desvio);

/* Renderiza o histograma na janela secundaria */
void renderizarHistograma(SDL_Renderer *renderer, Histograma *hist, TTF_Font *font, int winW, int winH, int equalizado);

#endif
