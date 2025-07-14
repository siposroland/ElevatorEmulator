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
void Test_lift_assert(void)
{
    // Example assertion to demonstrate the assert mechanism
    int x = 5;

    // Test assertion
    LIFT_ASSERT(x == 5);   // ✅ will pass
    LIFT_ASSERT(x == 10);  // ❌ will fail if ENABLE_ASSERT is 1

    printf("... you would never reach it :)\n");
}
