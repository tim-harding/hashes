#ifndef DIGEST_H
#define DIGEST_H

#include <stdbool.h>

#include "th_stdlib.h"
#include "shared.h"

typedef struct Hash {
    u32 a;
    u32 b;
    u32 c;
    u32 d;
    u32 e;
} Hash;

void Hash_print(Hash hash);
bool Hash_equal(Hash left, Hash right);
Hash Hash_from_be(u32 a, u32 b, u32 c, u32 d, u32 e);
Hash Hash_sum(Hash left, Hash right);

#endif