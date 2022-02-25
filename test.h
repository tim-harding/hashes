#ifndef TEST_H
#define TEST_H

#include "md5.h"
#include "padded_message.h"
#include "digest.h"
#include "th_stdlib.h"

void test_padding(const char* message, const char* expected, u32 expected_length);
void test_digest(char* message, Digest expected);

#endif