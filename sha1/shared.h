#ifndef SHARED_H
#define SHARED_H

#include "th_stdlib.h"

void swap(u8* bytes, u8 i, u8 j);
u32 reverse_endianness_u32(u32 n);
u64 reverse_endianness_u64(u64 n);

#endif