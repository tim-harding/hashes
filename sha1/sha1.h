#ifndef SHA1_H
#define SHA1_H

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"
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

typedef union Block {
    u8 byte[BLOCK_BYTES];
    u32 word[BLOCK_U32S];
    u64 long_word[BLOCK_U64S];
} Block;

Hash hash(const char* message);

#endif