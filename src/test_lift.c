
#include "scenario_loader.h"
#include "seqnet.h"
#include "condsel.h"
#include "seqnet_internal.h"
#include "condsel_internal.h"
#include "test_lift.h"
#include <stdio.h>
#include <string.h>
#include "lift_assert.h"

#define LIFT_TEST_DEBUG_LOG_ENABLED 0

/**
 * @brief Simulates a lift test case defined by input-output steps.
 *
 * @param case Pointer to array of LiftTestCase_t
 */
void RunLiftTestCase(const LiftTestCase_t* test, const char* name)
{
    CondSel_In cond_in;
    SeqNet_Out seq_out;
    LiftState_t actual;

    uint16_t instr = 0;
    uint16_t pc_pre = 0;
    uint16_t pc_post = 0;

    bool cond_result = false;


    if (test == NULL || test->steps == 0) 
    {
        printf("No test case provided or empty steps.\n");
        return;
    }

    // Print test case name
    printf("Test case: %s\n", name);

    // Initialize the sequential network
    SeqNet_init();

    // Optionally set the Program Counter to a preset value
    SeqNet_set_pc(test->PC_preset);

    // Load the initial state from the test case
    memcpy(&actual, &(test->initial_state), sizeof(LiftState_t));

    // Print the initial state
    printf (
        "INT: Floor: %d, Door Open: %s, Moving: %s, Calls: [%d, %d, %d, %d, %d, %d], PC preset: %d\n",
            actual.floor,
            actual.is_door_open ? "Y" : "N",
            actual.is_moving ? "Y" : "N",
            actual.calls[0], actual.calls[1], actual.calls[2],
            actual.calls[3], actual.calls[4], actual.calls[5],
            test->PC_preset
    );

    // Iterate through each step in the test case
    for (uint8_t step = 0; step < test->steps; ++step)
    {
        // Get actual index of ProgMem
        pc_pre = SeqNet_get_pc();

        // Fetch the instruction from ProgMem
        instr = SeqNet_get_program_memory()[pc_pre];

        // Convert 16-bit array to instruction
        SeqNet_Out output = SeqNet_convert_instruction(instr);

        // Convert initial lift state to condition selector input format
        Lift_state_to_array(&actual, &cond_in);

        // Calculate the condition selector input
        cond_result = CondSel_calc(output.cond_inv, output.cond_sel, cond_in);
        //printf("Condition result: %s, inv: %d, idx: %d\n", cond_result ? "True" : "False", output.cond_inv, output.cond_sel);

        // Process the sequence network loop
        seq_out = SeqNet_loop(cond_result);

        // Get NEW index of ProgMem
        pc_post = SeqNet_get_pc();

#if LIFT_TEST_DEBUG_LOG_ENABLED
        // Print the results for debugging
        printf (
            "Step %02d: Floor: %d, Door Open: %s, Moving: %s, prePC: %02d, postPC: %02d, Reset: %d, Calls: [%d, %d, %d, %d, %d, %d], Up: %d, Down: %d, DReq: %d\n",
                step,
                actual.floor,
                actual.is_door_open ? "Y" : "N",
                actual.is_moving ? "Y" : "N",
                pc_pre,
                pc_post,
                seq_out.req_reset ? 1 : 0,
                actual.calls[0], actual.calls[1], actual.calls[2],
                actual.calls[3], actual.calls[4], actual.calls[5],
                seq_out.req_move_up ? 1 : 0,
                seq_out.req_move_down ? 1 : 0,
                seq_out.req_door_state == DOOR_REQ_OPEN ? 1 : 0
        );
#endif //LIFT_TEST_DEBUG_LOG_ENABLED

        // EMULATE the lift state change

        // DOORS
        if (DOOR_REQ_OPEN == seq_out.req_door_state)
        {
            actual.is_door_open = true;
        }
        else if (DOOR_REQ_CLOSE == seq_out.req_door_state)
        {
            actual.is_door_open = false;
        }

        // CALLS
        if (seq_out.req_reset)
        {
            // Reset the call state for the current floor
            actual.calls[actual.floor] = false;
        }

        // MOVEMENT
        if (seq_out.req_move_up && !seq_out.req_move_down)
        {
            actual.is_moving = true;
            actual.floor++;
        }
        else if (seq_out.req_move_down && !seq_out.req_move_up)
        {
            actual.is_moving = true;
            actual.floor--;
        }
        else if (!seq_out.req_move_down && !seq_out.req_move_up)
        {
            actual.is_moving = false; // Stop moving if no direction is requested
        }
        else
        {
            LIFT_ASSERT(false);
        }
    }

    // Print the last state
    printf (
        "END: Floor: %d, Door Open: %s, Moving: %s, Calls: [%d, %d, %d, %d, %d, %d]\n",
            actual.floor,
            actual.is_door_open ? "Y" : "N",
            actual.is_moving ? "Y" : "N",
            actual.calls[0], actual.calls[1], actual.calls[2],
            actual.calls[3], actual.calls[4], actual.calls[5]
    );

    // Print the comparison state
    printf (
        "REF: Floor: %d, Door Open: %s, Moving: %s, Calls: [%d, %d, %d, %d, %d, %d]\n",
            test->end_state.floor,
            test->end_state.is_door_open ? "Y" : "N",
            test->end_state.is_moving ? "Y" : "N",
            test->end_state.calls[0], actual.calls[1], actual.calls[2],
            test->end_state.calls[3], actual.calls[4], actual.calls[5]
    );

    // Print final state
    bool result = LiftState_equals_verbose(&actual, &(test->end_state));
    if (result)
    {
        printf("Test case PASSED!\n");
    }
    else
    {
        printf("Test case FAILED!\n");
    }
    
    printf("============================\n");
}

