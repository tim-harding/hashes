#include "md5.h"

// https://datatracker.ietf.org/doc/html/rfc1321

u32 rotate_left(u32 n, u8 bits) { return (n << bits) | (n >> (32 - bits)); }

Digest shuffle(Block* block, Digest digest, u32 i, u32 f, u32 g) {
    f += digest.a + SINES[i] + block->word[g];
    digest.a = digest.d;
    digest.d = digest.c;
    digest.c = digest.b;
    u32 shifts_index = (i / 16) * 4 + (i % 4);
    digest.b += rotate_left(f, SHIFTS[shifts_index]);
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

    return Digest_sum(digest, inner);
}

void compute_sines(u32 sines[BLOCK_BYTES]) {
    for (u32 i = 0; i < BLOCK_BYTES; i++) {
        sines[i] = floor(SINE_MULTIPLICAND * fabs(sin(i + 1)));
    }
}

Digest hash(const char* message) {
    compute_sines(SINES);
    Block* blocks = (Block*)message;
    u32 message_length = strlen(message);
    u32 whole_block_count = message_length / BLOCK_BYTES;
    u32 last_block_remainder = message_length % BLOCK_BYTES;

    Digest digest = {
        .a = 0x67452301,
        .b = 0xefcdab89,
        .c = 0x98badcfe,
        .d = 0x10325476,
    };

    for (u32 block_i = 0; block_i < whole_block_count; block_i++) {
        digest = contribute_block(&blocks[block_i], digest);
    }

    Block padded_block;
    memset(&padded_block, 0, BLOCK_BYTES);
    memcpy(&padded_block, &blocks[whole_block_count], last_block_remainder);
    padded_block.byte[last_block_remainder] = 0x80;
    if (last_block_remainder > FINAL_BLOCK_MESSAGE_BYTES) {
        digest = contribute_block(&padded_block, digest);
        memset(&padded_block.byte, 0, BLOCK_BYTES);
    }
    padded_block.long_word[BLOCK_U64S - 1] = message_length * BITS_PER_BYTE;
    digest = contribute_block(&padded_block, digest);

    return digest;
}