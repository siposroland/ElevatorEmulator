/**
 * @file main.c
 * @brief Entry point for the elevator controller emulator.
 */

#include <stdio.h>
#include "version.h"

#define ENABLE_ASSERT 1  // <-- Toggle this to 0 to disable

#include "lift_assert.h"

/**
 * @brief Main function to test the build system.
 * 
 * @return int Returns 0 on successful execution.
 */
int main(void)
{

    // Print version and git hash
    printf("Hello, Elevator Emulator!\n");
    printf("Version: %s\n", Version_get());
    printf("Git hash: %s\n", Version_git_hash());

    // Example assertion to demonstrate the assert mechanism
    int x = 5;

    // Test assertion
    LIFT_ASSERT(x == 5);   // ✅ will pass
    LIFT_ASSERT(x == 10);  // ❌ will fail if ENABLE_ASSERT is 1

    printf("... you would never reach it :)\n");

    return 0;
}
