#ifndef _INTEGRATOR_STRING_H_
#define _INTEGRATOR_STRING_H_

#include "types.h"



typedef enum Ordering {
    Ordering_Less = -1,
    Ordering_Equal = 0,
    Ordering_Greater = 1,
} Ordering;



typedef struct str {
    char* ptr;
    usize len;
} str;

#define str_DEFAULT ((str) { .ptr = null, .len = 0 })

#define str(char_array) ((str) { \
    .ptr = (char_array), \
    .len = sizeof(char_array) - 1 \
})

str str_from_ptr(char* ptr);

Ordering str_cmp(str self, str other);

bool str_eq(str self, str other);

str str_slice(str self, usize start, usize end);



#endif // !_INTEGRATOR_STRING_H_