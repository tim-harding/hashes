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
}

void test_digest(char* message, Digest expected) {
    Digest digest = hash(message);
    if (Digest_equal(digest, expected)) {
        printf("Success");
    } else {
        printf("Failure");
    }
    printf("\n\tExpected: ");
    Digest_print(expected);
    printf("\n\tReceived: ");
    Digest_print(digest);
    printf("\n\n");
}