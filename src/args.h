#ifndef _INTEGRATOR_ARGS_H_
#define _INTEGRATOR_ARGS_H_

#include "types.h"
#include "string.h"
#include "vec.h"



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



typedef struct Flag {
    str name;
    VecStr values;
} Flag;

#define Flag_DEFAULT ((Flag) { \
    .name = str_DEFAULT, \
    .value = VecStr_DEFAULT, \
})

void Flag_drop(Flag* self);



typedef struct Args {
    Vec short_flags_series;
    Vec long_flags_series;
} Args;

Args Args_parse(ArgsInfo const* info, i32 argc, char** argv);

bool Args_contains_help_flag(Args const* self);

bool Args_contains_flag(Args const* self, ArgsInfo const* info, str flag);

bool Args_contains_short_flag(
    Args const* self, ArgsInfo const* info, char flag
);

VecStr const* Args_get_flag_values(
    Args const* self, ArgsInfo const* info, str flag
);

void Args_drop(Args* self);



#endif // !_INTEGRATOR_ARGS_H_