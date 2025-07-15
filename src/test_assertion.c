/**
 * @file test_assertion.c
 * @brief Structured unit test for ASSERTION macro.
 */

#include <stdio.h>
#include <stdbool.h>
#include "lift_assert.h"

/**
 * @brief Runs a simple test for the LIFT_ASSERT macro.
 */
void LiftAssert_test(void)
{
    // Test assertion
    LIFT_ASSERT(true);   // will pass
    LIFT_ASSERT(false);  // will fail if ENABLE_ASSERT is 1

    printf("... you would never reach it :)\n");
}
