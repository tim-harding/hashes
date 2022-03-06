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

typedef struct State {
    u32 a;
    u32 b;
    u32 c;
    u32 d;
    u32 e;
} State;

void Hash_print(Hash hash);
bool Hash_equal(Hash left, Hash right);
Hash Hash_from_be(u32 a, u32 b, u32 c, u32 d, u32 e);
Hash Hash_sum(Hash left, Hash right);

void State_print(State state);
bool State_equal(State left, State right);
State State_from_be(u32 a, u32 b, u32 c, u32 d, u32 e);
State State_sum(State left, State right);

Hash State_to_hash(State state);

#endif