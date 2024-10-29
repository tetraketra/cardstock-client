#include "toolkit/toolkit.h"
#include "toolkit/time.h"
#include "sdlext/init.h"
#include "sdlext/keys.h"
#include "sdlext/mouse.h"
#include "sdlext/window.h"
#include "sdlext/opengl.h"

// TESTING
#include "client/config.h"
// END TESTING

int main(void) {

    /* SDL setup. */
    SDL_Window* window;
    SDL_GLContext context;
    SDL_Event event;
    SDL_bool quit;
    SDLEXT_Keys sdlext_keys;
    SDLEXT_Mouse sdlext_mouse;
    SDLEXT_Window sdlext_window;
    SDLEXT_OpenGL sdlext_opengl;

    sdlext_init(
        &window,
        &context,
        &event,
        &quit,
        &sdlext_keys,
        &sdlext_mouse,
        &sdlext_window,
        &sdlext_opengl
    );

    /* User config setup. */

    // TESTING START
    client_config_t user_config = {0};
    client_config_overwrite_user();
    client_config_load(&user_config);
    client_config_make_real(
        &user_config,
        &sdlext_window
    );

    // TESTING END

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