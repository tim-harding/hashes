#include "digest.h"

bool Digest_equal(Hash left, Hash right) {
    return left.a == right.a && left.b == right.b && left.c == right.c &&
           left.d == right.d;
}

void swap(u8* bytes, u8 i, u8 j) {
    u8 tmp = bytes[i];
    bytes[i] = bytes[j];
    bytes[j] = tmp;
}

u32 reverse_endianness(u32 n) {
    u8* bytes = (u8*)&n;
    swap(bytes, 0, 3);
    swap(bytes, 1, 2);
    return n;
}

Hash Digest_from_be(u32 a, u32 b, u32 c, u32 d) {
    Hash digest = {
        .a = reverse_endianness(a),
        .b = reverse_endianness(b),
        .c = reverse_endianness(c),
        .d = reverse_endianness(d),
    };
    return digest;
}

void print_le(u32 n) {
    u8* bytes = (u8*)&n;
    for (u8 i = 0; i < 4; i++) {
        printf("%02x", bytes[i]);
    }
}

void Digest_print(Hash digest) {
    print_le(digest.a);
    print_le(digest.b);
    print_le(digest.c);
    print_le(digest.b);
}

Hash Digest_sum(Hash left, Hash right) {
    Hash out = {
        .a = left.a + right.a,
        .b = left.b + right.b,
        .c = left.c + right.c,
        .d = left.d + right.d,
    };
    return out;
}