#include "shared.h"

void swap(u8* bytes, u8 i, u8 j) {
    u8 tmp = bytes[i];
    bytes[i] = bytes[j];
    bytes[j] = tmp;
}

u32 reverse_endianness_u32(u32 n) {
    u8* bytes = (u8*)&n;
    swap(bytes, 0, 3);
    swap(bytes, 1, 2);
    return n;
}

u64 reverse_endianness_u64(u64 n) {
    u8* bytes = (u8*)&n;
    swap(bytes, 0, 7);
    swap(bytes, 1, 6);
    swap(bytes, 2, 5);
    swap(bytes, 3, 4);
    return n;
}