/**
 * @file scenario_loader.c
 * @brief Provides predefined test sequences for the elevator emulator.
 */

#include "scenario_loader.h"
#include "seqnet.h"
#include "seqnet_internal.h"  // For BIT_*, MASK_*
#include "condsel_internal.h"  // CONDSEL ENUMs
#include <stdint.h>
#include <stdio.h>

/// Predefined scenario steps in sequential logic order
static const SeqNet_Out default_program[] = {
    // PC 0: Check for any pending call → jump to PC 4 if exists
    { .jump_addr = 2,  .cond_sel = CONDSEL_ENUM_PEND_ANY, .cond_inv = 0, .req_move_up = 0, .req_move_down = 0, .req_door_state = DOOR_REQ_OPEN, .req_reset = 0 },

    // PC 1: Idle loop if no call → jump to PC 0
    { .jump_addr = 0,  .cond_sel = CONDSEL_ENUM_CONST_FALSE, .cond_inv = 1, .req_move_up = 0, .req_move_down = 0, .req_door_state = DOOR_REQ_OPEN, .req_reset = 0 },

    // PC 2: If call on same floor → jump to PC 13
    { .jump_addr = 13, .cond_sel = CONDSEL_ENUM_PEND_ONLY_SAME, .cond_inv = 0, .req_move_up = 0, .req_move_down = 0, .req_door_state = DOOR_REQ_OPEN, .req_reset = 0 },

    // PC 3: Request to close door → jump to PC 4
    { .jump_addr = 4,  .cond_sel = CONDSEL_ENUM_CONST_FALSE, .cond_inv = 1, .req_move_up = 0, .req_move_down = 0, .req_door_state = DOOR_REQ_CLOSE, .req_reset = 0 },

    // PC 4: Wait until door is closed (loop)
    { .jump_addr = 4,  .cond_sel = CONDSEL_ENUM_DOOR_CLOSED, .cond_inv = 1, .req_move_up = 0, .req_move_down = 0, .req_door_state = DOOR_REQ_CLOSE, .req_reset = 0 },

    // PC 5: If call below → jump to PC 10
    { .jump_addr = 10,  .cond_sel = CONDSEL_ENUM_PEND_ONLY_BELOW, .cond_inv = 0, .req_move_up = 0, .req_move_down = 0, .req_door_state = DOOR_REQ_CLOSE, .req_reset = 0 },

    // PC 6: If NOT call above → jump to PC 0
    { .jump_addr = 0, .cond_sel = CONDSEL_ENUM_PEND_ONLY_ABOVE, .cond_inv = 1, .req_move_up = 0, .req_move_down = 0, .req_door_state = DOOR_REQ_CLOSE, .req_reset = 0 },

    // PC 7: Move up → jump to PC 8
    { .jump_addr = 8, .cond_sel = CONDSEL_ENUM_CONST_FALSE, .cond_inv = 1, .req_move_up = 1, .req_move_down = 0, .req_door_state = DOOR_REQ_CLOSE, .req_reset = 0 },

    // PC 8: Wait until floor reached (up)
    { .jump_addr = 7, .cond_sel = CONDSEL_ENUM_PEND_ONLY_SAME, .cond_inv = 1, .req_move_up = 0, .req_move_down = 0, .req_door_state = DOOR_REQ_CLOSE, .req_reset = 0 },

    // PC 9: Stop moving (up) → jump to Door open PC 14
    { .jump_addr = 13, .cond_sel = CONDSEL_ENUM_CONST_FALSE, .cond_inv = 1, .req_move_up = 0, .req_move_down = 0, .req_door_state = DOOR_REQ_CLOSE, .req_reset = 0 },

    // PC 10: Move down → jump to PC 11
    { .jump_addr = 11,  .cond_sel = CONDSEL_ENUM_CONST_FALSE, .cond_inv = 1, .req_move_up = 0, .req_move_down = 1, .req_door_state = DOOR_REQ_CLOSE, .req_reset = 0 },

    // PC 11: Wait until floor reached (down)
    { .jump_addr = 10,  .cond_sel = CONDSEL_ENUM_PEND_ONLY_SAME, .cond_inv = 1, .req_move_up = 0, .req_move_down = 0, .req_door_state = DOOR_REQ_CLOSE, .req_reset = 0 },

    // PC 12: Stop moving → jump to Door open PC 14
    { .jump_addr = 14, .cond_sel = CONDSEL_ENUM_CONST_FALSE, .cond_inv = 1, .req_move_up = 0, .req_move_down = 0, .req_door_state = DOOR_REQ_CLOSE, .req_reset = 0 },

    // PC 13: Open door → jump to PC 14
    { .jump_addr = 14, .cond_sel = CONDSEL_ENUM_CONST_FALSE, .cond_inv = 1, .req_move_up = 0, .req_move_down = 0, .req_door_state = DOOR_REQ_OPEN, .req_reset = 0 },

    // PC 14: Wait until door is open (loop)
    { .jump_addr = 14, .cond_sel = CONDSEL_ENUM_DOOR_OPENED, .cond_inv = 1, .req_move_up = 0, .req_move_down = 0, .req_door_state = DOOR_REQ_OPEN, .req_reset = 0 },

    // PC 15: Reset call → jump to idle (PC 0)
    { .jump_addr = 0,  .cond_sel = CONDSEL_ENUM_CONST_FALSE, .cond_inv = 1, .req_move_up = 0, .req_move_down = 0, .req_door_state = DOOR_REQ_OPEN, .req_reset = 1 },
};

/**
 * @brief Loads the default instruction set into program memory.
 */
void ScenarioDefaultProgram_load(void)
{
    uint16_t* ProgMem = SeqNetProgramMemory_get();
    for (uint8_t i = 0; i < sizeof(default_program)/sizeof(default_program[0]); ++i)
    {
        ProgMem[i] = SeqNetOut_convert(&default_program[i]);
    }
}

/**
 * @brief Prints the program memory content in human-readable format.
 */
void ScenarioProgram_print(void)
{
    uint16_t* ProgMem = SeqNetProgramMemory_get();
    printf("=== Program Memory Dump ===\n");
    printf(" PC | Jmp | MU | MD | DR | R | CSEL | CIN | Hex \n");
    printf("----+-----+----+----+----+---+------+-----+------\n");
    for (uint8_t i = 0; i < sizeof(default_program)/sizeof(default_program[0]); ++i)
    {
        SeqNet_Out instr = SeqNetInstruction_convert(ProgMem[i]);
        printf("%3u | %3u | %2u | %2u | %2s | %u |  %2u  |  %u  | 0x%04X\n",
               i,
               instr.jump_addr,
               instr.req_move_up,
               instr.req_move_down,
               instr.req_door_state ? "OP" : "CL",
               instr.req_reset,
               instr.cond_sel,
               instr.cond_inv,
               ProgMem[i]);
    }
    printf("============================\n");
}