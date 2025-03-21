#include "toolkit/toolkit.h"
#include "toolkit/time.h"
#include "sdlext/init.h"
#include "sdlext/keys.h"
#include "sdlext/keychord.h"
#include "sdlext/mouse.h"
#include "sdlext/window.h"
#include "sdlext/opengl.h"
#include "client/config.h"

int main(void) {

    /* SDL setup. */
    SDL_Window* window;
    SDL_GLContext context;
    SDL_Event event;
    SDLEXT_Keys sdlext_keys;
    SDLEXT_Mouse sdlext_mouse;
    SDLEXT_Window sdlext_window;
    SDLEXT_OpenGL sdlext_opengl;
    bool quit;

    sdlext_init(
        &window,
        &context,
        &event,
        &sdlext_keys,
        &sdlext_mouse,
        &sdlext_window,
        &sdlext_opengl,
        &quit
    );

    /* Client setup. */
    CACL_Config cacl_config = {.window.title = "Cardstock Client"};

    cacl_config_load_and_init(
        &cacl_config, 
        &sdlext_window
    );

    /* Event loop. */
    while (!quit) {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        while (SDL_PollEvent(&event)) {
            sdlext_window_update(&event, &sdlext_window);
            sdlext_keys_update(&event, &sdlext_keys);
            sdlext_mouse_update(&event, &sdlext_mouse, &sdlext_window);

            switch (event.type) {
                case SDL_EVENT_QUIT:
                    INFO("User quit the app via SDL_EVENT_QUIT.\n");
                    quit = true;
                    break;
            }
        }

        SDL_GL_SwapWindow(window);
    }

    sdlext_exit(
        &window, 
        &context, 
        &sdlext_opengl
    );
}