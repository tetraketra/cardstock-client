#include "keychord.h"

/*
    Updates a user-defined `SDLEXT_KeyChord` given its referenced keys.
    This can be used for *literally any keybind*. It doesn't reat
    modifier keys as anything special. Those'd just be part of the chord.

    @param chord Address of the `SDLEXT_KeyChord` to update.

    @note You'll want to call this for every `SDLEXT_KeyChord` you
    have. To optimze, only call if `event->type` is `SDL_EVENT_KEY_DOWN` 
    or `SDL_EVENT_KEY_UP` (down=768, up=769).
*/
inline void sdlext_keychord_update(SDLEXT_KeyChord* chord) {
    /* Any up. */
    for (size_t i = 0; i < chord->refkeys_n; i++) {
        if (!chord->refkeys[i]->is_down) {
            chord->is_down = false;
            chord->last_up = timestamp_get(true);
            return; /* done */ /* at least one up; chord up */
        }
    }

    /* All down. */
    chord->is_down = true;
    chord->last_down = timestamp_get(true);
}