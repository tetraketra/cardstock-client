#ifndef SDLEXT_KEYCHORD_H
#define SDLEXT_KEYCHORD_H

#include "keys.h"
#include "external/vanarg.h"

#ifndef SDLEXT_MAX_KEYS_PER_COMBO
#define SDLEXT_MAX_KEYS_PER_COMBO 16
#endif

typedef struct SDLEXT_KeyChord {
    timestamp_t last_down; /* From when key combo was entirely pressed. */
    timestamp_t last_up; /* From when the key combo was broken. */
    bool is_down;

    SDLEXT_Key* refkeys[SDLEXT_MAX_KEYS_PER_COMBO]; /* Addresses of keys to reference when saying if this keycombo is up or down. */
    size_t      refkeys_n;
} SDLEXT_KeyChord;

#define SDLEXT_KeyChord_from_SDLEXT_Key_addresses(...) \
    (SDLEXT_KeyChord){.refkeys = {__VA_ARGS__}, .refkeys_n = PP_NARG(__VA_ARGS__)}

extern void sdlext_keychord_update(SDLEXT_KeyChord* chord);

#endif