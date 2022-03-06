#ifndef DIGEST_H
#define DIGEST_H

#include <stdbool.h>

#include "th_stdlib.h"

typedef struct Hash {
    u32 a;
    u32 b;
    u32 c;
    u32 d;
} Hash;

void Digest_print(Hash digest);
bool Digest_equal(Hash left, Hash right);
Hash Digest_from_be(u32 a, u32 b, u32 c, u32 d);
Hash Digest_sum(Hash left, Hash right);

#endif