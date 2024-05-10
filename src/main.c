#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "types.h"
#include "args.h"


#define f32_EPS 2e-4

typedef f32 (*SmoothFn)(f32);


f32 f32_lerp(f32 left, f32 right, f32 mix) {
    return (1.0 - mix) * left + mix * right;
}


typedef struct RootFindCfg {
    f32 eps;
    f32 segment_start;
    f32 segment_end;
} RootFindCfg;

#define RootFindCfg_DEFAULT ((RootFindCfg) { \
    .eps = f32_EPS, \
    .segment_start = -1.0 / f32_EPS, \
    .segment_end = 1.0 / f32_EPS, \
})


typedef struct Root {
    f32 value;
    usize n_steps;
} Root;

/// Estimates the root of the f(x) = g(x) equation.
Root Root_of(
    SmoothFn f, SmoothFn g, SmoothFn dfdx, SmoothFn dgdx, RootFindCfg const cfg
) {
    f32 a = cfg.segment_start;
    f32 b = cfg.segment_end;
    usize n_steps = 0;

    while (fabs(a - b) >= cfg.eps) {
        f32 const funcs_diff_a = f(a) - g(a);
        f32 const funcs_diff_b = f(b) - g(b);
        f32 const primes_diff_b = dfdx(b) - dgdx(b);

        a -= (b - a) / (funcs_diff_b - funcs_diff_a) * funcs_diff_a;
        b -= funcs_diff_b / primes_diff_b;

        n_steps += 1;
    }

    return (Root) {
        .value = 0.5 * (b - a),
        .n_steps = n_steps,
    };
}


f32 estimate_integral(
    SmoothFn f, f32 segment_left, f32 segment_right, f32 dx
) {
    usize const n_steps = 1 + (usize) ((segment_right - segment_left) / dx);

    f32 sum = 0.0;

    for (usize i = 0; i < n_steps - 1; ++i) {
        f32 const left_point = f32_lerp(
            segment_left,
            segment_right,
            (f32) i / (f32) (n_steps - 1)
        );

        f32 const right_point = f32_lerp(
            segment_left,
            segment_right,
            (f32) (i + 1) / (f32) (n_steps - 1)
        );

        sum += 0.5 * (f(left_point) + f(right_point))
            * (right_point - left_point);
    }

    return sum;
}


i32 main(i32 argc, char** argv) {
    ArgsInfo args_info = ArgsInfo_new(
        str("integrator"),
        str("integrator is a program that calculates area bounded "
            "by f1(x) = ln(x), f2(x) = -2x + 14, f3(x) = 1 / (2 - x) + 6.")
    );

    ArgsInfo_add_flag(
        &args_info,
        str("intersection"),
        'i',
        str("Prints x values of functions intersection points")
    );

    ArgsInfo_add_flag(
        &args_info,
        str("count"),
        'c',
        str("Prints number of iterations are performed "
            "to estimate intersection points")
    );

    Args args = Args_parse(&args_info, argc, argv);

    if (Args_contains_help_flag(&args)) {
        ArgsInfo_print_help(&args_info);
        Args_drop(&args);
        ArgsInfo_drop(&args_info);
        return EXIT_SUCCESS;
    }

    VecStr const* intersection_values
        = Args_get_flag_values(&args, &args_info, str("intersection"));

    if (null != intersection_values) {
        printf("intersection flag values readed:\n");

        for (usize i = 0; i < intersection_values->len; ++i) {
            printf("  - %s\n", intersection_values->ptr[i].ptr);
        }
    }

    VecStr const* count_values
        = Args_get_flag_values(&args, &args_info, str("count"));

    if (null != count_values) {
        printf("count flag values readed:\n");

        for (usize i = 0; i < count_values->len; ++i) {
            printf("  - %s\n", count_values->ptr[i].ptr);
        }
    }

    bool const do_print_points
        = Args_contains_flag(&args, &args_info, str("intersection"));

    bool const do_print_n_iterations
        = Args_contains_flag(&args, &args_info, str("count"));

    Args_drop(&args);
    ArgsInfo_drop(&args_info);

    return EXIT_SUCCESS;
}