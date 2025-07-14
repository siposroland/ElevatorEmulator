/**
 * @file test_condsel.c
 * @brief Structured unit test for the condition selector module.
 */

#include <stdio.h>
#include <stdbool.h>
#include "condsel.h"
#include "lift_assert.h"

typedef struct {
    const char* name;
    CondSel_In inputs;
    uint8_t index;
    bool invert;
    bool expected;
} CondSel_TestCase;

/**
 * @brief Runs all defined test cases for the condition selector.
 */
void Test_condsel_all_cases(void)
{
    printf("[TEST] Running CondSel_calc() test cases...\n");

    CondSel_TestCase tests[] = {
        {
            .name = "Pending below only, index=1",
            .inputs = { true, false, false, false, false },
            .index = 1,
            .invert = false,
            .expected = true
        },
        {
            .name = "Pending above only, index=3",
            .inputs = { false, false, true, false, false },
            .index = 3,
            .invert = false,
            .expected = true
        },
        {
            .name = "All pending, index=0 (any pending)",
            .inputs = { true, true, true, false, false },
            .index = 0,
            .invert = false,
            .expected = true
        },
        {
            .name = "No pending, index=0 (any pending)",
            .inputs = { false, false, false, false, false },
            .index = 0,
            .invert = false,
            .expected = false
        },
        {
            .name = "Door open, index=5",
            .inputs = { false, false, false, false, true },
            .index = 5,
            .invert = false,
            .expected = true
        },
        {
            .name = "Door closed, index=4, invert=true",
            .inputs = { false, false, false, true, false },
            .index = 4,
            .invert = true,
            .expected = false
        },
        {
            .name = "Reserved index 6, always false",
            .inputs = { true, true, true, true, true },
            .index = 6,
            .invert = false,
            .expected = false
        },
        {
            .name = "Constant false (index 7), invert=true",
            .inputs = { true, true, true, true, true },
            .index = 7,
            .invert = true,
            .expected = true
        },
        {
            .name = "Only same pending, index=2",
            .inputs = { false, true, false, false, false },
            .index = 2,
            .invert = false,
            .expected = true
        },
        {
            .name = "Only below pending, index=0 (any pend)",
            .inputs = { true, false, false, false, false },
            .index = 0,
            .invert = false,
            .expected = true
        }
    };

    const size_t num_tests = sizeof(tests) / sizeof(tests[0]);
    size_t passed = 0;

    for (size_t i = 0; i < num_tests; ++i) 
    {
        CondSel_TestCase t = tests[i];
        bool result = CondSel_calc(t.invert, t.index, t.inputs);
        printf("  - %-40s ... ", t.name);
        if (result == t.expected) 
        {
            printf("OK\n");
            passed++;
        } 
        else 
        {
            printf("FAIL (got %d, expected %d)\n", result, t.expected);
        }
        LIFT_ASSERT(result == t.expected);
    }

    printf("[TEST] %zu/%zu tests passed.\n", passed, num_tests);
}
