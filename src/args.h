#ifndef _INTEGRATOR_ARGS_H_
#define _INTEGRATOR_ARGS_H_

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



typedef struct VecStr {
    str* ptr;
    usize len;
    usize cap;
} VecStr;

#define VecStr_DEFAULT ((VecStr) { .ptr = null, .len = 0, .cap = 0 })

VecStr VecStr_with_capacity(usize cap);

void VecStr_push(VecStr* self, str value);

void VecStr_drop(VecStr* self);



typedef struct VecChar {
    char* ptr;
    usize len;
    usize cap;
} VecChar;

#define VecChar_DEFAULT ((VecChar) { .ptr = null, .len = 0, .cap = 0 })

VecChar VecChar_with_capacity(usize cap);

void VecChar_push(VecChar* self, char value);

void VecChar_drop(VecChar* self);



typedef struct ArgsInfo {
    str program_name;
    str program_description;
    VecStr flags;
    VecChar short_flags;
    VecStr descriptions;
} ArgsInfo;

#define ArgsInfo_HELP_FLAG str("help")
#define ArgsInfo_NO_SHORT_FLAG ((char) 0)

ArgsInfo ArgsInfo_new(str program_name, str program_description);

void ArgsInfo_add_flag(
    ArgsInfo* self, str flag, char short_flag, str description
);

bool ArgsInfo_contains_flag(ArgsInfo const* self, str flag);

bool ArgsInfo_contains_short_flag(ArgsInfo const* self, char flag);

char ArgsInfo_get_short_flag_from_long(ArgsInfo const* self, str flag);

str ArgsInfo_get_long_flag_from_short(ArgsInfo const* self, char flag);

void ArgsInfo_drop(ArgsInfo* self);

void ArgsInfo_print_help(ArgsInfo const* self);



typedef struct Args {
    VecStr flags;
    VecChar short_flags;
} Args;

Args Args_parse(ArgsInfo const* info, i32 argc, char** argv);

bool Args_contains_help_flag(Args const* self);

bool Args_contains_flag(Args const* self, ArgsInfo const* info, str flag);

bool Args_contains_short_flag(Args const* self, ArgsInfo const* info, char flag);

void Args_drop(Args* self);



#endif // !_INTEGRATOR_ARGS_H_