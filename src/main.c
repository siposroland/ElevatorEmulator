/**
 * @file main.c
 * @brief Entry point for the elevator controller emulator.
 */

#include <stdio.h>
#include <stdint.h>
#include "version.h"
#include "test_assertion.h"
#include "test_condsel.h"
#include "test_seqnet.h"
#include "scenario_loader.h"

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

    // Test_lift_assert();  // Run simple assertion test

    Test_condsel_all_cases();  // Run all condition selector tests
    Test_seqnet_all_cases();   // Run SeqNet tests

    Scenario_load_default_program();  // Load default program into SeqNet
    Scenario_print_program();  // Print the default program memory

    return 0;
}

