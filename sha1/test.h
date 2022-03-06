#ifndef TEST_H
#define TEST_H

#include "hash.h"
#include "md5.h"
#include "th_stdlib.h"

void test_padding(const char* message, const char* expected,
                  u32 expected_length);
void test_digest(char* message, Hash expected);

#endif