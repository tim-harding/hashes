#include "digest.h"

bool Digest_equal(Digest left, Digest right) {
    return
        left.a == right.a &&
        left.b == right.b &&
        left.c == right.c &&
        left.d == right.d;
}

void print_little_endian(u32 n) {
    u8* bytes = (u8*) &n;
    for (int i = 0; i < 4; i++) {
        printf("%02x", bytes[i]);
    }
}

void Digest_print(Digest digest) {
    print_little_endian(digest.a);
    print_little_endian(digest.b);
    print_little_endian(digest.c);
    print_little_endian(digest.d);
}