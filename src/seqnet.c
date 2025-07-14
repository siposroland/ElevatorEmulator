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
    SeqNet_Out out = SeqNet_convert_instruction(instr);

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

/**
 * @brief Convert a 16-bit instruction to SeqNet_Out structure.
 * 
 * @param instr 16-bit instruction
 * @return Converted instruction structure
 */
SeqNet_Out SeqNet_convert_instruction(uint16_t instruction)
{
    SeqNet_Out out = 
    {
        .cond_inv       = (instruction >> BIT_COND_INV)   & 0x1,
        .cond_sel       = (instruction >> BIT_COND_SEL)   & MASK_COND_SEL,
        .req_reset      = (instruction >> BIT_REQ_RESET)  & 0x1,
        .req_door_state = (instruction >> BIT_DOOR_STATE) & 0x1,
        .req_move_down  = (instruction >> BIT_MOVE_DOWN)  & 0x1,
        .req_move_up    = (instruction >> BIT_MOVE_UP)    & 0x1,
        .jump_addr      = (instruction >> BIT_JUMP_ADDR)  & MASK_JUMP_ADDR
    };

    return out;
}

/**
 * @brief Convert a SeqNet_Out structure into a 16-bit instruction.
 *
 * @param[in] out Pointer to the structure to encode.
 * @return Converted 16-bit instruction.
 */
uint16_t SeqNet_convert_out(const SeqNet_Out* out)
{
    LIFT_ASSERT(out != NULL);

    uint16_t instr = 0;

    instr |= ((out->jump_addr     & MASK_JUMP_ADDR)   << BIT_JUMP_ADDR);
    instr |= ((out->req_move_up   & 0x1)              << BIT_MOVE_UP);
    instr |= ((out->req_move_down & 0x1)              << BIT_MOVE_DOWN);
    instr |= ((out->req_door_state & 0x1)             << BIT_DOOR_STATE);
    instr |= ((out->req_reset     & 0x1)              << BIT_REQ_RESET);
    instr |= ((out->cond_sel      & MASK_COND_SEL)    << BIT_COND_SEL);
    instr |= ((out->cond_inv      & 0x1)              << BIT_COND_INV);

    return instr;
}