/**
 * @brief Converts a high-level lift state into condition selector input format.
 *
 * This function takes the current elevator state (floor, door, calls)
 * and fills a CondSel_In structure used for evaluating logic conditions.
 *
 * @param[in]  state  Pointer to the current lift state.
 * @param[out] out    Pointer to the output CondSel_In structure to be filled.
 */
void Lift_state_to_array (const LiftState_t* state, CondSel_In* out)
{
    // Door state
    out->door_closed = !state->is_door_open;
    out->door_open = state->is_door_open;
    /*printf("O: %d, C: %d\n",
           out->door_open ? 1 : 0,
           out->door_closed ? 1 : 0);*/

    // Calls
    out->call_pending_below = false;
    out->call_pending_same  = false;
    out->call_pending_above = false;

    for (int i = 0; i < LIFT_TEST_MAX_FLOORS; ++i)
    {
        if (!state->calls[i]) continue;
        if (i < state->floor)
            out->call_pending_below = true;
        else if (i == state->floor)
            out->call_pending_same = true;
        else
            out->call_pending_above = true;
    }
}

/**
 * @brief Compares two LiftState_t structures and prints differences if any.
 *
 * @param[in] a Pointer to the first structure.
 * @param[in] b Pointer to the second structure.
 * @return true if all fields match, false otherwise.
 */
bool LiftState_equals_verbose(const LiftState_t* a, const LiftState_t* b)
{
    bool match = true;

    if (a->floor != b->floor)
    {
        printf("Mismatch: floor (expected: %u, got: %u)\n", b->floor, a->floor);
        match = false;
    }

    if (a->is_door_open != b->is_door_open)
    {
        printf("Mismatch: is_door_open (expected: %s, got: %s)\n",
               b->is_door_open ? "true" : "false",
               a->is_door_open ? "true" : "false");
        match = false;
    }

    if (a->is_moving != b->is_moving)
    {
        printf("Mismatch: is_moving (expected: %s, got: %s)\n",
               b->is_moving ? "true" : "false",
               a->is_moving ? "true" : "false");
        match = false;
    }

    for (int i = 0; i < LIFT_TEST_MAX_FLOORS; ++i)
    {
        if (a->calls[i] != b->calls[i])
        {
            printf("Mismatch: calls[%d] (expected: %s, got: %s)\n",
                   i,
                   b->calls[i] ? "true" : "false",
                   a->calls[i] ? "true" : "false");
            match = false;
        }
    }

    return match;
}

// Call from current floor, door already open
const LiftTestCase_t test_case_already_open = {
    .initial_state = { .floor = 1, .is_door_open = true, .is_moving = false, .calls = { false, true, false, false, false, false } },
    .end_state     = { .floor = 1, .is_door_open = true, .is_moving = false, .calls = { false, false, false, false, false, false } },
    .steps = 10
};

// Call from current floor, door closed
const LiftTestCase_t test_case_open_door_same_floor = {
    .initial_state = { .floor = 2, .is_door_open = true, .is_moving = false, .calls = { false, false, true, false, false, false } },
    .end_state     = { .floor = 2, .is_door_open = true, .is_moving = false, .calls = { false, false, false, false, false, false } },
    .steps = 8
};

