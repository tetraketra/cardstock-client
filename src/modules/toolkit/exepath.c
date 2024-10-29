#include "exepath.h"

/*
    Gets the directory of program in a cross-platform manner.
    Windows route has yet to be tested.

    @param `buffer`: The buffer to print the path to.
    @param `buffer_len`: The length of said buffer.
*/
void exepath_get(char *buffer, size_t buffer_len) {
#ifdef BUIDFLAG_WINDOWS
    GetModuleFileName(NULL, buffer, buffer_len);
    char *last_backslash = strrchr(buffer, '\\');
    if (last_backslash) {
        *last_backslash = '\0';
    }
#else
    ssize_t len = readlink("/proc/self/exe", buffer, buffer_len - 1);
    if (len != -1) {
        buffer[len] = '\0';
        char *last_slash = strrchr(buffer, '/');
        if (last_slash) {
            *last_slash = '\0';
        }
    } else {
        perror("readlink");
        exit(EXIT_FAILURE);
    }
#endif
}