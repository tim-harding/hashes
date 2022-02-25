#ifndef PADDED_MESSAGE_H
#define PADDED_MESSAGE_H

#include <stdlib.h>
#include <string.h>

#include "shared.h"

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

typedef struct PaddedMessage {
    u32 block_count;
    Block* blocks;
} PaddedMessage;

PaddedMessage PaddedMessage_from_cstr(const char* message);
void PaddedMessage_free(PaddedMessage* message);
u32 PaddedMessage_length_in_bytes(PaddedMessage* message);
void PaddedMessage_print_hex(PaddedMessage* message);

#endif