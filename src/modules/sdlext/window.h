#ifndef SDLEXT_WINDOW_H
#define SDLEXT_WINDOW_H

#include "sdlext.h"

/* SDL to OpenGL window coordinate converter. e.g. 123.492 to 0.309 in a 400-[wide/high] window. */
#define SDLTOGL_COORD(base, window_dimension) ((float)(base))/((float)(window_dimension))

typedef struct SDLEXT_Window {
    int width;
    int height;
    SDL_Window* window;
} SDLEXT_Window;

extern void sdlext_window_update(SDL_Event* event, SDLEXT_Window* window);

#endif