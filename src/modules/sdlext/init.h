#ifndef SDLEXT_INIT_H
#define SDLEXT_INIT_H

#include <glad/glad.h>

#include "sdlext.h"
#include "keys.h"
#include "mouse.h"
#include "window.h"
#include "opengl.h"

#define WINDOW_INIT_WIDTH 800
#define WINDOW_INIT_HEIGHT 600

void sdlext_init(
    SDL_Window** window, 
    SDL_GLContext* context, 
    SDL_Event* event, 
    SDL_bool* quit, 
    SDLEXT_Keys* sdlext_keys, 
    SDLEXT_Mouse* sdlext_mouse,
    SDLEXT_Window* sdlext_window,
    SDLEXT_OpenGL* sdlext_opengl
);

void sdlext_exit(
    SDL_Window** window, 
    SDL_GLContext* context, 
    SDLEXT_OpenGL* sdlext_opengl
);

#endif