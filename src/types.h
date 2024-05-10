#ifndef _INTEGRATOR_TYPES_H_
#define _INTEGRATOR_TYPES_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>



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
typedef void* AddrMut;
typedef void const* Addr;
typedef void (*DropFn)(AddrMut);
#define DropFn_DEFAULT DropFn_noop
#define DropFn(of) ((DropFn) (of))
#define DropFn_of(type) DropFn(type##_drop)

#define null ((void*) 0)

#define usize_MIN ((usize) 0)
#define usize_MAX ((usize) SIZE_MAX)
#define isize_MIN ((isize) PTRDIFF_MIN)
#define isize_MAX ((isize) PTRDIFF_MAX)
#define u8_MIN ((u8) 0)
#define u8_MAX ((u8) 255)
#define i8_MIN ((i8) -128)
#define i8_MAX ((i8) 127)
#define u16_MIN ((u16) 0)
#define u16_MAX ((u16) UINT16_MAX)
#define i16_MIN ((i16) INT16_MIN)
#define i16_MAX ((i16) INT16_MAX)
#define u32_MIN ((u32) 0)
#define u32_MAX ((u32) UINT32_MAX)
#define i32_MIN ((i32) INT32_MIN)
#define i32_MAX ((i32) INT32_MAX)
#define u64_MIN ((u64) 0)
#define u64_MAX ((u64) UINT64_MAX)
#define i64_MIN ((i64) INT64_MIN)
#define i64_MAX ((i64) INT64_MAX)



#endif // !_INTEGRATOR_TYPES_H_