// Call from floor below
const LiftTestCase_t test_case_move_down = {
    .initial_state = { .floor = 3, .is_door_open = true, .is_moving = false, .calls = { false, true, false, false, false, false } },
    .end_state     = { .floor = 1, .is_door_open = true, .is_moving = false, .calls = { false, false, false, false, false, false } },
    .steps = 15
};

// Call from floor above
const LiftTestCase_t test_case_move_up = {
    .initial_state = { .floor = 1, .is_door_open = true, .is_moving = false, .calls = { false, false, false, true, false, false } },
    .end_state     = { .floor = 3, .is_door_open = true, .is_moving = false, .calls = { false, false, false, false, false, false } },
    .steps = 15
};

// Multiple calls – up and down
const LiftTestCase_t test_case_multiple_calls = {
    .initial_state = { .floor = 2, .is_door_open = true, .is_moving = false, .calls = { false, true, false, true, false, false } },
    .end_state     = { .floor = 3, .is_door_open = true, .is_moving = false, .calls = { false, false, false, false, false, false } },
    .steps = 25
};

// No calls (idle)
const LiftTestCase_t test_case_idle = {
    .initial_state = { .floor = 2, .is_door_open = true, .is_moving = false, .calls = { false, false, false, false, false, false } },
    .end_state     = { .floor = 2, .is_door_open = true, .is_moving = false, .calls = { false, false, false, false, false, false } },
    .steps = 10
};

// Door re-open during closing
const LiftTestCase_t test_case_reopen_during_close = {
    .initial_state = { .floor = 3, .is_door_open = false, .is_moving = false, .calls = { false, false, false, true, false, false } },
    .end_state     = { .floor = 3, .is_door_open = true, .is_moving = false, .calls = { false, false, false, false, false, false } },
    .steps = 15,
    .PC_preset = 3 // Start at door closing state
};

// Long trip from bottom to top
const LiftTestCase_t test_case_bottom_to_top = {
    .initial_state = { .floor = 0, .is_door_open = false, .is_moving = false, .calls = { false, false, false, false, false, true } },
    .end_state     = { .floor = 5, .is_door_open = true, .is_moving = false, .calls = { false, false, false, false, false, false } },
    .steps = 25
};

// All floors have calls
const LiftTestCase_t test_case_all_calls = {
    .initial_state = { .floor = 0, .is_door_open = false, .is_moving = false, .calls = { true, true, true, true, true, true } },
    .end_state     = { .floor = 5, .is_door_open = true, .is_moving = false, .calls = { false, false, false, false, false, false } },
    .steps = 75
};

// Stop on middle floor
const LiftTestCase_t test_case_middle_stop = {
    .initial_state = { .floor = 0, .is_door_open = true, .is_moving = false, .calls = { false, false, true, false, false, false } },
    .end_state     = { .floor = 2, .is_door_open = true, .is_moving = false, .calls = { false, false, false, false, false, false } },
    .steps = 16
};

// Move up 2 floors
const LiftTestCase_t test_case_up_two_floors = {
    .initial_state = { .floor = 1, .is_door_open = true, .is_moving = false, .calls = { false, false, false, true, false, false } },
    .end_state     = { .floor = 3, .is_door_open = true, .is_moving = false, .calls = { false, false, false, false, false, false } },
    .steps = 16
};

// Move down 2 floors
const LiftTestCase_t test_case_down_two_floors = {
    .initial_state = { .floor = 4, .is_door_open = false, .is_moving = false, .calls = { false, true, false, false, false, false } },
    .end_state     = { .floor = 1, .is_door_open = true, .is_moving = false, .calls = { false, false, false, false, false, false } },
    .steps = 18
};

void RunAllLiftTestCases(void)
{
    RunLiftTestCase(&test_case_already_open,           "already_open");
    RunLiftTestCase(&test_case_open_door_same_floor,   "open_door_same_floor");
    RunLiftTestCase(&test_case_move_down,              "move_down");
    RunLiftTestCase(&test_case_move_up,                "move_up");
    RunLiftTestCase(&test_case_multiple_calls,         "multiple_calls");
    RunLiftTestCase(&test_case_idle,                   "idle");
    RunLiftTestCase(&test_case_reopen_during_close,    "reopen_during_close");
    RunLiftTestCase(&test_case_bottom_to_top,          "bottom_to_top");
    RunLiftTestCase(&test_case_middle_stop,            "middle_stop");
    RunLiftTestCase(&test_case_up_two_floors,          "up_two_floors");
    RunLiftTestCase(&test_case_down_two_floors,        "down_two_floors");
    RunLiftTestCase(&test_case_all_calls,              "all_calls");
}
