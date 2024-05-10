#include <stdlib.h>
#include <stdio.h>
#include "args.h"



ArgsInfo ArgsInfo_new(str program_name, str program_description) {
    ArgsInfo result = {
        .program_name = program_name,
        .program_description = program_description,
        .flags = VecStr_DEFAULT,
        .short_flags = VecChar_DEFAULT,
        .descriptions = VecStr_DEFAULT,
    };

    VecStr_push(&result.flags, str("help"));
    VecChar_push(&result.short_flags, 'h');
    VecStr_push(&result.descriptions, str("Print help"));

    return result;
}

void ArgsInfo_add_flag(
    ArgsInfo* self, str flag, char short_flag, str description
) {
    VecStr_push(&self->flags, flag);
    VecChar_push(&self->short_flags, short_flag);
    VecStr_push(&self->descriptions, description);
}

char ArgsInfo_get_short_flag_from_long(ArgsInfo const* self, str flag) {
    for (usize i = 0; i < self->flags.len; ++i) {
        if (str_eq(flag, self->flags.ptr[i])) {
            return self->short_flags.ptr[i];
        }
    }

    return ArgsInfo_NO_SHORT_FLAG;
}

str ArgsInfo_get_long_flag_from_short(ArgsInfo const* self, char flag) {
    for (usize i = 0; i < self->short_flags.len; ++i) {
        if (self->short_flags.ptr[i] == flag) {
            return self->flags.ptr[i];
        }
    }

    return str_DEFAULT;
}

bool ArgsInfo_contains_flag(ArgsInfo const* self, str flag) {
    for (usize i = 0; i < self->flags.len; ++i) {
        if (str_eq(flag, self->flags.ptr[i])) {
            return true;
        }
    }

    return false;
}

bool ArgsInfo_contains_short_flag(ArgsInfo const* self, char flag) {
    for (usize i = 0; i < self->short_flags.len; ++i) {
        if (self->short_flags.ptr[i] == flag) {
            return true;
        }
    }

    return false;
}

void ArgsInfo_drop(ArgsInfo* self) {
    VecStr_drop(&self->flags);
    VecChar_drop(&self->short_flags);
    VecStr_drop(&self->descriptions);
}

void ArgsInfo_print_help(ArgsInfo const* self) {
    printf("%s\n\n", self->program_description.ptr);
    printf("Usage: %s [OPTIONS]\n\n", self->program_name.ptr);

    printf("Options:\n");

    usize const max_flag_len = ({
        usize max_len = 0;

        for (usize i = 0; i < self->flags.len; ++i) {
            if (max_len < self->flags.ptr[i].len) {
                max_len = self->flags.ptr[i].len;
            }
        }

        max_len;
    });

    for (usize i = 0; i < self->flags.len; ++i) {
        char const short_flag = self->short_flags.ptr[i];
        usize const n_spaces = max_flag_len - self->flags.ptr[i].len + 1;

        if (ArgsInfo_NO_SHORT_FLAG != short_flag) {
            printf("  -%c, ", short_flag);
        } else {
            printf("      ");
        }

        printf("--%s", self->flags.ptr[i].ptr);

        for (usize i = 0; i < n_spaces; ++i) {
            putchar(' ');
        }

        printf("%s\n", self->descriptions.ptr[i].ptr);
    }
}



void Flag_drop(Flag* self) {
    VecStr_drop(&self->values);
}



static void _print_run_program_hint(void) {
    fprintf(stderr, "HELP: Try to run program with --help flag.\n");
}

static void _print_unknown_flag_error(str flag) {
    fprintf(stderr, "ERROR: unknown flag --%s.\n\n", flag.ptr);
    _print_run_program_hint();
}

static void _print_unknown_short_flag_error(char flag) {
    fprintf(stderr, "ERROR: unknown flag -%c.\n\n", flag);
    _print_run_program_hint();
}

