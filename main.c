/* Pedro Nogueira Ribeiro - 10324924 */
/* Rafael Santos Lourenço da Silva - 10403588 */
/* Thiago Siqueira - 10390176 */

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "histograma.h"
#include "window.h"


/*  Validacao de extensao */
static int validarExtensao(const char *caminho) {
    const char *ext = strrchr(caminho, '.');
    if (!ext) { printf("Erro: arquivo sem extensao.\n"); return 0; }

    char low[16] = {0};
    int i;
    for (i = 0; ext[i] && i < 15; i++)
        low[i] = (char)(ext[i] >= 'A' && ext[i] <= 'Z' ? ext[i]+32 : ext[i]);

    const char *ok[] = {".png",".jpg",".jpeg",".bmp",".gif",".tga",".tiff",NULL};
    for (i = 0; ok[i]; i++)
        if (strcmp(low, ok[i]) == 0) return 1;

    printf("Erro: extensao '%s' nao suportada.\n", low);
    return 0;
}

/* Conversao para escala de cinza */
static int estaEmCinza(SDL_Surface *s) {
    SDL_Surface *c = SDL_ConvertSurface(s, SDL_PIXELFORMAT_RGBA8888);
    if (!c) return 0;
    const SDL_PixelFormatDetails *fmt =
        SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA8888);
    int x, y, ok = 1;
    for (y = 0; y < c->h && ok; y++)
        for (x = 0; x < c->w && ok; x++) {
            Uint8 *p = (Uint8*)c->pixels + y*c->pitch + x*4;
            Uint8 r,g,b;
            SDL_GetRGB(*(Uint32*)p, fmt, NULL, &r, &g, &b);
            if (r!=g || g!=b) ok=0;
        }
    SDL_DestroySurface(c);
    return ok;
}

static SDL_Surface *converterParaCinza(SDL_Surface *s) {
    SDL_Surface *src = SDL_ConvertSurface(s, SDL_PIXELFORMAT_RGBA8888);
    if (!src) return NULL;
    SDL_Surface *dst = SDL_CreateSurface(src->w, src->h,
                                         SDL_PIXELFORMAT_RGBA8888);
    if (!dst) { SDL_DestroySurface(src); return NULL; }

    const SDL_PixelFormatDetails *fmt =
        SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA8888);
    int x, y;
    for (y = 0; y < src->h; y++)
        for (x = 0; x < src->w; x++) {
            Uint8 *sp = (Uint8*)src->pixels + y*src->pitch + x*4;
            Uint8 r,g,b,a;
            SDL_GetRGBA(*(Uint32*)sp, fmt, NULL, &r, &g, &b, &a);
            Uint8 gray = (Uint8)(0.2125*r + 0.7154*g + 0.0721*b);
            Uint8 *dp = (Uint8*)dst->pixels + y*dst->pitch + x*4;
            *(Uint32*)dp = SDL_MapRGBA(fmt, NULL, gray, gray, gray, a);
        }
    SDL_DestroySurface(src);
    return dst;
}


/* Salvar imagem */

static void salvarImagem(SDL_Surface *surface) {
    if (IMG_SavePNG(surface, "output_image.png"))
        printf("Imagem salva em output_image.png\n");
    else
        printf("Erro ao salvar imagem: %s\n", SDL_GetError());
}

