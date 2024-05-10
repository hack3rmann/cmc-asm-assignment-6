#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "args.h"



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



static void _print_run_program_hint(void) {
    fprintf(stderr, "Try run program with --help flag.\n");
}

static void _print_unknown_flag_error(str flag) {
    fprintf(stderr, "ERROR: unknown flag --%s\n\n", flag.ptr);
    _print_run_program_hint();
}

static void _print_unknown_short_flag_error(char flag) {
    fprintf(stderr, "ERROR: unknown flag -%c\n\n", flag);
    _print_run_program_hint();
}

Args Args_parse(ArgsInfo const* info, i32 argc, char** argv) {
    if (argc < 0) {
        fprintf(stderr, "ERROR: Invalid argc %d < 0\n", argc);
        exit(EXIT_FAILURE);
    }

    usize const n_args = (usize) argc;

    VecStr flags = VecStr_DEFAULT;
    VecChar short_flags = VecChar_DEFAULT;

    // Skip program name argument
    for (usize i = 1; i < n_args; ++i) {
        usize const arg_len = strlen(argv[i]);

        if (arg_len < 2 || '-' != argv[i][0]) {
            fprintf(stderr, "ERROR: Invalid flag %s\n\n", argv[i]);
            _print_run_program_hint();
            exit(EXIT_FAILURE);
        }

        bool const flag_is_long = '-' == argv[i][1];

        if (flag_is_long) {
            str const flag = str_from_ptr(argv[i] + 2);

            if (!ArgsInfo_contains_flag(info, flag)) {
                _print_unknown_flag_error(flag);
                exit(EXIT_FAILURE);
            }

            VecStr_push(&flags, flag);
        } else {
            str const short_flag_string = str_from_ptr(argv[i] + 1);

            for (usize j = 0; j < short_flag_string.len; ++j) {
                if (!ArgsInfo_contains_short_flag(info, short_flag_string.ptr[j])) {
                    _print_unknown_short_flag_error(short_flag_string.ptr[j]);
                    exit(EXIT_FAILURE);
                }

                VecChar_push(&short_flags, short_flag_string.ptr[j]);
            }
        }
    }

    return (Args) {
        .flags = flags,
        .short_flags = short_flags,
    };
}

bool Args_contains_help_flag(Args const* self) {
    for (usize i = 0; i < self->short_flags.len; ++i) {
        if ('h' == self->short_flags.ptr[i]) {
            return true;
        }
    }

    for (usize i = 0; i < self->flags.len; ++i) {
        if (str_eq(str("help"), self->flags.ptr[i])) {
            return true;
        }
    }

    return false;
}

bool Args_contains_flag(Args const* self, ArgsInfo const* info, str flag) {
    if (!ArgsInfo_contains_flag(info, flag)) {
        return false;
    }

    for (usize i = 0; i < self->flags.len; ++i) {
        if (str_eq(flag, self->flags.ptr[i])) {
            return true;
        }
    }

    char const short_flag = ArgsInfo_get_short_flag_from_long(info, flag);

    for (usize i = 0; i < self->short_flags.len; ++i) {
        if (short_flag == self->short_flags.ptr[i]) {
            return true;
        }
    }

    return false;
}

bool Args_contains_short_flag(
    Args const* self, ArgsInfo const* info, char const short_flag
) {
    if (!ArgsInfo_contains_short_flag(info, short_flag)) {
        return false;
    }

    for (usize i = 0; i < self->short_flags.len; ++i) {
        if (short_flag == self->short_flags.ptr[i]) {
            return true;
        }
    }

    str const flag = ArgsInfo_get_long_flag_from_short(info, short_flag);

    for (usize i = 0; i < self->flags.len; ++i) {
        if (str_eq(flag, self->flags.ptr[i])) {
            return true;
        }
    }

    return false;
}

void Args_drop(Args* self) {
    VecStr_drop(&self->flags);
    VecChar_drop(&self->short_flags);
}