/* Pedro Nogueira Ribeiro - 10324924 */
/* Rafael Santos Lourenço da Silva - 10403588 */
/* Thiago Siqueira - 10390176 */

#include "window.h"
#include <string.h>

/* Janela Secundaria */

SDL_Window *criarJanelaSecundaria(SDL_Window *janelaPrincipal, int largura, int altura) {
    int px, py, pw, ph;
    SDL_GetWindowPosition(janelaPrincipal, &px, &py);
    SDL_GetWindowSize(janelaPrincipal, &pw, &ph);

    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, "Histograma");
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER,  largura);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, altura);
    SDL_SetPointerProperty(props, SDL_PROP_WINDOW_CREATE_PARENT_POINTER, janelaPrincipal);

    SDL_Window *sec = SDL_CreateWindowWithProperties(props);
    SDL_DestroyProperties(props);

    if (sec) {
        /* posiciona ao lado direito da janela principal */
        SDL_SetWindowPosition(sec, px + pw + 8, py);
    }

    return sec;
}

/* Botao */
static void corBotao(BotaoEstado estado,
                     Uint8 *r, Uint8 *g, Uint8 *b) {
    switch (estado) {
        case BTN_HOVER:
            *r = 100; *g = 160; *b = 255; break;   /* azul claro */
        case BTN_PRESSED:
            *r =  20; *g =  80; *b = 180; break;   /* azul escuro */
        default:
            *r =  50; *g = 120; *b = 220; break;   /* azul neutro */
    }
}

void renderizarBotao(SDL_Renderer *renderer, Botao *botao, TTF_Font *font) {
    Uint8 r, g, b;
    corBotao(botao->estado, &r, &g, &b);

    /* Corpo Botao */
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderFillRect(renderer, &botao->rect);

    /* Borda */
    SDL_SetRenderDrawColor(renderer, 200, 220, 255, 255);
    SDL_RenderRect(renderer, &botao->rect);

    /* Texto Centralizado */
    const char *texto = botao->equalizado ? "Ver original" : "Equalizar";
    SDL_Color branco = {255, 255, 255, 255};

    SDL_Surface *surf = TTF_RenderText_Blended(font, texto, 0, branco);
    if (!surf) return;

    float tx = botao->rect.x + (botao->rect.w - surf->w) / 2.0f;
    float ty = botao->rect.y + (botao->rect.h - surf->h) / 2.0f;

    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FRect dst = { tx, ty, (float)surf->w, (float)surf->h };
    SDL_DestroySurface(surf);

    if (!tex) return;
    SDL_RenderTexture(renderer, tex, NULL, &dst);
    SDL_DestroyTexture(tex);
}

int pontoDentroBotao(Botao *botao, float mx, float my) {
    return mx >= botao->rect.x &&
           mx <= botao->rect.x + botao->rect.w &&
           my >= botao->rect.y &&
           my <= botao->rect.y + botao->rect.h;
}
