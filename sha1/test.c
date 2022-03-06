#include "test.h"

int main() {
    char message1[] = "sha1 this string";
    Hash expected1 =
        Hash_from_be(0XCF23DF22, 0X07D99A74, 0XFBE169E3, 0XEBA035E6, 0X33B65D94);
    test_digest(message1, expected1);

    char message2[] = "The path of the righteous man is beset on all sides";
    Hash expected2 =
        Hash_from_be(0xD1A04E8A, 0xE06E4921, 0xE1088CA4, 0x8025BEB5, 0x1421F5BB);
    test_digest(message2, expected2);

    char message3[] = "";
    Hash expected3 =
        Hash_from_be(0XDA39A3EE, 0X5E6B4B0D, 0X3255BFEF, 0X95601890, 0XAFD80709);
    test_digest(message3, expected3);
}

void test_digest(char* message, Hash expected) {
    Hash digest = hash(message);
    if (Hash_equal(digest, expected)) {
        printf("Success");
    } else {
        printf("Failure");
    }
    printf("\n\tExpected: ");
    Hash_print(expected);
    printf("\n\tReceived: ");
    Hash_print(digest);
    printf("\n\n");
}