Args Args_parse(ArgsInfo const* info, i32 argc, char** argv) {
    if (argc < 1) {
        fprintf(stderr, "Invalid argc = %d < 0\n", argc);
        _print_run_program_hint();
        exit(EXIT_FAILURE);
    }

    usize const n_args = (usize) argc;

    Vec short_flag_series = Vec_new(sizeof(Flag), DropFn_of(Flag));
    Vec long_flags_series = Vec_new(sizeof(Flag), DropFn_of(Flag));
    
    Vec* last_series = null;
    
    // i = 1 to skip program path
    for (usize i = 1; i < n_args; ++i) {
        str const arg = str_from_ptr(argv[i]);

        if (arg.len < 2 || '-' != arg.ptr[0]) {
            if (null == last_series) {
                fprintf(
                    stderr,
                    "ERROR: unexpected value '%s' before any flag.\n",
                    arg.ptr
                );
                exit(EXIT_SUCCESS);
            } else {
                Flag* flag = &Vec_get(Flag, last_series, last_series->len - 1);

                VecStr_push(&flag->values, arg);

                continue;
            }
        }

        bool const flag_is_long = '-' == arg.ptr[1];

        if (flag_is_long) {
            Flag flag = {
                .name = str_slice(arg, 2, usize_MAX),
                .values = VecStr_DEFAULT,
            };

            if (!ArgsInfo_contains_flag(info, flag.name)) {
                _print_unknown_flag_error(flag.name);
                exit(EXIT_FAILURE);
            }

            Vec_push(&long_flags_series, &flag);

            last_series = &long_flags_series;
        } else {
            Flag flag = {
                .name = str_slice(arg, 1, usize_MAX),
                .values = VecStr_DEFAULT,
            };

            for (usize j = 0; j < flag.name.len; ++j) {
                if (!ArgsInfo_contains_short_flag(info, flag.name.ptr[j])) {
                    _print_unknown_short_flag_error(flag.name.ptr[j]);
                    exit(EXIT_FAILURE);
                }
            }

            Vec_push(&short_flag_series, &flag);

            last_series = &short_flag_series;
        }
    }

    return (Args) {
        .short_flags_series = short_flag_series,
        .long_flags_series = long_flags_series,
    };
}

bool Args_contains_help_flag(Args const* self) {
    for (usize i = 0; i < self->short_flags_series.len; ++i) {
        Flag const* flag = &Vec_get(Flag, &self->short_flags_series, i);

        for (usize j = 0; j < flag->name.len; ++j) {
            if ('h' == flag->name.ptr[j]) {
                return true;
            }
        }
    }

    for (usize i = 0; i < self->long_flags_series.len; ++i) {
        Flag const* flag = &Vec_get(Flag, &self->long_flags_series, i);

        if (str_eq(str("help"), flag->name)) {
            return true;
        }
    }

    return false;
}

bool Args_contains_flag(Args const* self, ArgsInfo const* info, str name) {
    if (!ArgsInfo_contains_flag(info, name)) {
        return false;
    }

    for (usize i = 0; i < self->long_flags_series.len; ++i) {
        Flag const* flag = &Vec_get(Flag, &self->long_flags_series, i);

        if (str_eq(name, flag->name)) {
            return true;
        }
    }

    char const short_flag = ArgsInfo_get_short_flag_from_long(info, name);

    for (usize i = 0; i < self->short_flags_series.len; ++i) {
        Flag const* flag = &Vec_get(Flag, &self->short_flags_series, i);

        for (usize j = 0; j < flag->name.len; ++j) {
            if (short_flag == flag->name.ptr[j]) {
                return true;
            }
        }
    }

    return false;
}

bool Args_contains_short_flag(
    Args const* self, ArgsInfo const* info, char short_flag
) {
    if (!ArgsInfo_contains_short_flag(info, short_flag)) {
        return false;
    }

    for (usize i = 0; i < self->short_flags_series.len; ++i) {
        Flag const* flag = &Vec_get(Flag, &self->short_flags_series, i);

        for (usize j = 0; j < flag->name.len; ++j) {
            if (short_flag == flag->name.ptr[j]) {
                return true;
            }
        }
    }

    str const name = ArgsInfo_get_long_flag_from_short(info, short_flag);

    for (usize i = 0; i < self->long_flags_series.len; ++i) {
        Flag const* flag = &Vec_get(Flag, &self->long_flags_series, i);

        if (str_eq(name, flag->name)) {
            return true;
        }
    }

    return false;
}

VecStr const* Args_get_flag_values(
    Args const* self, ArgsInfo const* info, str name
) {
    if (!ArgsInfo_contains_flag(info, name)) {
        return null;
    }

    for (usize i = 0; i < self->long_flags_series.len; ++i) {
        Flag const* flag = &Vec_get(Flag, &self->long_flags_series, i);

        if (str_eq(name, flag->name)) {
            return &flag->values;
        }
    }

    char const short_flag = ArgsInfo_get_short_flag_from_long(info, name);

    for (usize i = 0; i < self->short_flags_series.len; ++i) {
        Flag const* flag = &Vec_get(Flag, &self->short_flags_series, i);

        if (0 == flag->name.len) {
            continue;
        }

        if (short_flag == flag->name.ptr[flag->name.len - 1]) {
            return &flag->values;
        }
    }

    return null;
}

void Args_drop(Args* self) {
    Vec_drop(&self->short_flags_series);
    Vec_drop(&self->long_flags_series);
}