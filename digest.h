#ifndef DIGEST_H
#define DIGEST_H

#include "shared.h"
#include <stdbool.h>

typedef struct Digest {
    u32 a;
    u32 b;
    u32 c;
    u32 d;
} Digest;

bool Digest_equal(Digest left, Digest right);

#endif