#include <stdio.h>
#include <assert.h>

#include "util.h"



void DropFn_noop(AddrMut __attribute__((unused)) value) { }

usize usize_parse(str value) {
    usize result;

    assert(1 == sscanf(value.ptr, "%zu", &result) && "failed to parse usize\n");

    return result;
}

f32 f32_parse(str value) {
    f32 result;

    assert(1 == sscanf(value.ptr, "%f", &result) && "failed to parse f32\n");

    return result;
}