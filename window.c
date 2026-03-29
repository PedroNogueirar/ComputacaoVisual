#include <stdlib.h>
#include<stdio.h>

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
        primaria
    );

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

void renderizarWindow(Window *window)
{
    SDL_RenderClear(window->renderer);
    SDL_RenderTexture(window->renderer, window->texture, NULL, NULL);
    SDL_RenderPresent(window->renderer);
}

void destroyWindow(Window *window)
{
    SDL_DestroyTexture(window->texture);
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->sdlWindow);
    free(window);
}
