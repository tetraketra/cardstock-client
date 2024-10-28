#ifndef TTK_HEXDUMP_H
#define TTK_HEXDUMP_H

#include "toolkit.h"

/*
    === INFO ===
    1. It's a hexdump; 'nuf said :V
*/

extern void hex_dump(FILE* stream, void* buffer, size_t buf_sb, char* desc);

#endif