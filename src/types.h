#ifndef _INTEGRATOR_TYPES_H_
#define _INTEGRATOR_TYPES_H_

#include <stdint.h>
#include <stddef.h>



typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;
typedef float f32;
typedef double f64;
typedef size_t usize;
typedef ptrdiff_t isize;
typedef _Bool bool;

#define null ((void*) 0)
#define true ((bool) 1)
#define false ((bool) 0)



#endif // !_INTEGRATOR_TYPES_H_