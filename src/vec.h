#ifndef _INTEGRATOR_VEC_H_
#define _INTEGRATOR_VEC_H_

#include "string.h"



/// Growable [`str`] array
typedef struct VecStr {
    str* ptr;
    usize len;
    usize cap;
} VecStr;

/// Default implementation for [`VecStr`]
#define VecStr_DEFAULT ((VecStr) { .ptr = null, .len = 0, .cap = 0 })

/// Creates [`VecStr`] with allocated capacity.
VecStr VecStr_with_capacity(usize cap);

/// Adds new element to [`VecStr`]
void VecStr_push(VecStr* self, str value);

/// Drop implementation for [`VecStr`]
void VecStr_drop(VecStr* self);



/// Glowable [`char`] array.
typedef struct VecChar {
    char* ptr;
    usize len;
    usize cap;
} VecChar;

/// Default implementation for [`VecChar`]
#define VecChar_DEFAULT ((VecChar) { .ptr = null, .len = 0, .cap = 0 })

/// Creates [`VecChar`] with allocated capacity.
VecChar VecChar_with_capacity(usize cap);

/// Adds new element to [`VecChar`]
void VecChar_push(VecChar* self, char value);

/// Drop implementation for [`VecChar`]
void VecChar_drop(VecChar* self);



/// Glowable anytype array
typedef struct Vec {
    AddrMut ptr;
    usize len;
    usize cap;
    usize elem_size;
    DropFn drop;
} Vec;

/// Retrieves i-th element of vector
#define Vec_get(type, self, index) \
    (*(type*) ((self)->ptr + (self)->elem_size * (index)))

/// Creates new [`Vec`]
Vec Vec_new(usize elem_size, DropFn drop);

/// Creates [`Vec`] with allocated capacity.
Vec Vec_with_capacity(usize cap, usize elem_size, DropFn drop);

/// Adds new element to [`Vec`]
void Vec_push(Vec* self, AddrMut value_ptr);

/// Drop implementation for [`Vec`]
void Vec_drop(Vec* self);



#endif // !_INTEGRATOR_VEC_H_