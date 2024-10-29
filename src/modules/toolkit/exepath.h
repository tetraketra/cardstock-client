#ifndef TTK_EXEPATH_H
#define TTK_EXEPATH_H

#include "toolkit.h"

/*
    === INFO ===
    1. Cross-platform (hopefully) executable path fetcher.
*/

extern void exepath_get(char* buffer, size_t buffer_len);

#define PATH_MAX 4096

#endif