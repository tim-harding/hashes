#ifndef MD5_H
#define MD5_H

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "digest.h"
#include "th_stdlib.h"

#define BITS_PER_BYTE 8
#define BLOCK_BITS 512
#define BLOCK_BYTES (BLOCK_BITS / BITS_PER_BYTE)
#define BLOCK_U32S (BLOCK_BITS / 32)
#define BLOCK_U64S (BLOCK_BITS / 64)

// The message is postpended by a 64-bit number
// that gives the original message length.
#define MESSAGE_LENGTH_BYTES 8

// The message must be postpended by 0x80,
// which takes one byte.
#define MINIMUM_PADDING_BYTES 1

#define FINAL_BLOCK_MESSAGE_BYTES \
    (BLOCK_BYTES - MESSAGE_LENGTH_BYTES - MINIMUM_PADDING_BYTES)

#define SINE_MULTIPLICAND ((u64)1 << (u64)32)

typedef union Block {
    u8 byte[BLOCK_BYTES];
    u32 word[BLOCK_U32S];
    u64 long_word[BLOCK_U64S];
} Block;

const u8 SHIFTS[] = {
    7, 12, 17, 22, // Round 1
    5, 9,  14, 20, // Round 2
    4, 11, 16, 23, // Round 3
    6, 10, 15, 21, // Round 4
};

u32 SINES[64];

Hash hash(const char* message);

#endif