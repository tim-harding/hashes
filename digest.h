#ifndef DIGEST_H
#define DIGEST_H

#include <stdbool.h>
#include "th_stdlib.h"

typedef struct Digest {
    u32 a;
    u32 b;
    u32 c;
    u32 d;
} Digest;

void Digest_print(Digest digest);
bool Digest_equal(Digest left, Digest right);
Digest Digest_from_be(u32 a, u32 b, u32 c, u32 d);

#endif