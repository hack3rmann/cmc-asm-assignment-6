#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "types.h"
#include "args.h"
#include "functions.h"
#include "util.h"


#define f32_EPS 1e-3



/// Linear interpilation for [`f32`]
f32 f32_lerp(f32 left, f32 right, f32 mix) {
    return (1.0 - mix) * left + mix * right;
}



/// Root finding configuration
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



/// Information about esimated root.
typedef struct Root {
    f32 value;
    usize n_steps;
} Root;

/// Estimates the root of the f(x) = g(x) equation.
Root Root_of(
    DiffFunc f, DiffFunc g, RootFindCfg const cfg
) {
    f32 a = cfg.segment_start;
    f32 b = cfg.segment_end;
    usize n_steps = 0;

    while (fabs(a - b) >= 2.0 * cfg.eps) {
        n_steps += 1;

        f32 const funcs_diff_a = f.f(a) - g.f(a);
        f32 const funcs_diff_b = f.f(b) - g.f(b);
        f32 const primes_diff_a = f.dfdx(a) - g.dfdx(a);
        f32 const primes_diff_b = f.dfdx(b) - g.dfdx(b);
        f32 const primes2_diff_a = f.d2fdx2(a) - g.d2fdx2(a);
        f32 const primes2_diff_b = f.d2fdx2(b) - g.d2fdx2(b);

        if (primes_diff_a * primes2_diff_a < 0.0) {
            a -= funcs_diff_a * (a - b) / (funcs_diff_a - funcs_diff_b);
        } else {
            a -= funcs_diff_a / primes_diff_a;
        }

        if (primes_diff_b * primes2_diff_b < 0.0) {
            b -= funcs_diff_b * (a - b) / (funcs_diff_a - funcs_diff_b);
        } else {
            b -= funcs_diff_b / primes2_diff_b;
        }
    }

    return (Root) {
        .value = 0.5 * (a + b),
        .n_steps = n_steps,
    };
}