/* main */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <caminho_da_imagem>\n", argv[0]);
        return 1;
    }
    if (!validarExtensao(argv[1])) return 1;

    /* Inicializacao */
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init: %s\n", SDL_GetError()); return 1;
    }
    if (!TTF_Init()) {
        printf("TTF_Init: %s\n", SDL_GetError()); SDL_Quit(); return 1;
    }

    /* Carrega Imagem */
    SDL_Surface *imgOriginal = IMG_Load(argv[1]);
    if (!imgOriginal) {
        printf("Erro ao carregar imagem: %s\n", SDL_GetError());
        TTF_Quit(); SDL_Quit(); return 1;
    }
    printf("Imagem: %s (%dx%d)\n", argv[1], imgOriginal->w, imgOriginal->h);

    /* Converte */
    SDL_Surface *cinzaBase;
    if (estaEmCinza(imgOriginal)) {
        printf("Imagem ja em escala de cinza.\n");
        cinzaBase = SDL_ConvertSurface(imgOriginal, SDL_PIXELFORMAT_RGBA8888);
    } else {
        printf("Convertendo para escala de cinza...\n");
        cinzaBase = converterParaCinza(imgOriginal);
    }
    SDL_DestroySurface(imgOriginal);
    if (!cinzaBase) { TTF_Quit(); SDL_Quit(); return 1; }

    /* Surface equalizada exibida*/
    SDL_Surface *cinzaAtual = SDL_ConvertSurface(cinzaBase,
                                                  SDL_PIXELFORMAT_RGBA8888);

    /* Janela principal */
    int winW = cinzaBase->w < 1280 ? cinzaBase->w : 1280;
    int winH = cinzaBase->h < 720  ? cinzaBase->h : 720;

    SDL_Window *janelaPrincipal = SDL_CreateWindow(argv[1], winW, winH, 0);
    if (!janelaPrincipal) {
        printf("Erro janela principal: %s\n", SDL_GetError());
        goto cleanup_surfaces;
    }
    SDL_SetWindowPosition(janelaPrincipal,
                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    SDL_Renderer *rendPrincipal = SDL_CreateRenderer(janelaPrincipal, NULL);
    if (!rendPrincipal) {
        printf("Erro renderer principal: %s\n", SDL_GetError());
        goto cleanup_win_principal;
    }

    /* Janela secundaria */
    int secW = 420, secH = 520;
    SDL_Window *janelaSecundaria =
        criarJanelaSecundaria(janelaPrincipal, secW, secH);
    if (!janelaSecundaria) {
        printf("Erro janela secundaria: %s\n", SDL_GetError());
        goto cleanup_rend_principal;
    }

    SDL_Renderer *rendSecundario =
        SDL_CreateRenderer(janelaSecundaria, NULL);
    if (!rendSecundario) {
        printf("Erro renderer secundario: %s\n", SDL_GetError());
        goto cleanup_win_secundaria;
    }

    /* Fonte */
    TTF_Font *font = TTF_OpenFont("C:/Windows/Fonts/Arial.ttf", 16);
    if (!font) {
        font = TTF_OpenFont("C:/Windows/Fonts/segoeui.ttf", 16);
    }
    if (!font) {
        printf("Aviso: fonte nao encontrada, texto nao sera exibido.\n");
    }

    /* Botao */
    Botao botao;
    botao.estado    = BTN_NORMAL;
    botao.equalizado = 0;
    botao.rect.w    = 180.0f;
    botao.rect.h    =  44.0f;
    botao.rect.x    = (secW - botao.rect.w) / 2.0f;
    botao.rect.y    = (float)(secH - 60);

    /* Primeiro Histograma */
    Histograma hist;
    calcularHistograma(cinzaAtual, &hist);

    SDL_Texture *texturaPrincipal =
        SDL_CreateTextureFromSurface(rendPrincipal, cinzaAtual);

    SDL_WindowID idPrincipal   = SDL_GetWindowID(janelaPrincipal);
    SDL_WindowID idSecundaria  = SDL_GetWindowID(janelaSecundaria);


    /* Loop principal */

    int rodando = 1;
    SDL_Event ev;

    while (rodando) {
        while (SDL_PollEvent(&ev)) {

            /* Fechar encerra */
            if (ev.type == SDL_EVENT_QUIT) {
                rodando = 0;
            }

            /* Teclas janela principal */
            if (ev.type == SDL_EVENT_KEY_DOWN &&
                ev.key.windowID == idPrincipal) {
                if (ev.key.key == SDLK_ESCAPE || ev.key.key == SDLK_Q)
                    rodando = 0;

                /* Salvar com S */
                if (ev.key.key == SDLK_S)
                    salvarImagem(cinzaAtual);
            }

            /* Fechar janela */
            if (ev.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
                rodando = 0;

            /* Eventos mouse */
            if (ev.type == SDL_EVENT_MOUSE_MOTION &&
                ev.motion.windowID == idSecundaria) {
                float mx = ev.motion.x, my = ev.motion.y;
                if (botao.estado != BTN_PRESSED) {
                    botao.estado = pontoDentroBotao(&botao, mx, my)
                                   ? BTN_HOVER : BTN_NORMAL;
                }
            }

            if (ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
                ev.button.windowID == idSecundaria &&
                ev.button.button == SDL_BUTTON_LEFT) {
                float mx = ev.button.x, my = ev.button.y;
                if (pontoDentroBotao(&botao, mx, my))
                    botao.estado = BTN_PRESSED;
            }

            if (ev.type == SDL_EVENT_MOUSE_BUTTON_UP &&
                ev.button.windowID == idSecundaria &&
                ev.button.button == SDL_BUTTON_LEFT) {
                float mx = ev.button.x, my = ev.button.y;
                if (botao.estado == BTN_PRESSED &&
                    pontoDentroBotao(&botao, mx, my)) {

                    /* Alterna tipo */
                    botao.equalizado = !botao.equalizado;

                    SDL_DestroySurface(cinzaAtual);
                    if (botao.equalizado) {
                        cinzaAtual = equalizarHistograma(cinzaBase, &hist);
                    } else {
                        cinzaAtual = SDL_ConvertSurface(
                            cinzaBase, SDL_PIXELFORMAT_RGBA8888);
                        calcularHistograma(cinzaAtual, &hist);
                    }

                    /* Atualiza textura */
                    SDL_DestroyTexture(texturaPrincipal);
                    texturaPrincipal =
                        SDL_CreateTextureFromSurface(rendPrincipal, cinzaAtual);
                }
                botao.estado = pontoDentroBotao(&botao, mx, my)
                               ? BTN_HOVER : BTN_NORMAL;
            }

            if (ev.type == SDL_EVENT_WINDOW_MOUSE_LEAVE &&
                ev.window.windowID == idSecundaria) {
                if (botao.estado != BTN_PRESSED)
                    botao.estado = BTN_NORMAL;
            }
        }

        /* Renderiza Principal */
        SDL_SetRenderDrawColor(rendPrincipal, 0, 0, 0, 255);
        SDL_RenderClear(rendPrincipal);
        SDL_RenderTexture(rendPrincipal, texturaPrincipal, NULL, NULL);
        SDL_RenderPresent(rendPrincipal);

        /* Renderiza Secundária */
        renderizarHistograma(rendSecundario, &hist, font,
                             secW, secH, botao.equalizado);
        renderizarBotao(rendSecundario, &botao, font);
        SDL_RenderPresent(rendSecundario);

        SDL_Delay(16);
    }


    /* Limpeza */
    SDL_DestroyTexture(texturaPrincipal);
    if (font) TTF_CloseFont(font);
    SDL_DestroyRenderer(rendSecundario); cleanup_win_secundaria:
    SDL_DestroyWindow(janelaSecundaria); cleanup_rend_principal:
    SDL_DestroyRenderer(rendPrincipal); cleanup_win_principal:
    SDL_DestroyWindow(janelaPrincipal); cleanup_surfaces:
    SDL_DestroySurface(cinzaAtual);
    SDL_DestroySurface(cinzaBase);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
