#ifndef _INTEGRATOR_VEC_H_
#define _INTEGRATOR_VEC_H_

#include "string.h"



typedef struct VecStr {
    str* ptr;
    usize len;
    usize cap;
} VecStr;

#define VecStr_DEFAULT ((VecStr) { .ptr = null, .len = 0, .cap = 0 })

VecStr VecStr_with_capacity(usize cap);

void VecStr_push(VecStr* self, str value);

void VecStr_drop(VecStr* self);



typedef struct VecVecStr {
    VecStr* ptr;
    usize len;
    usize cap;
} VecVecStr;

#define VecVecStr_DEFAULT ((VecVecStr) { .ptr = null, .len = 0, .cap = 0 })

VecVecStr VecVecStr_with_capacity(usize cap);

void VecVecStr_push(VecVecStr* self, VecStr value);

void VecVecStr_drop(VecVecStr* self);



typedef struct VecChar {
    char* ptr;
    usize len;
    usize cap;
} VecChar;

#define VecChar_DEFAULT ((VecChar) { .ptr = null, .len = 0, .cap = 0 })

VecChar VecChar_with_capacity(usize cap);

void VecChar_push(VecChar* self, char value);

void VecChar_drop(VecChar* self);



typedef struct Vec {
    AddrMut ptr;
    usize len;
    usize cap;
    usize elem_size;
    DropFn drop;
} Vec;

#define Vec_get(type, self, index) \
    (*(type*) ((self)->ptr + (self)->elem_size * (index)))

Vec Vec_new(usize elem_size, DropFn drop);

Vec Vec_with_capacity(usize cap, usize elem_size, DropFn drop);

void Vec_push(Vec* self, AddrMut value_ptr);

void Vec_drop(Vec* self);



#endif // !_INTEGRATOR_VEC_H_