#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// Question: Is the message length given in bits or bytes?

#define BITS_PER_BYTE 8
// TODO: Possible removal of these constants
#define BYTES_PER_U32 32 / BITS_PER_BYTE
#define BYTES_PER_U64 64 / BITS_PER_BYTE

#define BLOCK_BITS  512
#define BLOCK_BYTES BLOCK_BITS  / BITS_PER_BYTE
#define BLOCK_U32S  BLOCK_BYTES / BYTES_PER_U32
#define BLOCK_U64S  BLOCK_BYTES / BYTES_PER_U64

// The message is postpended by a 64-bit number
// that gives the original message length.
#define MESSAGE_LENGTH_BYTES 8

// The message must be postpended by 0x80,
// which takes one byte.
#define MINIMUM_PADDING_BYTES 1

typedef u8  Block_byte[BLOCK_BYTES];
typedef u32 Block_u32 [BLOCK_U32S];
typedef u64 Block_u64 [BLOCK_U64S];

typedef struct PaddedMessage {
    u32 block_count;
    Block_byte* blocks;
} PaddedMessage;

// Todo: Reduce this using modulus
const u8 SHIFTS[] = {
    7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
    5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
    4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
    6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,
};

// Precomputed from floor(2^32 * abs(sin(i+1)))
// for i in [0..64]
const u32 SINES[] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
};

int main() {

}

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
void hash(const u8* message, const u32 message_length, u32 (*digest)[4]) {
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