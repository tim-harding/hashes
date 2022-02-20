#include "padded_message.h"

u32 get_block_count(const u32 message_length) {
    u32 q = message_length / BLOCK_BYTES + 1;
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

PaddedMessage PaddedMessage_from_cstr(const char* message, const u32 message_length) {
    u32 block_count = get_block_count(message_length);
    Block_byte* blocks = malloc(block_count * sizeof(Block_byte));
    char* buffer = (char*) blocks;

    memcpy(blocks, message, message_length);
    // Append a single bit to the end of the message
    buffer[message_length] = 0x80;
    for (u32 i = message_length + 1, end = block_count * BLOCK_BYTES - MESSAGE_LENGTH_BYTES; i < end; i++) {
        buffer[i] = 0x00;
    }

    u64 message_length_in_bits = ((u64) message_length) * BITS_PER_BYTE;
    Block_u64* last_block = (Block_u64*) &blocks[block_count - 1];
    (*last_block)[BLOCK_U64S - 1] = message_length_in_bits;

    PaddedMessage out = {
        .block_count = block_count,
        .blocks = blocks,
    };

    return out;
}

u32 PaddedMessage_length_in_bytes(PaddedMessage* message) {
    return message->block_count * BLOCK_BYTES;
}

void PaddedMessage_free(PaddedMessage* message) {
    free(message->blocks);
}