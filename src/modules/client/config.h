#ifndef CLIENT_CONFIG_H
#define CLIENT_CONFIG_H

#include "toolkit/toolkit.h"
#include "toolkit/exepath.h"

#include "sdlext/window.h"

#include "external/cwalk.h"

typedef struct client_config_t {
    struct {
        char title[256];
    } window;
} client_config_t; 

extern bool client_config_save(client_config_t* save);
extern bool client_config_load(client_config_t* load);
extern bool client_config_overwrite_default(void);
extern bool client_config_overwrite_user(void);

extern void client_config_make_real(client_config_t* config, SDLEXT_Window* sdlext_window);
extern void client_config_set_window_title(client_config_t* config, SDLEXT_Window* sdlext_window, const char* title, size_t title_len);

#endif