#ifndef TTK_RLE_H
#define TTK_RLE_H

#include "toolkit.h"

/*
    === INFO ===
    1. Run-length encoding and decoding.
       - 2485 == two 4s, eight 5s == 4455555555. 
    3. Decoding function expects target data size.
*/

typedef struct rle_t {
    uint64_t* data;
    size_t len;
} rle_t;

#define DEFINE_RLE_FUNC_TEMPLATES_FOR_TYPE(type) \
    extern rle_t        rle_encode_##type(type* arr, size_t len); \
    extern rle_t        rle_encode_arr_##type(arr_##type arr); \
    extern arr_##type   rle_decode_##type(rle_t rle);

DEFINE_RLE_FUNC_TEMPLATES_FOR_TYPE(int8_t);
DEFINE_RLE_FUNC_TEMPLATES_FOR_TYPE(int16_t);
DEFINE_RLE_FUNC_TEMPLATES_FOR_TYPE(int32_t);
DEFINE_RLE_FUNC_TEMPLATES_FOR_TYPE(int64_t);
DEFINE_RLE_FUNC_TEMPLATES_FOR_TYPE(uint8_t);
DEFINE_RLE_FUNC_TEMPLATES_FOR_TYPE(uint16_t);
DEFINE_RLE_FUNC_TEMPLATES_FOR_TYPE(uint32_t);
DEFINE_RLE_FUNC_TEMPLATES_FOR_TYPE(uint64_t);
DEFINE_RLE_FUNC_TEMPLATES_FOR_TYPE(float);
DEFINE_RLE_FUNC_TEMPLATES_FOR_TYPE(double);
DEFINE_RLE_FUNC_TEMPLATES_FOR_TYPE(char);

#endif