#ifndef _INTEGRATOR_UTIL_H_
#define _INTEGRATOR_UTIL_H_

#include "types.h"



#define max(lhs, rhs) ({ \
    typeof(lhs) __left = (lhs); \
    typeof(rhs) __right = (rhs); \
    __left >= __right ? __left : __right; \
})

#define min(lhs, rhs) ({ \
    typeof(lhs) __left = (lhs); \
    typeof(rhs) __right = (rhs); \
    __left <= __right ? __left : __right; \
})

void DropFn_noop(AddrMut ptr);



#endif // !_INTEGRATOR_UTIL_H_