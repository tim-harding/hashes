#include "hash.h"

bool Hash_equal(Hash left, Hash right) {
    return left.a == right.a && left.b == right.b && left.c == right.c &&
           left.d == right.d && left.e == right.e;
}

Hash Hash_from_be(u32 a, u32 b, u32 c, u32 d, u32 e) {
    Hash hash = {
        .a = reverse_endianness_u32(a),
        .b = reverse_endianness_u32(b),
        .c = reverse_endianness_u32(c),
        .d = reverse_endianness_u32(d),
        .e = reverse_endianness_u32(e),
    };
    return hash;
}

void print_le(u32 n) {
    u8* bytes = (u8*)&n;
    for (u8 i = 0; i < 4; i++) {
        printf("%02x", bytes[i]);
    }
}

void Hash_print(Hash hash) {
    print_le(hash.a);
    print_le(hash.b);
    print_le(hash.c);
    print_le(hash.d);
    print_le(hash.e);
}

Hash Hash_sum(Hash left, Hash right) {
    Hash out = {
        .a = left.a + right.a,
        .b = left.b + right.b,
        .c = left.c + right.c,
        .d = left.d + right.d,
        .e = left.e + right.e,
    };
    return out;
}