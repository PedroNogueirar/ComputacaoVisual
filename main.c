#include <stdio.h>
#include <string.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "window.h"
#include "histograma.h"

/* Verifica se a extensao do arquivo e um formato de imagem suportado */
int validarExtensao(const char *caminho)
{
    const char *ext = strrchr(caminho, '.');
    if (!ext)
    {
        printf("Erro: Arquivo sem extensao: %s\n", caminho);
        return 0;
    }

    char extLower[16] = {0};
    int i;
    for (i = 0; ext[i] && i < 15; i++)
        extLower[i] = (char)((ext[i] >= 'A' && ext[i] <= 'Z') ? ext[i] + 32 : ext[i]);

    const char *suportados[] = {".png", ".jpg", ".jpeg", ".bmp", ".gif", ".tga", ".tiff", NULL};
    for (i = 0; suportados[i]; i++)
    {
        if (strcmp(extLower, suportados[i]) == 0)
            return 1;
    }

    printf("Erro: Extensao '%s' nao suportada.\n", extLower);
    printf("Formatos aceitos: PNG, JPG, BMP, GIF, TGA, TIFF\n");
    return 0;
}

/* Carrega a imagem do caminho informado */
SDL_Surface *carregarImagem(const char *caminho)
{
    SDL_IOStream *io = SDL_IOFromFile(caminho, "r");
    if (!io)
    {
        printf("Erro: Arquivo nao encontrado: %s\n", caminho);
        return NULL;
    }
    SDL_CloseIO(io);

    SDL_Surface *surface = IMG_Load(caminho);
    if (!surface)
    {
        printf("Erro: Formato de imagem invalido ou nao suportado: %s\n", caminho);
        printf("SDL_image: %s\n", SDL_GetError());
        return NULL;
    }

    printf("Imagem carregada: %s (%dx%d px)\n", caminho, surface->w, surface->h);
    return surface;
}

/* Verifica se a imagem ja esta em escala de cinza (R == G == B em todos os pixels) */
int estaEmCinza(SDL_Surface *surface)
{
    SDL_Surface *conv = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA8888);
    if (!conv)
        return 0;

    const SDL_PixelFormatDetails *fmt = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA8888);
    int x, y;
    for (y = 0; y < conv->h; y++)
    {
        for (x = 0; x < conv->w; x++)
        {
            Uint8 *p = (Uint8 *)conv->pixels + y * conv->pitch + x * 4;
            Uint8 r, g, b;
            SDL_GetRGB(*(Uint32 *)p, fmt, NULL, &r, &g, &b);
            if (r != g || g != b)
            {
                SDL_DestroySurface(conv);
                return 0;
            }
        }
    }

    SDL_DestroySurface(conv);
    return 1;
}

/* Converte a imagem para escala de cinza usando a formula:
   Y = 0.2125 * R + 0.7154 * G + 0.0721 * B */
SDL_Surface *converterParaCinza(SDL_Surface *surface)
{
    SDL_Surface *src = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA8888);
    if (!src)
    {
        printf("Erro ao converter surface: %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Surface *dst = SDL_CreateSurface(src->w, src->h, SDL_PIXELFORMAT_RGBA8888);
    if (!dst)
    {
        printf("Erro ao criar surface de destino: %s\n", SDL_GetError());
        SDL_DestroySurface(src);
        return NULL;
    }

    const SDL_PixelFormatDetails *fmt = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA8888);
    int x, y;
    for (y = 0; y < src->h; y++)
    {
        for (x = 0; x < src->w; x++)
        {
            Uint8 *sp = (Uint8 *)src->pixels + y * src->pitch + x * 4;
            Uint8 r, g, b, a;
            SDL_GetRGBA(*(Uint32 *)sp, fmt, NULL, &r, &g, &b, &a);

            Uint8 gray = (Uint8)(0.2125 * r + 0.7154 * g + 0.0721 * b);

            Uint8 *dp = (Uint8 *)dst->pixels + y * dst->pitch + x * 4;
            *(Uint32 *)dp = SDL_MapRGBA(fmt, NULL, gray, gray, gray, a);
        }
    }

    SDL_DestroySurface(src);
    return dst;
}

/* Analisa a imagem e retorna sempre uma surface em escala de cinza */
SDL_Surface *analisarEConverterCinza(SDL_Surface *surface)
{
    if (estaEmCinza(surface))
    {
        printf("Imagem ja esta em escala de cinza. Nenhuma conversao necessaria.\n");
        return SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA8888);
    }

    printf("Imagem colorida detectada. Convertendo para escala de cinza...\n");
    SDL_Surface *cinza = converterParaCinza(surface);
    if (cinza)
        printf("Conversao concluida com sucesso.\n");
    return cinza;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Uso: %s <caminho_da_imagem>\n", argv[0]);
        return 1;
    }

    if (!validarExtensao(argv[1]))
        return 1;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    /* 1. Carrega a imagem */
    SDL_Surface *surface = carregarImagem(argv[1]);
    if (!surface)
    {
        SDL_Quit();
        return 1;
    }

    /* 2. Analisa e converte para escala de cinza */
    SDL_Surface *cinza = analisarEConverterCinza(surface);
    SDL_DestroySurface(surface);
    if (!cinza)
    {
        SDL_Quit();
        return 1;
    }

    /* Cria janela com o tamanho da imagem (limitado a 1280x720) */
    int winW = cinza->w < 1280 ? cinza->w : 1280;
    int winH = cinza->h < 720 ? cinza->h : 720;

    Window *janelaPrincipal = iniciarWindow(cinza, winW, winH, "janela principal");
    if (janelaPrincipal == NULL)
    {
        printf("ERRO ao criar uma janela");
        return 1;
    }

    HIST *hist;
    plotarHistograma(cinza, hist);

    int histWidth = winW / 4;
    int histHeight = winW / 4;
    SDL_Surface *histSurface = criarImagemHistograma(hist, histWidth, histHeight);

    Window *janelaHist = iniciarWindowSecundaria(
        janelaPrincipal,
        histSurface,
        histWidth,
        histHeight,
        "janela histograma");

    SDL_Event event;
    int running = 1;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = 0;
            if (event.type == SDL_EVENT_KEY_DOWN &&
                (event.key.key == SDLK_ESCAPE || event.key.key == SDLK_Q))
                running = 0;
        }

        renderizarWindow(janelaPrincipal);
        renderizarWindow(janelaHist);
        SDL_Delay(16);
    }

    SDL_Quit();

    return 0;
}