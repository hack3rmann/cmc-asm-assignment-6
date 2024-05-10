#include <string.h>

#include "string.h"
#include "util.h"



str str_from_ptr(char* ptr) {
    return (str) {
        .ptr = ptr,
        .len = strlen(ptr),
    };
}

Ordering str_cmp(str self, str other) {
    return strcmp(self.ptr, other.ptr);
}

bool str_eq(str self, str other) {
    return Ordering_Equal == str_cmp(self, other);
}

str str_slice(str self, usize start, usize end) {
    if (end < start) {
        return self;
    } else if (start == end) {
        return str_DEFAULT;
    } else {
        end = min(end, self.len);

        return (str) {
            .ptr = self.ptr + start,
            .len = end - start,
        };
    }
}