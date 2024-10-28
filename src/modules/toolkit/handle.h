#ifndef TTK_HANDLE_T
#define TTK_HANDLE_T

#include "toolkit.h"

typedef struct handle_t {
    void* data; /* pointer for constant base object addresses */
    size_t size; /* size of data */
    void (*free_fn)(void*); /* function which frees the data */
} handle_t;

/* Frees a handle's data using its `free_fn` if possible, `free` otherwise. */
#define FREE_HANDLE_SAFELY_WITH_FALLBACK(handle) FREE_SAFELY_WITH_FALLBACK(handle.free_fn, handle.data)

#endif