#include "th_stdlib.h"

// Not used by print_any due to conflicts, but still handy.
debug_primitive(char, c);

debug_primitive(float, g);
debug_primitive(double, g);
debug_primitive(i8, i);
debug_primitive(i16, i);
debug_primitive(i32, i);
debug_primitive(i64, li);
debug_primitive(u8, u);
debug_primitive(u16, u);
debug_primitive(u32, u);
debug_primitive(u64, lu);
