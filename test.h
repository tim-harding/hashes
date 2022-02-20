#ifndef TEST_H
#define TEST_H

#include "md5.h"
#include "padded_message.h"
#include "digest.h"
#include "th_stdlib.h"

void test_padding(const char* message, const u32 message_length, const char* expected, const u32 expected_length);
void test_digest(char* message, const u32 message_length, Digest expected);

#endif