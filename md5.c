#include "md5.h"

// https://datatracker.ietf.org/doc/html/rfc1321

u32 rotate_left(u32 n, u8 bits) {
    return (n << bits) | (n >> (32 - bits));
}

void shuffle(Block_u32* block, u32* a, u32* b, u32* c, u32* d, u32 i, u32 f, u32 g) {
    f += *a + SINES[i] + (*block)[g];
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
    u32 a0 = 0x67452301;
    u32 b0 = 0xefcdab89;
    u32 c0 = 0x98badcfe;
    u32 d0 = 0x10325476;

    for (u32 block_i = 0; block_i < padded.block_count; block_i++) {
        Block_u32* block = (Block_u32*) padded.blocks[block_i];

        u32 a = a0;
        u32 b = b0;
        u32 c = c0;
        u32 d = d0;

/*
        for (u32 i = 0; i < 64; i++) {
            u32 f, g;
            if (i < 16) {
                f = (b & c) | (~b & d);
                g = i;
            } else if (i < 32) {
                f = (d & b) | (~d & c);
                g = (5 * i + 1) % 16;
            } else if (i < 48) {
                f = b ^ c ^ d;
                g = (3 * i + 5) % 16;
            } else {
                f = c ^ (b | ~d);
                g = (7 * i) % 16;
            }
        }
        */

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

        a0 += a;
        b0 += b;
        c0 += c;
        d0 += d;
    }

    PaddedMessage_free(&padded);

    Digest digest = {
        .a = a0,
        .b = b0,
        .c = c0,
        .d = d0,
    };
    return digest;
}