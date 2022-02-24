#include "md5.h"

// https://datatracker.ietf.org/doc/html/rfc1321

u32 rotate_left(u32 n, u8 bits) { return (n << bits) | (n >> (32 - bits)); }

Digest shuffle(Block* block, Digest digest, u32 i, u32 f, u32 g) {
    f += digest.a + SINES[i] + (block->u32)[g];
    digest.a = digest.d;
    digest.d = digest.c;
    digest.c = digest.b;
    digest.b += rotate_left(f, SHIFTS[i]);
    return digest;
}

u32 get_block_count(const u32 message_length) {
    u32 q = message_length / BLOCK_BYTES + 1;
    u32 r = message_length % BLOCK_BYTES;
    // The last block needs 9 bytes available
    // for padding and to store the message length.
    if (r < LAST_BLOCK_MESSAGE_SPACE) {
        // The last block has enough space to shove in our extras.
        return q;
    } else {
        // We need to add another block worth of space.
        return q + 1;
    }
}

Digest apply_block(Digest digest, Block* block) {
    Digest inner = digest;

    for (u32 i = 0; i < 16; i++) {
        u32 f = (digest.b & digest.c) | (~digest.b & digest.d);
        u32 g = i;
        shuffle(block, digest, i, f, g);
    }

    for (u32 i = 16; i < 32; i++) {
        u32 f = (digest.d & digest.b) | (~digest.d & digest.c);
        u32 g = (5 * i + 1) % 16;
        shuffle(block, digest, i, f, g);
    }

    for (u32 i = 32; i < 48; i++) {
        u32 f = digest.b ^ digest.c ^ digest.d;
        u32 g = (3 * i + 5) % 16;
        shuffle(block, digest, i, f, g);
    }

    for (u32 i = 48; i < 64; i++) {
        u32 f = digest.c ^ (digest.b | ~digest.d);
        u32 g = (7 * i) % 16;
        shuffle(block, digest, i, f, g);
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
    Block* message_blocks = (Block*)message;
    u32 message_length = strlen(message);
    u32 block_count = get_block_count(message_length);

    Digest digest = {
        .a = 0x67452301,
        .b = 0xefcdab89,
        .c = 0x98badcfe,
        .d = 0x10325476,
    };

    for (u32 block_i = 0; block_i < block_count - 1; block_i++) {
        Block* block = &message_blocks[block_i];
        digest = apply_block(digest, block);
    }

    {
        Block block;
        u32 message_remainder = message_length % BLOCK_BYTES;
        memcpy(&block, &message_blocks[block_count - 1], message_remainder);
        u32 bytes_to_zero = LAST_BLOCK_MESSAGE_SPACE - message_remainder;
        memset(&block.u8[message_remainder + 1], 0, bytes_to_zero);
        block.u8[message_remainder] = 0x80;
        block.u64[BLOCK_U64S - 1] = message_length * BITS_PER_BYTE;
        digest = apply_block(digest, &block);
    }

    return digest;
}