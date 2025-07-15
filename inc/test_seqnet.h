/**
 * @file test_seqnet.h
 * @brief Public test entry point for the sequential network module.
 *
 * This module contains unit tests for the SeqNet_loop() function,
 * which evaluates the 16-bit instructions stored in program memory.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Executes all test cases for the sequential network evaluation.
 * 
 * Prints test results to standard output. Uses assertions to fail fast if any case fails.
 */
void SeqNetAllCases_test(void);

#ifdef __cplusplus
}
#endif
