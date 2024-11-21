#include "config.h"

typedef struct path_t {
    char path[PATH_MAX];
    size_t path_len;
} path_t;

/*
    Gets a file relative to the program's directory.
    I trust you (me) to use `cwk_get_style` and not f[]k this up.

    @param `relative_file_path`: The path to join to the program's path.

    @internal
*/
static path_t _get_relative_file(const char* relative_file_path) {
    char exe_path[PATH_MAX] = {0};
    exepath_get(exe_path, PATH_MAX);
    exe_path[PATH_MAX-1] = '\0';

    path_t config_path = {0};
    config_path.path_len = cwk_path_join(
        (const char*) exe_path, 
        relative_file_path, 
        config_path.path, 
        PATH_MAX
    );

    return config_path;
}

/*
    Copies the relative file at `src_relative_path` 
    to the relative file `dest_relative_path`.

    @param `src_relative_path`: Relative file path to copy from.
    @param `dest_relative_path`: Relative file path to copy to.

    @returns `true` if the copy was successful, `false` otherwise.

    @internal
*/
static bool _copy_rename_relative_file(const char* src_relative_path, const char* dest_relative_path) {
    path_t src_abs_path = _get_relative_file(src_relative_path);
    path_t dest_abs_path = _get_relative_file(dest_relative_path);

    FILE* src = fopen((const char*) src_abs_path.path, "rb");
    if (src == NULL) {
        ERROR("Failed to open `%s`.\n", src_relative_path);
        exit(EXIT_FAILURE);
    }

    FILE* dest = fopen((const char*) dest_abs_path.path, "wb");
    if (dest == NULL) {
        ERROR("Failed to open `%s`.\n", dest_relative_path);
        exit(EXIT_FAILURE);
    }

    CACL_Config temp = {0};

    size_t read = fread(&temp, sizeof(CACL_Config), 1, src);
    if (read != 1) {
        ERROR("Failed to load `%s`.\n", src_relative_path);
        fclose(src);
        return false;
    }

    size_t written = fwrite(&temp, sizeof(CACL_Config), 1, dest);
    if (written != 1) {
        ERROR("Failed to save `%s`.\n", dest_relative_path);
        fclose(dest);
        return false;
    }

    fclose(src);
    fclose(dest);

    return true;
}

/* 
    Saves the current `user.config` 
    as `default.config` for future use. 

    @returns `true` if the transfer was successful, `false` otherwise.
*/
bool cacl_config_overwrite_default(void) {
    return _copy_rename_relative_file("user.config", "default.config");
}

/* 
    Loads the current `default.config` as `user.config`.
    Does *NOT* call `cacl_config_load` on the new 
    `user.config`. Do that yourself!

    @returns `true` if the transfer was successful, `false` otherwise.
*/
bool cacl_config_overwrite_user(void) {
    return _copy_rename_relative_file("default.config", "user.config");
}

/*
    Save the client's config to `user.config`.

    @param `save`: The config to save.
    @returns `true` if saved, `false` if not.
*/
bool cacl_config_save(CACL_Config* save) {
    RUNTIME_ASSERT(save != NULL);

    path_t config_path = _get_relative_file("user.config");

    FILE* file = fopen((const char*) config_path.path, "wb");
    if (file == NULL) {
        ERROR("Failed to open `user.config`.\n");
        exit(EXIT_FAILURE);
    }

    size_t written = fwrite(save, sizeof(CACL_Config), 1, file);
    fclose(file);

    if (written != 1) {
        ERROR("Failed to save `user.config`.\n");
        return false;
    } else {
        return true;
    }
}

/*
    Loads the client's config from `user.config`.

    @param `load`: The config to load to.
    
    @returns `true` if loaded, `false` if not.

    @note Only updates `CACL_Config`. You 
    *MUST* call `cacl_config_make_real` to 
    see these changes actually impact the app.
*/
bool cacl_config_load(CACL_Config* load) {
    RUNTIME_ASSERT(load != NULL);

    path_t config_path = _get_relative_file("user.config");

    FILE* file = fopen((const char*) config_path.path, "rb");
    if (file == NULL) {
        ERROR("Failed to open `user.config`.\n");
        exit(EXIT_FAILURE);
    }

    *load = (CACL_Config){0}; /* Just for safety. */
    size_t read = fread(load, sizeof(CACL_Config), 1, file);
    fclose(file);
    
    if (read != 1) {
        ERROR("Failed to load `user.config`.\n");
        return false;
    } else {
        return true;
    }
};

/*
    Loads `user.config` and makes all relevant funcs real.
    Litearlly just `cacl_config_load` into `cacl_config_make_real`.
    Useful for simplifying client startup.

    @note For documentation, see `cacl_config_load` and `cacl_config_make_real`.
*/
void cacl_config_load_and_init(CACL_Config* config, SDLEXT_Window* sdlext_window) {
    cacl_config_load(config);
    cacl_config_make_real(
        config,
        sdlext_window
    );
}

/*
    Ideally your `CACL_Config` stays in sync with what your app is doing,
    but sometimes that isn't the case (such as when loading settings on startup).

    This function updates the application with the client configs (set by the user).

    @param `config`: The `CACL_Config` to update with.
    @param `sdlext_window`: The `SDLEXT_Window` to update.
*/
void cacl_config_make_real(CACL_Config* config, SDLEXT_Window* sdlext_window) {
    cacl_config_set_window_title(config, sdlext_window, NULL, 0);
    // TODO FIXME: Add new calls as settings expand.
}

/*
    Writes up to 255 characters of `title` to `config.window.title`,
    then changes the title of the SDL window (if non-`NULL`).
    Both halves of this behavior are skippable via `NULL`s.

    @param `config`: The config to use.
    @param `sdlext_window`:  The `SDLEXT_Window` to change the title of. `NULL` if behavior not wanted.
    @param `title`: The new title to set. `NULL` if just updating `sdlext_window` to match `config`.
    @param `title_len`: The length of the new title.
*/
void cacl_config_set_window_title(CACL_Config* config, SDLEXT_Window* sdlext_window, const char* title, size_t title_len) {
    RUNTIME_ASSERT(config != NULL);
    
    if (title != NULL) {
        memset(config->window.title, 0, 256);
        strncpy(config->window.title, title, MIN(title_len, (size_t)255));
    }
    
    if (sdlext_window != NULL) {
        SDL_bool set = SDL_SetWindowTitle(sdlext_window->window, config->window.title);

        if(set == SDL_FALSE) {
            ERROR("Could not set SDL_window title: %s\n", SDL_GetError());
        }
    }
}