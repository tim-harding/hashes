#ifndef PADDED_MESSAGE_H
#define PADDED_MESSAGE_H

#include "shared.h"
#include <string.h>
#include <stdlib.h>

#define BITS_PER_BYTE 8
#define BLOCK_BITS  512
#define BLOCK_BYTES BLOCK_BITS / BITS_PER_BYTE
#define BLOCK_U32S  BLOCK_BITS / 32
#define BLOCK_U64S  BLOCK_BITS / 64

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

PaddedMessage PaddedMessage_from_cstr(const char* message, const u32 message_length);
void PaddedMessage_free(PaddedMessage* message);
u32 PaddedMessage_length_in_bytes(PaddedMessage* message);

#endif