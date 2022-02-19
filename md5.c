#include "md5.h"

u32 get_block_count(const u32 message_length) {
    u32 q = message_length / BLOCK_BYTES;
    u32 r = message_length % BLOCK_BYTES;
    // The last block needs 9 bytes available
    // for padding and to store the message length.
    if (r < BLOCK_BYTES - MINIMUM_PADDING_BYTES - MESSAGE_LENGTH_BYTES) {
        // The last block has enough space to shove in our extras.
        return q;
    } else {
        // We need to add another block worth of space.
        return q + 1;
    }
}

PaddedMessage message_to_blocks(const u8* message, const u32 message_length) {
    u32 block_count = get_block_count(message_length);
    Block_byte* blocks = malloc(block_count * sizeof(Block_byte));
    u8* buffer = blocks;

    strcpy(blocks, message);
    buffer[message_length] = 0x80;
    for (u32 i = message_length + 1, end = block_count * BLOCK_BYTES - MESSAGE_LENGTH_BYTES; i < end; i++) {
        buffer[i] = 0x00;
    }

    u64 message_length_in_bits = ((u64) message_length) * BITS_PER_BYTE;
    Block_u64 last_block = blocks[block_count - 1];
    last_block[BLOCK_U64S - 1] = message_length_in_bits;

    PaddedMessage out = {
        .block_count = block_count,
        .blocks = blocks,
    };

    return out;
}

u32 left_rotate(u32 n, u8 bits) {
    return (n << bits) | (n >> (sizeof(u32) - bits));
}

// Message will be reallocated with padding for now.
// Would it be better to add to the function contract
// that the string allocation be padded in some way?
Digest hash(const u8* message, const u32 message_length) {
    PaddedMessage padded = message_to_blocks(message, message_length);

    // Constants are just some standard initial values.
    // TODO: Can we find out how these were chosen?
    u32 a0 = 0x67452301;
    u32 b0 = 0xefcdab89;
    u32 c0 = 0x98badcfe;
    u32 d0 = 0x10325476;

    for (u32 block_i = 0; block_i < padded.block_count; block_i++) {
        u32 a = a0;
        u32 b = b0;
        u32 c = c0;
        u32 d = d0;
        for (u32 i = 0; i < 16; i++) {
            u32 f = (b & c) | ((~b) & d);
            f += a + SINES[i] + SHIFTS[i];
            a = d;
            d = c;
            c = b;
            b += left_rotate(f, SHIFTS[block_i]);
        }
    }
}