/// Integral esimator
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
        str("Prints x values of functions intersection points"),
        FlagValues_EMPTY
    );

    ArgsInfo_add_flag(
        &args_info,
        str("count"),
        'c',
        str("Prints number of iterations are performed "
            "to estimate intersection points"),
        FlagValues_EMPTY
    );

    static str intersection_values_names[] = {
        str("F"), str("G"), str("LEFT"), str("RIGHT"), str("EPS"),
    };

    ArgsInfo_add_flag(
        &args_info,
        str("test-intersections"),
        ArgsInfo_NO_SHORT_FLAG,
        str("Tests function intersection algorythm. "
            "F ang G are 1, 2, 3, F != G - functions to test. LEFT and RIGHT "
            "are left and right segment boundaries to find root on. "
            "EPS != 0 is accuracy of the algorithm"),
        (FlagValues) {
            .arg_names = intersection_values_names,
            .n_arg_names = 5,
        }
    );

    static str integration_values_names[] = {
        str("F"), str("LEFT"), str("RIGHT"), str("EPS"),
    };

    ArgsInfo_add_flag(
        &args_info,
        str("test-integration"),
        ArgsInfo_NO_SHORT_FLAG,
        str("Tests function integration algorythm. "
            "F is 1, 2, 3 - function to integrate. LEFT and RIGHT "
            "are left and right boundaries of the segment to integrate on. "
            "EPS != 0 is accuracy of the algorithm"),
        (FlagValues) {
            .arg_names = integration_values_names,
            .n_arg_names = 4,
        }
    );

    Args args = Args_parse(&args_info, argc, argv);

    if (Args_contains_help_flag(&args)) {
        ArgsInfo_print_help(&args_info);
        Args_drop(&args);
        ArgsInfo_drop(&args_info);
        return EXIT_SUCCESS;
    }

    Root const intersection_12
        = Root_of(diff_f1, diff_f2, (RootFindCfg) {
            .eps = f32_EPS,
            .segment_start = 6.0,
            .segment_end = 6.5,
        });
    
    Root const intersection_23
        = Root_of(diff_f2, diff_f3, (RootFindCfg) {
            .eps = f32_EPS,
            .segment_start = 4.0,
            .segment_end = 4.5,
        });

    Root const intersection_13
        = Root_of(diff_f1, diff_f3, (RootFindCfg) {
            .eps = f32_EPS,
            .segment_start = 2.1,
            .segment_end = 2.2,
        });

    VecStr const* test_intersection_values
        = Args_get_flag_values(&args, &args_info, str("test-intersections"));
    
    if (null != test_intersection_values) {
        assert(5 == test_intersection_values->len && "5 arguments expected");

        usize const f_index = usize_parse(test_intersection_values->ptr[0]) - 1;
        usize const g_index = usize_parse(test_intersection_values->ptr[1]) - 1;
        f32 const left = f32_parse(test_intersection_values->ptr[2]);
        f32 const right = f32_parse(test_intersection_values->ptr[3]);
        f32 const eps = f32_parse(test_intersection_values->ptr[4]);

        assert(f_index < 3
            && g_index < 3
            && f_index != g_index
            && "F and G should be 1, 2, 3 and F != G");
        
        assert(left <= right && "LEFT should be not greater than RIGH");

        assert(eps > 0.0 && "EPS should be greater than zero");

        DiffFunc const functions[] = { diff_f1, diff_f2, diff_f3 };

        Root const root = Root_of(
            functions[f_index],
            functions[g_index],
            (RootFindCfg) {
                .eps = eps,
                .segment_start = left,
                .segment_end = right,
            }
        );

        printf(
            "Found root x = %f with %zu iterations.", root.value, root.n_steps
        );

        Args_drop(&args);
        ArgsInfo_drop(&args_info);

        return EXIT_SUCCESS;
    }

    VecStr const* test_integration_values
        = Args_get_flag_values(&args, &args_info, str("test-integration"));
    
    if (null != test_integration_values) {
        assert(4 == test_integration_values->len && "4 arguments expected");

        usize const f_index = usize_parse(test_integration_values->ptr[0]) - 1;
        f32 const left = f32_parse(test_integration_values->ptr[1]);
        f32 const right = f32_parse(test_integration_values->ptr[2]);
        f32 const eps = f32_parse(test_integration_values->ptr[3]);

        assert(f_index < 3 && "F should be one of 1, 2, 3");
        assert(left <= right && "LEFT should be not greater than RIGHT");
        assert(eps > 0.0 && "EPS should be greater than zero");

        SmoothFn const functions[] = { f1, f2, f3 };

        f32 const area
            = estimate_integral(functions[f_index], left, right, eps);

        printf("Estimated area: %f\n", area);

        Args_drop(&args);
        ArgsInfo_drop(&args_info);

        return EXIT_SUCCESS;
    }

    if (Args_contains_flag(&args, &args_info, str("intersection"))) {
        f32 const x12 = intersection_12.value;
        f32 const x23 = intersection_23.value;
        f32 const x13 = intersection_13.value;

        printf("Estimated functions intersections:\n");
        printf("  f1 with f2: x = %f, y = %f\n", x12, f2(x12));
        printf("  f2 with f3: x = %f, y = %f\n", x23, f2(x23));
        printf("  f1 with f3: x = %f, y = %f\n", x13, f3(x13));
    }

    if (Args_contains_flag(&args, &args_info, str("count"))) {
        printf("Required numbers of iterations to estimate intersections:\n");
        printf("  f1 with f2: %zu\n", intersection_12.n_steps);
        printf("  f2 with f3: %zu\n", intersection_23.n_steps);
        printf("  f1 with f3: %zu\n", intersection_13.n_steps);
    }

    f32 const area1 = estimate_integral(
        f1, intersection_13.value, intersection_12.value, f32_EPS
    );

    f32 const area2 = estimate_integral(
        f2, intersection_23.value, intersection_12.value, f32_EPS
    );

    f32 const area3 = estimate_integral(
        f3, intersection_13.value, intersection_23.value, f32_EPS
    );

    f32 const area = area3 + area2 - area1;

    printf("Estimated area: %f\n", area);

    Args_drop(&args);
    ArgsInfo_drop(&args_info);

    return EXIT_SUCCESS;
}