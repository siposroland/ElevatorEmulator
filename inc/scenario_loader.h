/**
 * @file scenario_loader.h
 * @brief Interface for loading and printing predefined elevator control scenarios.
 */

#ifndef SCENARIO_LOADER_H
#define SCENARIO_LOADER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Loads the default instruction set into program memory.
 * 
 * This function should be called once at startup to populate the
 * program memory with a predefined instruction sequence for the elevator.
 */
void Scenario_load_default_program(void);

/**
 * @brief Prints the program memory contents in a structured and readable format.
 * 
 * Useful for debugging and validation purposes.
 */
void Scenario_print_program(void);

#ifdef __cplusplus
}
#endif

#endif // SCENARIO_LOADER_H