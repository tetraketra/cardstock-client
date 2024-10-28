#include "rle.h"

/* Defines an `array+len => rle_t` encoding func. */
#define DEFINE_RLE_ENCODE_FOR_TYPE(type) \
    rle_t rle_encode_##type(type* arr, size_t len) { \
        uint64_t* buffer = calloc(len*2, sizeof(uint64_t)); \
        size_t buffer_pos = 0; \
        uint64_t count_of_same = 1; \
        type currently_counting = arr[0]; \
        for (size_t i = 1; i < len; i++) { \
            if (arr[i] != currently_counting) { \
                buffer[buffer_pos++] = count_of_same; \
                buffer[buffer_pos++] = currently_counting; \
                count_of_same = 0; \
                currently_counting = arr[i]; \
            } \
            if (i == len - 1) { \
                buffer[buffer_pos++] = ++count_of_same; \
                buffer[buffer_pos++] = currently_counting; \
            } \
            count_of_same++; \
        } \
        return (rle_t){.data = realloc(buffer, sizeof(uint64_t) * buffer_pos), .len = buffer_pos}; \
    }

/* Defines an `arr_*_t => rle_t` encoding func. */
#define DEFINE_RLE_ARR_ENCODE_FOR_TYPE(type) \
    rle_t rle_encode_arr_##type(arr_##type arr) { \
        uint64_t* buffer = calloc(arr.len*2, sizeof(uint64_t)); \
        size_t buffer_pos = 0; \
        uint64_t count_of_same = 1; \
        type currently_counting = arr.data[0]; \
        for (size_t i = 1; i < arr.len; i++) { \
            if (arr.data[i] != currently_counting) { \
                buffer[buffer_pos++] = count_of_same; \
                buffer[buffer_pos++] = currently_counting; \
                count_of_same = 0; \
                currently_counting = arr.data[i]; \
            } \
            if (i == arr.len - 1) { \
                buffer[buffer_pos++] = ++count_of_same; \
                buffer[buffer_pos++] = currently_counting; \
            } \
            count_of_same++; \
        } \
        return (rle_t){.data = realloc(buffer, sizeof(uint64_t) * buffer_pos), .len = buffer_pos}; \
    }

/* Defines an `rle_t => arr_*_t` decoding func. */
#define DEFINE_RLE_DECODE_FOR_TYPE(type) \
    arr_##type rle_decode_##type(rle_t rle) { \
        size_t decoded_len = 0; \
        for (size_t i = 0; i < rle.len; i += 2) { \
            decoded_len += rle.data[i]; \
        } \
        arr_##type array = {.data = (type*)calloc(decoded_len, sizeof(type)), .len = decoded_len}; \
        size_t array_index = 0; \
        for (size_t i = 0; i < rle.len; i += 2) { \
            for (size_t j = 0; j < rle.data[i]; j++) { \
                array.data[array_index++] = rle.data[i+1]; \
            } \
        } \
        return array; \
    }

/* Defines all encoding and decoding functions for a given type. */
#define DEFINE_RLE_FUNC_DEFINITIONS_FOR_TYPE(type) \
    DEFINE_RLE_ARR_ENCODE_FOR_TYPE(type) \
    DEFINE_RLE_ENCODE_FOR_TYPE(type) \
    DEFINE_RLE_DECODE_FOR_TYPE(type)

DEFINE_RLE_FUNC_DEFINITIONS_FOR_TYPE(int8_t);
DEFINE_RLE_FUNC_DEFINITIONS_FOR_TYPE(int16_t);
DEFINE_RLE_FUNC_DEFINITIONS_FOR_TYPE(int32_t);
DEFINE_RLE_FUNC_DEFINITIONS_FOR_TYPE(int64_t);
DEFINE_RLE_FUNC_DEFINITIONS_FOR_TYPE(uint8_t);
DEFINE_RLE_FUNC_DEFINITIONS_FOR_TYPE(uint16_t);
DEFINE_RLE_FUNC_DEFINITIONS_FOR_TYPE(uint32_t);
DEFINE_RLE_FUNC_DEFINITIONS_FOR_TYPE(uint64_t);
DEFINE_RLE_FUNC_DEFINITIONS_FOR_TYPE(float);
DEFINE_RLE_FUNC_DEFINITIONS_FOR_TYPE(double);
DEFINE_RLE_FUNC_DEFINITIONS_FOR_TYPE(char);