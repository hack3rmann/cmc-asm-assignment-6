#ifndef _INTEGRATOR_UTIL_H_
#define _INTEGRATOR_UTIL_H_

#include "types.h"



/// Expression-safe `max` macro
#define max(lhs, rhs) ({ \
    typeof(lhs) __left = (lhs); \
    typeof(rhs) __right = (rhs); \
    __left >= __right ? __left : __right; \
})

/// Expression-safe `min` macro
#define min(lhs, rhs) ({ \
    typeof(lhs) __left = (lhs); \
    typeof(rhs) __right = (rhs); \
    __left <= __right ? __left : __right; \
})

/// Drop implementation for types without drop functionality.
void DropFn_noop(AddrMut ptr);



#endif // !_INTEGRATOR_UTIL_H_