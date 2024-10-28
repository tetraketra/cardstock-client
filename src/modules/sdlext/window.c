#include "window.h"

/*
    Updates the `sdlext::window` tracker struct given the current `SDL_Event`.

    @param event Address of the `SDL_Event` struct to update with.
    @param keys Address of the `SDLEXT_Window` struct to update. 

    @note Self-contained switch-case on `event->type`. 
    @note Intended for use inside your `SDL_PollEvent`/`SDL_WaitEvent` loop.
    @note ________________________________________________________________
    @note `while (SDL_PollEvent(&event) || !quit) { sdlext_window_update(&event, &keys); ...`
*/
inline void sdlext_window_update(SDL_Event* event, SDLEXT_Window* window) {
    switch (event->type) {
        case SDL_EVENT_WINDOW_RESIZED:
            window->width = event->window.data1;
            window->height = event->window.data2;
            break;
    }
}