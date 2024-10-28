#include "keys.h"

/*
    Updates the `sdlext::keys` tracker struct given the current `SDL_Event`.

    @param event Address of the `SDL_Event` struct to update with.
    @param keys Address of the `SDLEXT_Keys` struct to update. 

    @note Self-contained switch-case on `event->type`. 
    @note Intended for use inside your `SDL_PollEvent`/`SDL_WaitEvent` loop.
    @note ________________________________________________________________
    @note `while (SDL_PollEvent(&event) || !quit) { sdlext_keys_update(&event, &keys); ...`
*/
inline void sdlext_keys_update(SDL_Event* event, SDLEXT_Keys* keys) {
    switch (event->type) {
        /* Keyboard input. */
        case SDL_EVENT_KEY_DOWN: /* =768 */
        case SDL_EVENT_KEY_UP:   /* =769 */ /* no gap, safe fallthrough! */
            if (event->key.repeat) break;
            switch (event->key.scancode) {
                #define KEY_VAR(sdl_scancode) keys->SDL_SCANCODE_TO_SDLEXT_KEY(sdl_scancode)
                #define X(sdl_scancode) \
                case sdl_scancode: \
                    KEY_VAR(sdl_scancode).is_down = ( event->type == SDL_EVENT_KEY_DOWN ); \
                    event->type == SDL_EVENT_KEY_DOWN \
                        ? ( KEY_VAR(sdl_scancode).last_down = timestamp_get(true) ) \
                        : ( KEY_VAR(sdl_scancode).last_up   = timestamp_get(true) ); \
                    break;

                X_ALL_SDL_SCANCODES

                #undef X
                #undef KEY_VAR
            }
            break;
    }
}