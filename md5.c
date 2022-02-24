#include "md5.h"

// https://datatracker.ietf.org/doc/html/rfc1321

u32 rotate_left(u32 n, u8 bits) { return (n << bits) | (n >> (32 - bits)); }

Digest shuffle(Block* block, Digest digest, u32 i, u32 f, u32 g) {
    f += digest.a + SINES[i] + block->word[g];
    digest.a = digest.d;
    digest.d = digest.c;
    digest.c = digest.b;
    digest.b += rotate_left(f, SHIFTS[i]);
    return digest;
}

Digest contribute_block(Block* block, Digest digest) {
    Digest inner = digest;

    for (u32 i = 0; i < 16; i++) {
        u32 f = (inner.b & inner.c) | (~inner.b & inner.d);
        u32 g = i;
        inner = shuffle(block, inner, i, f, g);
    }

    for (u32 i = 16; i < 32; i++) {
        u32 f = (inner.d & inner.b) | (~inner.d & inner.c);
        u32 g = (5 * i + 1) % 16;
        inner = shuffle(block, inner, i, f, g);
    }

    for (u32 i = 32; i < 48; i++) {
        u32 f = inner.b ^ inner.c ^ inner.d;
        u32 g = (3 * i + 5) % 16;
        inner = shuffle(block, inner, i, f, g);
    }

    for (u32 i = 48; i < 64; i++) {
        u32 f = inner.c ^ (inner.b | ~inner.d);
        u32 g = (7 * i) % 16;
        inner = shuffle(block, inner, i, f, g);
    }

    digest.a += inner.a;
    digest.b += inner.b;
    digest.c += inner.c;
    digest.d += inner.d;

    return digest;
}

// Message will be reallocated with padding for now.
// Would it be better to add to the function contract
// that the string allocation be padded in some way?
Digest hash(const char* message) {
    Block* blocks = (Block*) message;
    PaddedMessage padded = PaddedMessage_from_cstr(message);

    // When written in little-endian, these just count up and down
    Digest digest = {
        .a = 0x67452301,
        .b = 0xefcdab89,
        .c = 0x98badcfe,
        .d = 0x10325476,
    };

    for (u32 block_i = 0; block_i < padded.block_count - 1; block_i++) {
        digest = contribute_block(&blocks[block_i], digest);
    }

    {
        digest =
            contribute_block(&padded.blocks[padded.block_count - 1], digest);
    }

    PaddedMessage_free(&padded);

    return digest;
}