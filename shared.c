#include "shared.h"

void print_bytes(u8* bytes, u32 byte_count) {
    for (u32 i = 0; i < byte_count; i++) {
        printf("%02x", bytes[i]);
    }
}