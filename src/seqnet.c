/**
 * @file seqnet.c
 * @brief Implements the sequential network logic for the elevator controller.
 */

#include "seqnet.h"
#include "lift_assert.h"
#include <string.h>  // For memset
#include "seqnet_internal.h"  // for BIT_*, MASK_*

/// Debug print for PC switch
#define DEBUG_PC_ENABLED 0

#if DEBUG_PC_ENABLED
    #define DEBUG_PC_PRINTF(...) printf(__VA_ARGS__)
#else
    #define DEBUG_PC_PRINTF(...) do {} while (0)
#endif


/// Size of the program memory
#define PROGMEM_SIZE 255

/// Program memory: holds 16-bit encoded instructions
static uint16_t SeqNet_ProgMem[PROGMEM_SIZE];

/// Program Counter: points to the current instruction
uint8_t SeqNet_PC = 0;

// === Internal accessors for testing ===

/**
 * @brief Returns a pointer to the internal program memory array.
 *
 * This function is intended for testing or emulation purposes only.
 *
 * @return Pointer to the 256-element instruction memory.
 */
uint16_t* SeqNet_get_program_memory(void)
{
    return SeqNet_ProgMem;
}

/**
 * @brief Returns the current value of the Program Counter.
 *
 * Used to inspect the internal state during testing.
 *
 * @return Current value of PC (0–255).
 */
uint8_t SeqNet_get_pc(void)
{
    DEBUG_PC_PRINTF("DEBUG: PC get: 0x%02X\n", SeqNet_PC);
    return SeqNet_PC;
}

/**
 * @brief Sets the value of the Program Counter.
 *
 * Used to initialize or force specific execution flow in tests or emulation.
 *
 * @param value New Program Counter value (must be < 256).
 */
void SeqNet_set_pc(uint8_t value)
{
    LIFT_ASSERT(value < PROGMEM_SIZE);
    SeqNet_PC = value;
    DEBUG_PC_PRINTF("DEBUG: PC set: 0x%02X\n", SeqNet_PC);
}

// === API functions ===

/**
 * @brief Initializes the sequential network.
 * 
 * Resets the internal state: PC is set to 0, program memory is cleared to 0.
 */
void SeqNet_init(void)
{
    SeqNet_PC = 0;
    memset(SeqNet_ProgMem, 0, sizeof(SeqNet_ProgMem));
}

/**
 * @brief Executes the current instruction and advances or jumps the PC.
 * 
 * This function fetches the current instruction from memory (at PC),
 * interprets it field-by-field, returns the requested output structure,
 * and updates PC based on the `condition_active` flag.
 * 
 * @param[in] condition_active  True if the selected condition is active (or inverted false).
 * @return SeqNet_Out           Decoded instruction fields.
 */
SeqNet_Out SeqNet_loop(const bool condition_active)
{
    // Fetch current instruction from memory
    LIFT_ASSERT(SeqNet_PC < PROGMEM_SIZE);
    uint16_t instr = SeqNet_ProgMem[SeqNet_PC];

    // Decode instruction into output structure
    SeqNet_Out out = 
    {
        .cond_inv       = ((instr >> BIT_COND_INV)   & 0x1),
        .cond_sel       = ((instr >> BIT_COND_SEL)   & MASK_COND_SEL),
        .req_reset      = ((instr >> BIT_REQ_RESET)  & 0x1),
        .req_door_state = ((instr >> BIT_DOOR_STATE) & 0x1),
        .req_move_down  = ((instr >> BIT_MOVE_DOWN)  & 0x1),
        .req_move_up    = ((instr >> BIT_MOVE_UP)    & 0x1),
        .jump_addr      = ((instr >> BIT_JUMP_ADDR)  & MASK_JUMP_ADDR)
    };

    // Update PC based on condition
    if (condition_active) 
    {
        SeqNet_PC = out.jump_addr;
        DEBUG_PC_PRINTF("DEBUG: PC jump: 0x%02X\n", SeqNet_PC);
    } 
    else 
    {
        DEBUG_PC_PRINTF("DEBUG: PC before increment: 0x%02X\n", SeqNet_PC);
        SeqNet_PC = (SeqNet_PC + 1) % PROGMEM_SIZE;
        DEBUG_PC_PRINTF("DEBUG: PC increment: 0x%02X\n", SeqNet_PC);
    }

    return out;
}
