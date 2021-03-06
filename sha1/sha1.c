#include "sha1.h"

u32 rotate_left(u32 n, u8 bits) { return (n << bits) | (n >> (32 - bits)); }

Hash shuffle(u32 w[80], Hash state, u32 i, u32 f, u32 k) {
    Hash out = {
        .a = rotate_left(state.a, 5) + f + state.e + k + w[i],
        .b = state.a,
        .c = rotate_left(state.b, 30),
        .d = state.c,
        .e = state.d,
    };
    return out;
}

Hash contribute_block(Block* block, Hash previous_hash) {
    u32 w[80];
    memcpy(w, block, 16 * sizeof(u32));
    for (u32 i = 16; i < 80; i++) {
        w[i] = rotate_left(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    }

    Hash hash = previous_hash;

    for (u32 i = 0; i < 20; i++) {
        u32 f = (hash.b & hash.c) | (~hash.b & hash.d);
        hash = shuffle(w, hash, i, f, 0x5A827999);
    }

    for (u32 i = 20; i < 40; i++) {
        u32 f = hash.b ^ hash.c ^ hash.d;
        hash = shuffle(w, hash, i, f, 0x6ED9EBA1);
    }

    for (u32 i = 40; i < 60; i++) {
        u32 f = (hash.b & hash.c) | (hash.b & hash.d) | (hash.c & hash.d);
        hash = shuffle(w, hash, i, f, 0x8F1BBCDC);
    }

    for (u32 i = 60; i < 80; i++) {
        u32 f = hash.b ^ hash.c ^ hash.d;
        hash = shuffle(w, hash, i, f, 0xCA62C1D6);
    }

    return Hash_sum(previous_hash, hash);
}

Hash hash(const char* message) {
    Block* blocks = (Block*)message;
    u32 message_length = strlen(message);
    u32 whole_block_count = message_length / BLOCK_BYTES;
    u32 last_block_remainder = message_length % BLOCK_BYTES;

    Hash hash = {
        .a = 0x67452301,
        .b = 0xEFCDAB89,
        .c = 0x98BADCFE,
        .d = 0x10325476,
        .e = 0xC3D2E1F0,
    };

    for (u32 block_i = 0; block_i < whole_block_count; block_i++) {
        hash = contribute_block(&blocks[block_i], hash);
    }

    Block padded_block;
    memset(&padded_block, 0, BLOCK_BYTES);
    memcpy(&padded_block, &blocks[whole_block_count], last_block_remainder);
    padded_block.byte[last_block_remainder] = 0x80;
    if (last_block_remainder > FINAL_BLOCK_MESSAGE_BYTES) {
        hash = contribute_block(&padded_block, hash);
        memset(&padded_block.byte, 0, BLOCK_BYTES);
    }
    padded_block.long_word[BLOCK_U64S - 1] =
        reverse_endianness_u64(message_length * BITS_PER_BYTE);
    hash = contribute_block(&padded_block, hash);

    return hash;
}