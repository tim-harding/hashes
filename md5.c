#include "md5.h"

// https://datatracker.ietf.org/doc/html/rfc1321

u32 rotate_left(u32 n, u8 bits) {
    return (n << bits) | (n >> (32 - bits));
}

void shuffle(Block* block, u32* a, u32* b, u32* c, u32* d, u32 i, u32 f, u32 g) {
    f += *a + SINES[i] + block->word[g];
    *a = *d;
    *d = *c;
    *c = *b;
    *b += rotate_left(f, SHIFTS[i]);
}

// Message will be reallocated with padding for now.
// Would it be better to add to the function contract
// that the string allocation be padded in some way?
Digest hash(const char* message) {
    PaddedMessage padded = PaddedMessage_from_cstr(message);

    // When written in little-endian, these just count up and down
    Digest digest = {
        .a = 0x67452301,
        .b = 0xefcdab89,
        .c = 0x98badcfe,
        .d = 0x10325476,
    };

    for (u32 block_i = 0; block_i < padded.block_count; block_i++) {
        Block* block = &padded.blocks[block_i];

        u32 a = digest.a;
        u32 b = digest.b;
        u32 c = digest.c;
        u32 d = digest.d;

        for (u32 i = 0; i < 16; i++) {
            u32 f = (b & c) | (~b & d);
            u32 g = i;
            shuffle(block, &a, &b, &c, &d, i, f, g);
        }

        for (u32 i = 16; i < 32; i++) {
            u32 f = (d & b) | (~d & c);
            u32 g = (5 * i + 1) % 16;
            shuffle(block, &a, &b, &c, &d, i, f, g);
        }

        for (u32 i = 32; i < 48; i++) {
            u32 f = b ^ c ^ d;
            u32 g = (3 * i + 5) % 16;
            shuffle(block, &a, &b, &c, &d, i, f, g);
        }

        for (u32 i = 48; i < 64; i++) {
            u32 f = c ^ (b | ~d);
            u32 g = (7 * i) % 16;
            shuffle(block, &a, &b, &c, &d, i, f, g);
        }

        digest.a += a;
        digest.b += b;
        digest.c += c;
        digest.d += d;
    }

    PaddedMessage_free(&padded);

    return digest;
}