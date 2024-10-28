#include "init.h"

/*
    Initialize SDL and necessary SDLEXT structs.

    @param window Address of the SDL_Window to initialize. You'll have to define this yourself.
    @param context Address of the SDL_GLContext to initialize. (^) Define yourself.
    @param keys Address of the SDLEXT_Keys to initialize. (^) Define yourself.
*/
void sdlext_init(
    SDL_Window** window, 
    SDL_GLContext* context, 
    SDL_Event* event,
    SDL_bool* quit,
    SDLEXT_Keys* sdlext_keys,
    SDLEXT_Mouse* sdlext_mouse,
    SDLEXT_Window* sdlext_window,
    SDLEXT_OpenGL* sdlext_opengl
) {

    /* Setup. */
    atexit(SDL_Quit);
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == SDL_FALSE) {
        ERROR("Could not initialize SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    /* Pre-window GL. */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    /* SDL window. */
    *window = NULL;
    if ((*window=SDL_CreateWindow("!yippie!", WINDOW_INIT_WIDTH, WINDOW_INIT_HEIGHT, SDL_WINDOW_OPENGL)) == NULL) {
        ERROR("Could not initialize SDL_Window: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    /* SDL GL context. */
    *context = (SDL_GLContext){0};
    if ((*context=SDL_GL_CreateContext(*window)) == NULL) {
        ERROR("Could not create SDL_GLContext: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    /* Post-context GL. */
    gladLoadGL();
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glGenBuffers(1, &sdlext_opengl->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sdlext_opengl->vbo);
    sdlext_opengl->shader_programs = hashtable_init(24, NULL, 0.8);

    /* SDL event. */
    *event = (SDL_Event){0};
    if (event->type != 0) {
        ERROR("Failed to initialize SDL_Event.\n");
        exit(EXIT_FAILURE);
    }

    /* SDL quit. */
    *quit = false;
    if (*quit) {
        INFO("If this doesn't work on your computer, seek help.\n");
        exit(EXIT_FAILURE);
    }

    /* SDLEXT keys. */
    *sdlext_keys = (SDLEXT_Keys){0};
    if (sdlext_keys->key_SDL_SCANCODE_0.is_down != false) {
        ERROR("Failed to initialize SDLEXT_Keys.\n");
        exit(EXIT_FAILURE);
    }

    /* SDLEXT mouse. */
    *sdlext_mouse = (SDLEXT_Mouse){.scroll_sensivitiy = 1.0f};
    if (sdlext_mouse->x != 0) {
        ERROR("Failed to initialize SDLEXT_Mouse.\n");
        exit(EXIT_FAILURE);
    }

    /* SDLEXT window. */
    *sdlext_window = (SDLEXT_Window){.window = *window, .height = WINDOW_INIT_HEIGHT, .width = WINDOW_INIT_WIDTH};
    if (sdlext_window->window != *window) {
        ERROR("Failed to initialize SDLEXT_Window.\n");
        exit(EXIT_FAILURE);
    }

}

/*
    Tears down SDL and necessary SDLEXT structs, then exits.

    @param window Address of the SDL_Window to tear down.
    @param context Address of the SDL_GLContext to tear down.
*/
void sdlext_exit(
    SDL_Window** window, 
    SDL_GLContext* context,
    SDLEXT_OpenGL* sdlext_opengl
) {
    hashtable_free(sdlext_opengl->shader_programs, true);

    SDL_GL_DestroyContext(*context);
    SDL_DestroyWindow(*window);
    SDL_Quit();
    exit(EXIT_SUCCESS);
}