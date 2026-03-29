#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

/* Estados do botao */
typedef enum {
    BTN_NORMAL = 0,
    BTN_HOVER,
    BTN_PRESSED
} BotaoEstado;

/* Botao desenhado com primitivas SDL */
typedef struct {
    SDL_FRect rect;       /* posicao e tamanho dentro da janela secundaria */
    BotaoEstado estado;
    int equalizado;       /* 0 = mostra "Equalizar", 1 = mostra "Ver original" */
} Botao;

/* Cria e posiciona a janela secundaria ao lado da janela principal */
SDL_Window   *criarJanelaSecundaria(SDL_Window *janelaPrincipal,
                                    int largura, int altura);

/* Renderiza o botao (deve ser chamado apos renderizarHistograma) */
void renderizarBotao(SDL_Renderer *renderer, Botao *botao, TTF_Font *font);

/* Testa se o ponto (mx, my) esta sobre o botao */
int pontoDentroBotao(Botao *botao, float mx, float my);

#endif /* WINDOW_H */