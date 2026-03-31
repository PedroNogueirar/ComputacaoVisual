/* Pedro Nogueira Ribeiro - 10324924 */
/* Rafael Santos Lourenço da Silva - 10403588 */
/* Thiago Siqueira - 10390176 */


#include "histograma.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/* Cálculo */

void calcularHistograma(SDL_Surface *surface, Histograma *hist) {
    memset(hist, 0, sizeof(Histograma));

    SDL_Surface *src = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA8888);
    if (!src) return;

    const SDL_PixelFormatDetails *fmt =
        SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA8888);

    int x, y;
    for (y = 0; y < src->h; y++) {
        for (x = 0; x < src->w; x++) {
            Uint8 *p = (Uint8 *)src->pixels + y * src->pitch + x * 4;
            Uint8 r, g, b;
            SDL_GetRGB(*(Uint32 *)p, fmt, NULL, &r, &g, &b);
            hist->freq[r]++;   /* imagem ja esta em cinza: r==g==b */
        }
    }

    hist->total = src->w * src->h;

    /* media */
    double soma = 0.0;
    int i;
    for (i = 0; i < 256; i++)
        soma += (double)i * hist->freq[i];
    hist->media = (float)(soma / hist->total);

    /* desvio padrao */
    double var = 0.0;
    for (i = 0; i < 256; i++) {
        double d = i - hist->media;
        var += d * d * hist->freq[i];
    }
    hist->desvio = (float)sqrt(var / hist->total);

    SDL_DestroySurface(src);
}

/* Classificação */

const char *classificarMedia(float media) {
    if (media > 170) return "clara";
    if (media > 85)  return "media";
    return "escura";
}

const char *classificarContraste(float desvio) {
    if (desvio > 60) return "alto";
    if (desvio > 30) return "medio";
    return "baixo";
}

/* Equalização */

SDL_Surface *equalizarHistograma(SDL_Surface *surface, Histograma *histOut) {
    Histograma hist;
    calcularHistograma(surface, &hist);

    /* Mapeamento CDF */
    int cdf[256] = {0};
    cdf[0] = hist.freq[0];
    int i;
    for (i = 1; i < 256; i++)
        cdf[i] = cdf[i - 1] + hist.freq[i];

    int cdfMin = 0;
    for (i = 0; i < 256; i++) {
        if (cdf[i] > 0) { cdfMin = cdf[i]; break; }
    }

    int total = hist.total;
    Uint8 lut[256];
    for (i = 0; i < 256; i++) {
        int val = (int)(((double)(cdf[i] - cdfMin) / (total - cdfMin)) * 255.0 + 0.5);
        lut[i] = (Uint8)(val < 0 ? 0 : val > 255 ? 255 : val);
    }

    /* Aplica LUT */
    SDL_Surface *src = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA8888);
    SDL_Surface *dst = SDL_CreateSurface(src->w, src->h, SDL_PIXELFORMAT_RGBA8888);
    const SDL_PixelFormatDetails *fmt =
        SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA8888);

    int x, y;
    for (y = 0; y < src->h; y++) {
        for (x = 0; x < src->w; x++) {
            Uint8 *sp = (Uint8 *)src->pixels + y * src->pitch + x * 4;
            Uint8 r, g, b, a;
            SDL_GetRGBA(*(Uint32 *)sp, fmt, NULL, &r, &g, &b, &a);
            Uint8 eq = lut[r];
            Uint8 *dp = (Uint8 *)dst->pixels + y * dst->pitch + x * 4;
            *(Uint32 *)dp = SDL_MapRGBA(fmt, NULL, eq, eq, eq, a);
        }
    }

    SDL_DestroySurface(src);

    if (histOut)
        calcularHistograma(dst, histOut);

    return dst;
}

/* Renderização Histograma */

/* Desenha texto simples com SDL_ttf */
static void desenharTexto(SDL_Renderer *renderer, TTF_Font *font,
                           const char *texto, int x, int y,
                           SDL_Color cor) {
    SDL_Surface *surf = TTF_RenderText_Blended(font, texto, 0, cor);
    if (!surf) return;
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FRect dst = { (float)x, (float)y,
                      (float)surf->w, (float)surf->h };
    SDL_DestroySurface(surf);
    if (!tex) return;
    SDL_RenderTexture(renderer, tex, NULL, &dst);
    SDL_DestroyTexture(tex);
}

void renderizarHistograma(SDL_Renderer *renderer, Histograma *hist,
                          TTF_Font *font, int winW, int winH,
                          int equalizado) {
    /* Fundo */
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    /* Area */
    int margem   = 20;
    int areaX    = margem;
    int areaY    = 40;
    int areaW    = winW - 2 * margem;
    int areaH    = winH - 210;

    SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
    SDL_FRect bgRect = { (float)areaX, (float)areaY, (float)areaW, (float)areaH };
    SDL_RenderFillRect(renderer, &bgRect);

    /* Barras */
    int maxFreq = 1;
    int i;
    for (i = 0; i < 256; i++)
        if (hist->freq[i] > maxFreq) maxFreq = hist->freq[i];

    float barW = (float)areaW / 256.0f;
    for (i = 0; i < 256; i++) {
        float barH = (float)areaH * hist->freq[i] / maxFreq;
        SDL_FRect bar = {
            (float)areaX + i * barW,
            (float)(areaY + areaH) - barH,
            barW < 1.0f ? 1.0f : barW,
            barH
        };
        SDL_SetRenderDrawColor(renderer, (Uint8)i, (Uint8)i, (Uint8)i, 255);
        SDL_RenderFillRect(renderer, &bar);
    }

    /* Titulo */
    SDL_Color branco = {220, 220, 220, 255};
    SDL_Color amarelo = {255, 220, 60, 255};

    const char *titulo = equalizado ? "Histograma (equalizado)" : "Histograma";
    desenharTexto(renderer, font, titulo, margem, 10, branco);

    /* Infos */
    char buf[128];
    int infoY = areaY + areaH + 12;

    snprintf(buf, sizeof(buf), "Media: %.1f  (%s)",
             hist->media, classificarMedia(hist->media));
    desenharTexto(renderer, font, buf, margem, infoY, branco);

    snprintf(buf, sizeof(buf), "Desvio padrao: %.1f  (contraste %s)",
             hist->desvio, classificarContraste(hist->desvio));
    desenharTexto(renderer, font, buf, margem, infoY + 22, branco);

    (void)amarelo;
}
