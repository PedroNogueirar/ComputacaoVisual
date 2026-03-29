#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

typedef struct Window{
    SDL_Surface *surface;
    SDL_Window *sdlWindow;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
} Window;


Window* iniciarWindow(SDL_Surface *surface, int max_w, int max_h, const char *nome);
Window* iniciarWindowSecundaria(Window *primaria, SDL_Surface *surface, int width, int height, const char *nome);
void destroyWindow(Window *window);
void renderizarWindow(Window *window);

#endif