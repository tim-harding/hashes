#include "test.h"

int main() {
    char* message1 = "The quick brown fox jumps over the lazy dog";
    Digest expected1 = {
        .a = 0x9e107d9d,
        .b = 0x372bb682,
        .c = 0x6bd81d35,
        .d = 0x42a419d6,
    };
    test(message1, sizeof(message1), expected1);

    char* message2 = "The quick brown fox jumps over the lazy dog.";
    Digest expected2 = {
        .a = 0xe4d909c2,
        .b = 0x90d0fb1c,
        .c = 0xa068ffad,
        .d = 0xdf22cbd0,
    };
    test(message2, sizeof(message2), expected2);

    char* message3 = "";
    Digest expected3 = {
        .a = 0xd41d8cd9,
        .b = 0x8f00b204,
        .c = 0xe9800998,
        .d = 0xecf8427e,
    };
    test(message3, sizeof(message3), expected3);
}

void test(char* message, const u32 message_length, Digest expected) {
    Digest digest = hash(message, message_length);
    if (Digest_equal(digest, expected)) {
        printf("Success: %s\n\n", message);
    } else {
        printf("Failure: %s\n\tExpected: ");
        Digest_print(expected);
        printf("\n\tReceived: ");
        Digest_print(digest);
        print("\n\n");
    }
}