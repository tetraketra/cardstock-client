#ifndef SDLEXT_MOUSE_H
#define SDLEXT_MOUSE_H

#include "sdlext.h"
#include "window.h"

typedef struct SDLEXT_Mouse {
    bool  scroll_inverted;
    float scroll_sensivitiy;
    float scroll_x;
    float scroll_y;

    float x;
    float x_rel;
    bool  x_inverted;
    float y;
    float y_rel;
    bool  y_inverted;
} SDLEXT_Mouse;

extern void sdlext_mouse_update(SDL_Event* event, SDLEXT_Mouse* mouse, SDLEXT_Window* window);

#endif