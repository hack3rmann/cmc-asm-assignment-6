#ifndef _INTEGRATOR_STRING_H_
#define _INTEGRATOR_STRING_H_

#include "types.h"



/// Strong comparision ordering
typedef enum Ordering {
    Ordering_Less = -1,
    Ordering_Equal = 0,
    Ordering_Greater = 1,
} Ordering;



/// Unowned string, string slice or string view
typedef struct str {
    char* ptr;
    usize len;
} str;

/// Default implementation for [`str`]
#define str_DEFAULT ((str) { .ptr = null, .len = 0 })

/// Creates string slice from string literal
#define str(char_array) ((str) { \
    .ptr = (char_array), \
    .len = sizeof(char_array) - 1 \
})

/// Creates string slice from pointer to null-terminated c-string.
str str_from_ptr(char* ptr);

/// Strong comparator for [`str`]
Ordering str_cmp(str self, str other);

/// == operator for [`str`]
bool str_eq(str self, str other);

/// Cretates subslice for given slice.
/// 
/// # Note
/// 
/// `end` is clamped to slice length.
str str_slice(str self, usize start, usize end);



#endif // !_INTEGRATOR_STRING_H_