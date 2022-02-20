#ifndef TH_STDLIB_H
#define TH_STDLIB_H

#include <stdint.h>
#include <stdio.h>

#define i8 int8_t
#define i16 int16_t
#define i32 int32_t
#define i64 int64_t
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define debug_primitive(T, fmt)                                 \
    int debug_##T(T value, char* variable_name) {               \
        return printf("%s: %" #fmt "\n", variable_name, value); \
    }

#define forward_declare_debug_primitive(T, fmt) \
    int debug_##T(T value, char* variable_name);

forward_declare_debug_primitive(char, c);
forward_declare_debug_primitive(float, g);
forward_declare_debug_primitive(double, g);
forward_declare_debug_primitive(i8, i);
forward_declare_debug_primitive(i16, i);
forward_declare_debug_primitive(i32, i);
forward_declare_debug_primitive(i64, li);
forward_declare_debug_primitive(u8, u);
forward_declare_debug_primitive(u16, u);
forward_declare_debug_primitive(u32, u);
forward_declare_debug_primitive(u64, lu);

#define debug(X) _Generic((X),            \
                          float           \
                          : debug_float,  \
                            double        \
                          : debug_double, \
                            i8            \
                          : debug_i8,     \
                            i16           \
                          : debug_i16,    \
                            i32           \
                          : debug_i32,    \
                            i64           \
                          : debug_i64,    \
                            u8            \
                          : debug_u8,     \
                            u16           \
                          : debug_u16,    \
                            u32           \
                          : debug_u32,    \
                            u64           \
                          : debug_u64)(X, #X)

#define trace() printf("TRACE %03d %s\n", __LINE__, __FILE__)

#endif