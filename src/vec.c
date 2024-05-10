#include <stdlib.h>
#include <string.h>

#include "vec.h"



VecStr VecStr_with_capacity(usize cap) {
    return (VecStr) {
        .ptr = malloc(sizeof(str) * cap),
        .len = 0,
        .cap = cap,
    };
}

void VecStr_push(VecStr* self, str value) {
    if (0 == self->cap) {
        self->cap = 1;
        self->ptr = malloc(sizeof(*self->ptr) * self->cap);
    } else if (self->cap == self->len) {
        self->cap += self->cap / 2 + 1;
        self->ptr = realloc(self->ptr, sizeof(*self->ptr) * self->cap);
    }

    self->ptr[self->len++] = value;
}

void VecStr_drop(VecStr* self) {
    free(self->ptr);
}



VecVecStr VecVecStr_with_capacity(usize cap) {
    return (VecVecStr) {
        .ptr = malloc(sizeof(VecStr) * cap),
        .len = 0,
        .cap = cap,
    };
}

void VecVecStr_push(VecVecStr* self, VecStr value) {
    if (0 == self->cap) {
        self->cap = 1;
        self->ptr = malloc(sizeof(*self->ptr) * self->cap);
    } else if (self->cap == self->len) {
        self->cap += self->cap / 2 + 1;
        self->ptr = realloc(self->ptr, sizeof(*self->ptr) * self->cap);
    }

    self->ptr[self->len++] = value;
}

void VecVecStr_drop(VecVecStr* self) {
    for (usize i = 0; i < self->len; ++i) {
        VecStr_drop(self->ptr + i);
    }

    free(self->ptr);

    self->len = 0;
    self->cap = 0;
}



VecChar VecChar_with_capacity(usize cap) {
    return (VecChar) {
        .ptr = malloc(sizeof(char) * cap),
        .len = 0,
        .cap = cap,
    };
}

void VecChar_push(VecChar* self, char value) {
    if (0 == self->cap) {
        self->cap = 16;
        self->ptr = malloc(sizeof(*self->ptr) * self->cap);
    } else if (self->cap == self->len) {
        self->cap += self->cap / 2 + 1;
        self->ptr = realloc(self->ptr, sizeof(*self->ptr) * self->cap);
    }

    self->ptr[self->len++] = value;
}

void VecChar_drop(VecChar* self) {
    free(self->ptr);
}



Vec Vec_new(usize elem_size, DropFn drop) {
    return (Vec) {
        .ptr = null,
        .len = 0,
        .cap = 0,
        .elem_size = elem_size,
        .drop = drop,
    };
}

Vec Vec_with_capacity(usize cap, usize elem_size, DropFn drop) {
    return (Vec) {
        .ptr = malloc(elem_size * cap),
        .len = 0,
        .cap = cap,
        .elem_size = elem_size,
        .drop = drop,
    };
}

void Vec_push(Vec* self, AddrMut value_ptr) {
    if (0 == self->cap) {
        self->cap = 1;
        self->ptr = malloc(self->elem_size * self->cap);
    } else if (self->len == self->cap) {
        self->cap += self->cap / 2 + 1;
        self->ptr = realloc(self->ptr, self->elem_size * self->cap);
    }

    memcpy(
        self->ptr + self->elem_size * self->len,
        value_ptr,
        self->elem_size
    );

    self->len += 1;
}

void Vec_drop(Vec* self) {
    for (usize i = 0; i < self->len; ++i) {
        (self->drop)(self->ptr + self->elem_size * i);
    }

    free(self->ptr);

    self->len = 0;
    self->cap = 0;
}