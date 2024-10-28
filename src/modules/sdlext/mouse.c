#include "sdlext/mouse.h"

/*
    Updates the `sdlext::mouse` tracker struct given the current `SDL_Event`.

    @param event Address of the `SDL_Event` struct to update with.
    @param keys Address of the `SDLEXT_Mouse` struct to update. 

    @note Self-contained switch-case on `event->type`. 
    @note Intended for use inside your `SDL_PollEvent`/`SDL_WaitEvent` loop.
    @note ________________________________________________________________
    @note `while (SDL_PollEvent(&event) || !quit) { sdlext_mouse_update(&event, &keys); ...`
*/
inline void sdlext_mouse_update(SDL_Event* event, SDLEXT_Mouse* mouse, SDLEXT_Window* window) {
    switch (event->type) {
        case SDL_EVENT_MOUSE_MOTION:
            mouse->x = SDLTOGL_COORD(event->motion.x * (mouse->x_inverted ? -1 : 1), window->width);
            mouse->x_rel = SDLTOGL_COORD(event->motion.xrel * (mouse->x_inverted ? -1 : 1), window->width);
            mouse->y = SDLTOGL_COORD(event->motion.y * (mouse->y_inverted ? -1 : 1), window->height);
            mouse->y_rel = SDLTOGL_COORD(event->motion.yrel * (mouse->y_inverted ? -1 : 1), window->height);
            break;
        
        case SDL_EVENT_MOUSE_WHEEL:
            mouse->scroll_x = event->wheel.x * (mouse->scroll_inverted ? -1 : 1);
            mouse->scroll_y = event->wheel.y * (mouse->scroll_inverted ? -1 : 1);
            break;
    }
}