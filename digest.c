#include "digest.h"

bool Digest_equal(Digest left, Digest right) {
    return
        left.a == right.a &&
        left.b == right.b &&
        left.c == right.c &&
        left.d == right.d;
}

void Digest_print(Digest digest) {
    printf("%x%x%x%x", digest.a, digest.b, digest.c, digest.d);
}