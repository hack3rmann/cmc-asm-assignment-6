//! Functions to be integrated.
//! 
//! # Note
//! 
//! Implementation is in [`functions.asm`].

#ifndef _INTEGRATOR_FUNCTIONS_H_
#define _INTEGRATOR_FUNCTIONS_H_

#include "types.h"



extern f32 f1(f32 value);
extern f32 f1_prime(f32 value);
extern f32 f2(f32 value);
extern f32 f2_prime(f32 value);
extern f32 f3(f32 value);
extern f32 f3_prime(f32 value);



#endif // !_INTEGRATOR_FUNCTIONS_H_