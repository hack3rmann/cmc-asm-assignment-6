#ifndef _INTEGRATOR_ARGS_H_
#define _INTEGRATOR_ARGS_H_

#include "types.h"
#include "string.h"
#include "vec.h"



/// CLI argument information needed to parse them.
typedef struct ArgsInfo {
    str program_name;
    str program_description;
    VecStr flags;
    VecChar short_flags;
    VecStr descriptions;
} ArgsInfo;

/// str("help") shorthand
#define ArgsInfo_HELP_FLAG str("help")

/// Sentinel value for the empty short variant of the long flag
#define ArgsInfo_NO_SHORT_FLAG ((char) 0)

/// Constructs new [`ArgsInfo`]
ArgsInfo ArgsInfo_new(str program_name, str program_description);

/// Adds a flag with description to parser info.
/// 
/// # Note
/// 
/// `short_flag` can be set to `ArgsInfo_NO_SHORT_FLAG` to exclude
/// it from parsing.
void ArgsInfo_add_flag(
    ArgsInfo* self, str flag, char short_flag, str description
);

/// Checks if long flag is in parser info.
bool ArgsInfo_contains_flag(ArgsInfo const* self, str flag);

/// Checks if short flag is in parser info.
bool ArgsInfo_contains_short_flag(ArgsInfo const* self, char flag);

/// Converts long flag to its short version.
/// 
/// # Note
/// 
/// It will return `ArgsInfo_NO_SHORT_FLAG` if either short flag is not set
/// or long version is not in parser info.
char ArgsInfo_get_short_flag_from_long(ArgsInfo const* self, str flag);

/// Converts short flag to its long version.
/// 
/// # Note
/// 
/// It will return `str_DEFAULT` if short flag is not set.
str ArgsInfo_get_long_flag_from_short(ArgsInfo const* self, char flag);

/// Drop implementation for [`ArgsInfo`]
void ArgsInfo_drop(ArgsInfo* self);

/// Prints help message based on all flags and their descriptions.
void ArgsInfo_print_help(ArgsInfo const* self);



/// Single parsed flag with its values or a group of short flags with
/// values of the last.
typedef struct Flag {
    str name;
    VecStr values;
} Flag;

/// Default values for [`Flag`]
#define Flag_DEFAULT ((Flag) { \
    .name = str_DEFAULT, \
    .value = VecStr_DEFAULT, \
})

/// Drop implementation for [`Flag`]
void Flag_drop(Flag* self);



/// All parsed arguments from command line.
typedef struct Args {
    Vec short_flags_series;
    Vec long_flags_series;
} Args;

/// Parses CLI arguments into [`Args`]
/// 
/// # Note
/// 
/// Will panic if arguments are wrong.
Args Args_parse(ArgsInfo const* info, i32 argc, char** argv);

/// Checks containment of --help flag.
bool Args_contains_help_flag(Args const* self);

/// Check containment of long `flag` in parsed arguments
/// in its long or short version
bool Args_contains_flag(Args const* self, ArgsInfo const* info, str flag);

/// Check containment of short `flag` in parsed arguments
/// in its long or short version
bool Args_contains_short_flag(
    Args const* self, ArgsInfo const* info, char flag
);

/// Retrieves flag's values, if any
VecStr const* Args_get_flag_values(
    Args const* self, ArgsInfo const* info, str flag
);

/// Drop implementation for [`Args`]
void Args_drop(Args* self);



#endif // !_INTEGRATOR_ARGS_H_