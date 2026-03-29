#include <stdlib.h>
#include <stdio.h>

#include "window.h"

Window *iniciarWindow(SDL_Surface *surface, int max_w, int max_h, const char *nome)
{
    if (surface == NULL)
        return NULL;

    SDL_Window *sdlWwindow = SDL_CreateWindow(nome, max_w, max_h, 0);
    if (sdlWwindow == NULL)
        return NULL;

    SDL_Renderer *renderer = SDL_CreateRenderer(sdlWwindow, NULL);
    if (renderer == NULL)
        return NULL;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
        return NULL;

    Window *window = malloc(sizeof(Window));
    window->surface = surface;
    window->renderer = renderer;
    window->sdlWindow = sdlWwindow;

    return window;
}

Window *iniciarWindowSecundaria(Window *primaria, SDL_Surface *surface, int width, int height, const char *nome)
{
    SDL_PropertiesID props = SDL_CreateProperties();

    SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, nome);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, width);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, height);

    SDL_SetPointerProperty(
        props,
        SDL_PROP_WINDOW_CREATE_PARENT_POINTER,
        primaria->sdlWindow);

    SDL_Window *secundaria = SDL_CreateWindowWithProperties(props);
    if (secundaria == NULL)
        return NULL;

    SDL_Renderer *renderer = SDL_CreateRenderer(secundaria, NULL);
    if (renderer == NULL)
        return NULL;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
        return NULL;

    Window *window = malloc(sizeof(Window));
    window->surface = surface;
    window->renderer = renderer;
    window->sdlWindow = secundaria;
    window->texture = texture;

    return window;
}

ButtonRenderer *iniciarButton(Button *button, SDL_Window *janelaPrima)
{
    SDL_PropertiesID props = SDL_CreateProperties();

    SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, button->texto);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, button->width);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, button->height);

    SDL_SetPointerProperty(
        props,
        SDL_PROP_WINDOW_CREATE_PARENT_POINTER,
        janelaPrima);

    SDL_Window *buttonJanela = SDL_CreateWindowWithProperties(props);
    SDL_Renderer *renderer = SDL_CreateRenderer(buttonJanela, NULL);

    SDL_Rect rect = {0, 0, button->width, button->height};
    TTF_Font *font = TTF_OpenFont("C:/Windows/Fonts/Arial.ttf", 24);

    SDL_Color preto = {0, 0, 0, 255};
    SDL_Surface *textSurface = TTF_RenderUTF8_Blended(font, "Clique Aqui!", preto);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    textRect.x = (button->width - textRect.w) / 2;
    textRect.y = (button->height - textRect.h) / 2;

    SDL_FreeSurface(textSurface);

    ButtonRenderer *buttonRenderer = malloc(sizeof(ButtonRenderer));
    buttonRenderer->sdlRect = textRect;
    buttonRenderer->renderer = renderer;
    buttonRenderer->texture = textTexture;

    return buttonRenderer;
}

void renderizarWindow(Window *window)
{
    SDL_RenderClear(window->renderer);
    SDL_RenderTexture(window->renderer, window->texture, NULL, NULL);
    SDL_RenderPresent(window->renderer);
}

void renderizarButton(ButtonRenderer *buttonRenderer){
        SDL_SetRenderDrawColor(buttonRenderer->renderer, 0, 255, 0, 255); // verde
        SDL_RenderClear(buttonRenderer->renderer);
        SDL_RenderCopy(buttonRenderer->renderer, buttonRenderer->texture, NULL, &buttonRenderer->sdlRect);
        SDL_RenderPresent(buttonRenderer->renderer);
}

void destroyWindow(Window *window)
{
    SDL_DestroyTexture(window->texture);
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->sdlWindow);
    free(window);
}
