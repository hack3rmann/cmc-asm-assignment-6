//! Functions to be integrated.
//! 
//! # Note
//! 
//! Implementation is in [`functions.asm`].

#ifndef _INTEGRATOR_FUNCTIONS_H_
#define _INTEGRATOR_FUNCTIONS_H_

#include "types.h"



/// Smooth function f(x)
typedef f32 (*SmoothFn)(f32);



/// Function and its first and second derivatives
typedef struct DiffFunc {
    SmoothFn f;
    SmoothFn dfdx;
    SmoothFn d2fdx2;
} DiffFunc;



/// Calculates ln(x)
extern f32 f1(f32 value);

/// Calculates (d/dx) ln(x) = 1/x
extern f32 df1dx(f32 value);

/// Calculates (d2/dx2) ln(x) = -1 / x**2
extern f32 d2f1dx2(f32 value);

/// Calculates f2(x) = -2x + 14
extern f32 f2(f32 value);

/// Calculates (d/dx) f2(x) = -2
extern f32 df2dx(f32 value);

/// Calculates (d2/dx2) d2(x) = 0
extern f32 d2f2dx2(f32 value);

/// Calculates f3(x) = 1 / (2 - x) + 6
extern f32 f3(f32 value);

/// Calculates (d/dx) f3(x) = 1 / (2 - x)**2
extern f32 df3dx(f32 value);

/// Calculates (d2/dx2) f3(x) = 2 / (2 - x)**3
extern f32 d2f3dx2(f32 value);



#define diff_f1 ((DiffFunc) { .f = f1, .dfdx = df1dx, .d2fdx2 = d2f1dx2 })
#define diff_f2 ((DiffFunc) { .f = f2, .dfdx = df2dx, .d2fdx2 = d2f2dx2 })
#define diff_f3 ((DiffFunc) { .f = f3, .dfdx = df3dx, .d2fdx2 = d2f3dx2 })



#endif // !_INTEGRATOR_FUNCTIONS_H_ 