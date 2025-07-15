
#ifndef LIFT_TEST_H
#define LIFT_TEST_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdbool.h>
#include <stdint.h>
#include "condsel.h"
#include "seqnet.h"

/// Maximum number of floors (adjust if needed)
#define LIFT_TEST_MAX_FLOORS            (6U)
#define LIFT_TEST_MAX_STEPS             (200U)

/**
 * @brief A single step in a test scenario for the lift controller.
 */
typedef struct
{
    uint8_t floor; // Current floor
    bool is_door_open; // Is the door open?
    bool is_moving; // Is the lift moving?
    bool calls[LIFT_TEST_MAX_FLOORS]; // Calls from other floors
} LiftState_t;

/**
 * @brief Test case step structure.
 */
typedef struct
{
    LiftState_t initial_state; // Initial state of the lift
    LiftState_t end_state; // End state of the lift
    uint8_t steps;
    uint8_t PC_preset;
} LiftTestCase_t;


/**
 * @brief Convert LiftState_t to CondSel_In structure.
 *
 * This function takes the current elevator state (floor, door, calls)
 * and fills a CondSel_In structure used for evaluating logic conditions.
 *
 * @param[in]  state  Pointer to the current lift state.
 * @param[out] out    Pointer to the output CondSel_In structure to be filled.
 */
void LiftStateArray_convert(const LiftState_t* state, CondSel_In* out);

/**
 * @brief Compares two LiftState_t structures and prints differences if any.
 *
 * @param[in] a Pointer to the first structure.
 * @param[in] b Pointer to the second structure.
 * @return true if all fields match, false otherwise.
 */
bool LiftState_compare(const LiftState_t* a, const LiftState_t* b);

/**
 * @brief Simulates a lift test case defined by input-output steps.
 *
 * @param case Pointer to array of LiftTestCase_t
 */
void LiftTestCase_run(const LiftTestCase_t* test, const char* name);

// Extern declarations for test cases
extern const LiftTestCase_t test_case_open_door_same_floor;
extern const LiftTestCase_t test_case_already_open;
extern const LiftTestCase_t test_case_open_door_same_floor;
extern const LiftTestCase_t test_case_move_down;
extern const LiftTestCase_t test_case_move_up;
extern const LiftTestCase_t test_case_multiple_calls;
extern const LiftTestCase_t test_case_new_call_during_move;
extern const LiftTestCase_t test_case_change_direction;
extern const LiftTestCase_t test_case_idle;
extern const LiftTestCase_t test_case_reopen_during_close;
extern const LiftTestCase_t test_case_bottom_to_top;
extern const LiftTestCase_t test_case_all_calls;
extern const LiftTestCase_t test_case_middle_stop;
extern const LiftTestCase_t test_case_oscillation;
extern const LiftTestCase_t test_case_up_two_floors;
extern const LiftTestCase_t test_case_down_two_floors;

/**
 * @brief Run all tests in one call
 */
void LiftTestAll_run(void);

#ifdef __cplusplus
}
#endif

#endif // LIFT_TEST_H
