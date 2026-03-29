#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL_ttf.h>

typedef struct Window
{
    SDL_Surface *surface;
    SDL_Window *sdlWindow;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
} Window;

typedef struct Button
{
    int width;
    int height;
    SDL_Color color;
    char *texto;
} Button;

typedef struct ButtonRenderer
{
    SDL_Rect sdlRect;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
} ButtonRenderer;

Window *iniciarWindow(SDL_Surface *surface, int max_w, int max_h, const char *nome);
Window *iniciarWindowSecundaria(Window *primaria, SDL_Surface *surface, int width, int height, const char *nome);
ButtonRenderer *iniciarButton(Button *button, SDL_Window *janelaPrima);
void renderizarWindow(Window *window);
void renderizarButton(ButtonRenderer *buttonRenderer);
void destroyWindow(Window *window);

#endif