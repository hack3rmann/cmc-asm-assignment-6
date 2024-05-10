#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "types.h"
#include "args.h"
#include "functions.h"


#define f32_EPS 1e-3



/// Smooth function f(x)
typedef f32 (*SmoothFn)(f32);



/// Linear interpilation for [`f32`]
f32 f32_lerp(f32 left, f32 right, f32 mix) {
    return (1.0 - mix) * left + mix * right;
}



typedef enum DerivativesSign {
    DerivativesSign_Same,
    DerivativesSign_Different,
} DerivativesSign;



/// Root finding configuration
typedef struct RootFindCfg {
    f32 eps;
    f32 segment_start;
    f32 segment_end;
    DerivativesSign derivetives_sign;
} RootFindCfg;

#define RootFindCfg_DEFAULT ((RootFindCfg) { \
    .eps = f32_EPS, \
    .segment_start = -1.0 / f32_EPS, \
    .segment_end = 1.0 / f32_EPS, \
})



/// Information about esimated root.
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
        n_steps += 1;

        f32 const funcs_diff_a = f(a) - g(a);
        f32 const funcs_diff_b = f(b) - g(b);
        f32 const primes_diff_a = dfdx(a) - dgdx(a);
        f32 const primes_diff_b = dfdx(b) - dgdx(b);

        switch (cfg.derivetives_sign) {
        case DerivativesSign_Same: {
            a -= (b - a) / (funcs_diff_b - funcs_diff_a) * funcs_diff_a;
            b -= funcs_diff_b / primes_diff_b;
        } break;

        case DerivativesSign_Different: {
            a -= funcs_diff_a / primes_diff_a;
            b -= (b - a) / (funcs_diff_b - funcs_diff_a) * funcs_diff_b;
        } break;
        }

        printf("a = %f, b = %f\n", a, b);
    }

    return (Root) {
        .value = 0.5 * (a + b),
        .n_steps = n_steps,
    };
}



/// Integram esimator
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

    Root const intersection_12
        = Root_of(f1, f2, f1_prime, f2_prime, (RootFindCfg) {
            .eps = f32_EPS,
            .segment_start = 6.0,
            .segment_end = 6.5,
            .derivetives_sign = DerivativesSign_Different,
        });
    
    Root const intersection_23
        = Root_of(f2, f3, f2_prime, f3_prime, (RootFindCfg) {
            .eps = f32_EPS,
            .segment_start = 4.0,
            .segment_end = 4.5,
            .derivetives_sign = DerivativesSign_Different,
        });

    Root const intersection_13
        = Root_of(f1, f3, f1_prime, f3_prime, (RootFindCfg) {
            .eps = f32_EPS,
            .segment_start = 2.1,
            .segment_end = 2.2,
            .derivetives_sign = DerivativesSign_Same,
        });

    if (Args_contains_flag(&args, &args_info, str("intersection"))) {
        printf("Estimated functions intersections:\n");
        printf("  f1 with f2: x = %f\n", intersection_12.value);
        printf("  f2 with f3: x = %f\n", intersection_23.value);
        printf("  f1 with f3: x = %f\n", intersection_13.value);
    }

    if (Args_contains_flag(&args, &args_info, str("count"))) {
        printf("Required numbers of iterations to estimate intersections:\n");
        printf("  f1 with f2: %zu\n", intersection_12.n_steps);
        printf("  f2 with f3: %zu\n", intersection_23.n_steps);
        printf("  f1 with f3: %zu\n", intersection_13.n_steps);
    }

    Args_drop(&args);
    ArgsInfo_drop(&args_info);

    return EXIT_SUCCESS;
}