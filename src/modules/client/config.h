#ifndef CLIENT_CONFIG_H
#define CLIENT_CONFIG_H

#include "toolkit/toolkit.h"
#include "toolkit/exepath.h"
#include "sdlext/window.h"
#include "external/cwalk.h"

/*
    === INFO ===
    1. User configuration manager.
    2. If the user can change it in a settings menu, it belongs here.
    3. `cacl_config_load` then `cacl_config_make_real`.

*/

typedef struct CACL_Config {
    struct {
        char title[256];
    } window;
} CACL_Config; 

extern bool cacl_config_save(CACL_Config* save);
extern bool cacl_config_load(CACL_Config* load);
extern bool cacl_config_overwrite_default(void);
extern bool cacl_config_overwrite_user(void);

extern void cacl_config_init(CACL_Config* config, SDLEXT_Window* sdlext_window);

extern void cacl_config_make_real(CACL_Config* config, SDLEXT_Window* sdlext_window);
extern void cacl_config_set_window_title(CACL_Config* config, SDLEXT_Window* sdlext_window, const char* title, size_t title_len);

#endif