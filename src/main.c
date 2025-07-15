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
#include "test_lift.h"
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
    printf("Git hash: %s\n", VersionGitHash_get());

    // LiftAssert_test();  // Run simple assertion test

    CondSelAllCases_test();  // Run all condition selector tests
    SeqNetAllCases_test();   // Run SeqNet tests

    ScenarioDefaultProgram_load();  // Load default program into SeqNet
    ScenarioProgram_print();  // Print the default program memory

    LiftTestAll_run();  // Run all lift test cases
    
    return 0;
}

