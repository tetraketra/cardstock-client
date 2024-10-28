#include "toolkit/hexdump.h"

/*
    Dump memory as formatted hex.

    @param `stream`: The stream to write the hex dump to (`stderr` recommended) {`!NULL`}.
    @param `buffer`: The buffer whose bytes to write to the stream {`!NULL`}.
    @param `buf_sb`: The size of the buffer in bytes {`!NULL`}.
    @param `desc`: An optional description of the buffer {`>0`}.
*/
void hex_dump(FILE* stream, void* buffer, size_t buf_sb, char* desc) {
    RUNTIME_ASSERT(stream != NULL);
    RUNTIME_ASSERT(buffer != NULL);
    RUNTIME_ASSERT(desc != NULL);
    RUNTIME_ASSERT(buf_sb > 0);

    /* Hex dump. */
    uint8_t* bytes = (uint8_t*)buffer;
    for (size_t i = 0; i < buf_sb; i++) {

        /* Separate by 16 bytes for readability (line start). */
        if (i%16 == 0) {
            fprintf(stream, "  %04lu: ", i);
        }

        /* Print byte */
        fprintf(stream, "%02x ", bytes[i]);

        /* Separate by 16 bytes for readability (line end). */
        if (i%16 == 15 || i == buf_sb - 1) {
            fprintf(stream, "\n");
        }
    }
}