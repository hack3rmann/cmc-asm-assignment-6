#include <stdlib.h>
#include <assert.h>
#include "functions.h"

#define f32_EPS 2e-4



int main(void) {
    assert(f1(2.71828182) - 1.0 < f32_EPS);

    return EXIT_SUCCESS;
}