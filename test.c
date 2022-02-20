#include "test.h"

int main() {
    char message1[] = "The quick brown fox jumps over the lazy dog";
    Digest expected1 =
        Digest_from_be(0x9e107d9d, 0x372bb682, 0x6bd81d35, 0x42a419d6);
    test_digest(message1, expected1);

    char message2[] = "The quick brown fox jumps over the lazy dog.";
    Digest expected2 =
        Digest_from_be(0xe4d909c2, 0x90d0fb1c, 0xa068ffad, 0xdf22cbd0);
    test_digest(message2, expected2);

    char message3[] = "";
    Digest expected3 =
        Digest_from_be(0xd41d8cd9, 0x8f00b204, 0xe9800998, 0xecf8427e);
    test_digest(message3, expected3);

    char message4[] = "Hello";
    // 0x28 = 5 * 8
    char expected4[] =
        "Hello\x80"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\x28\0\0\0\0\0\0\0";  // The length is little-endian
    test_padding(message4, expected4, sizeof(expected4) - 1);

    char message5[] = "";
    char expected5[] =
        "\x80\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
        "\0\0\0\0\0\0\0\0";
    test_padding(message5, expected5, sizeof(expected5) - 1);
}

void print_bytes(u8* bytes, u32 byte_count) {
    for (u32 i = 0; i < byte_count; i++) {
        printf("%02x", bytes[i]);
    }
}

void test_padding(const char* message, const char* expected,
                  u32 expected_length) {
    PaddedMessage padded = PaddedMessage_from_cstr(message);
    u32 padded_length = PaddedMessage_length_in_bytes(&padded);
    if (padded_length != expected_length) {
        printf(
            "The padded message was a different length from the expected "
            "message:\n\tExpected: %d\n\tReceived: %d\n\n",
            expected_length, padded_length);
        return;
    }

    if (memcmp(padded.blocks, expected, expected_length) == 0) {
        printf("Success: %s\n\n", expected);
    } else {
        printf("Failure: \n\tExpected: ");
        print_bytes((u8*)expected, expected_length);
        printf("\n\tReceived: ");
        print_bytes((u8*)padded.blocks, padded.block_count * BLOCK_BYTES);
        printf("\n\n");
    }
}

void test_digest(char* message, Digest expected) {
    Digest digest = hash(message);
    if (Digest_equal(digest, expected)) {
        printf("Success: %s\n\n", message);
    } else {
        printf("Failure\n\tExpected: ");
        Digest_print(expected);
        printf("\n\tReceived: ");
        Digest_print(digest);
        printf("\n\n");
    }
}