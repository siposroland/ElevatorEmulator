/**
 * @file test_seqnet.c
 * @brief Stepwise unit tests for the SeqNet_loop() function.
 *
 * Tests start with simple bitfield extractions and progress to complex combined instructions.
 */

#include <stdio.h>
#include <string.h>
#include "seqnet.h"
#include "seqnet_internal.h"
#include "lift_assert.h"

/**
 * @brief Test case definition for a single instruction evaluation.
 */
typedef struct {
    const char* name;
    uint16_t instr;
    bool condition_active;
    uint8_t initial_pc;
    uint8_t expected_pc;
    SeqNet_Out expected;
} SeqNet_TestCase;

/**
 * @brief Runs all SeqNet test cases, starting from simple to complex instructions.
 */
void Test_seqnet_all_cases(void)
{
    printf("[TEST] Running SeqNet_loop() test cases...\n");

    SeqNet_TestCase tests[] = {
        // 1. Move up only
        {
            .name = "Move up only",
            .instr = (1 << BIT_MOVE_UP),
            .condition_active = false,
            .initial_pc = 0x00,
            .expected_pc = 0x01,
            .expected = {0, 0, 0, 0, 0, 1, 0x00}
        },
        // 2. Move down only
        {
            .name = "Move down only",
            .instr = (1 << BIT_MOVE_DOWN),
            .condition_active = false,
            .initial_pc = 0x01,
            .expected_pc = 0x02,
            .expected = {0, 0, 0, 0, 1, 0, 0x00}
        },
        // 3. Door open only
        {
            .name = "Door open only",
            .instr = (1 << BIT_DOOR_STATE),
            .condition_active = false,
            .initial_pc = 0x02,
            .expected_pc = 0x03,
            .expected = {0, 0, 0, 1, 0, 0, 0x00}
        },
        // 4. Reset call only
        {
            .name = "Reset call only",
            .instr = (1 << BIT_REQ_RESET),
            .condition_active = false,
            .initial_pc = 0x03,
            .expected_pc = 0x04,
            .expected = {0, 0, 1, 0, 0, 0, 0x00}
        },
        // 5. Jump to 0x42 if condition active
        {
            .name = "Jump to 0x42 (condition true)",
            .instr = 0x42,
            .condition_active = true,
            .initial_pc = 0x04,
            .expected_pc = 0x42,
            .expected = {0, 0, 0, 0, 0, 0, 0x42}
        },
        // 6. Jump ignored if condition inactive
        {
            .name = "Jump ignored (condition false)",
            .instr = 0x42,
            .condition_active = false,
            .initial_pc = 0x05,
            .expected_pc = 0x06,
            .expected = {0, 0, 0, 0, 0, 0, 0x42}
        },
        // 7. Only condition selector = 3
        {
            .name = "Condition selector = 3",
            .instr = (3 << BIT_COND_SEL),
            .condition_active = false,
            .initial_pc = 0x06,
            .expected_pc = 0x07,
            .expected = {0, 3, 0, 0, 0, 0, 0x00}
        },
        // 8. Condition inversion only
        {
            .name = "Condition inversion active",
            .instr = (1 << BIT_COND_INV),
            .condition_active = true,
            .initial_pc = 0x07,
            .expected_pc = 0x00,
            .expected = {1, 0, 0, 0, 0, 0, 0x00}
        },
        // 9. Full instruction with jump and condition true
        {
            .name = "Full instruction, jump taken",
            .instr = (1 << BIT_COND_INV) |
                     (1 << BIT_REQ_RESET) |
                     (1 << BIT_DOOR_STATE) |
                     (1 << BIT_MOVE_DOWN) |
                     (1 << BIT_MOVE_UP) |
                     (1 << BIT_COND_SEL) |
                     0x7F,
            .condition_active = true,
            .initial_pc = 0x08,
            .expected_pc = 0x7F,
            .expected = {1, 1, 1, 1, 1, 1, 0x7F}
        },
        // 10. Same instruction, jump not taken
        {
            .name = "Full instruction, jump ignored",
            .instr = (1 << BIT_COND_INV) |
                     (1 << BIT_REQ_RESET) |
                     (1 << BIT_DOOR_STATE) |
                     (1 << BIT_MOVE_DOWN) |
                     (1 << BIT_MOVE_UP) |
                     (1 << BIT_COND_SEL) |
                     0x7F,
            .condition_active = false,
            .initial_pc = 0x09,
            .expected_pc = 0x0A,
            .expected = {1, 1, 1, 1, 1, 1, 0x7F}
        }
    };

    const size_t num_tests = sizeof(tests) / sizeof(tests[0]);
    size_t passed = 0;

    for (size_t i = 0; i < num_tests; ++i) 
    {
        const SeqNet_TestCase* t = &tests[i];

        // Initialize PC and memory
        SeqNet_set_pc(t->initial_pc);
        uint16_t* mem = SeqNet_get_program_memory();
        memset(mem, 0, 256 * sizeof(uint16_t));
        mem[t->initial_pc] = t->instr;

        // Execute one instruction
        SeqNet_Out out = SeqNet_loop(t->condition_active);
        uint8_t new_pc = SeqNet_get_pc();

        // Check result
        bool ok = (new_pc == t->expected_pc) &&
                  (memcmp(&out, &t->expected, sizeof(SeqNet_Out)) == 0);

        printf("  - %-40s ... %s\n", t->name, ok ? "OK" : "FAIL");

        if (!ok) {
            printf("    > Expected PC: 0x%02X, Got: 0x%02X\n", t->expected_pc, new_pc);
        }

        LIFT_ASSERT(ok);
        passed += ok;
    }

    printf("[TEST] %zu/%zu tests passed.\n", passed, num_tests);
}
