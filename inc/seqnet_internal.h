/**
 * @file seqnet_internal.h
 * @brief Internal accessors for testing the sequential network module.
 *
 * These functions are not part of the public API and are only intended
 * for unit testing or emulator support.
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Bit positions for decoding a 16-bit instruction.
 */
typedef enum InstructionBitPositions {
    BIT_MOVE_UP       = 8,   ///< Request to move the elevator upward
    BIT_MOVE_DOWN     = 9,   ///< Request to move the elevator downward
    BIT_DOOR_STATE    = 10,  ///< Target door state (1 = open, 0 = close)
    BIT_REQ_RESET     = 11,  ///< Request to delete pending call
    BIT_COND_SEL      = 12,  ///< Condition selector (2 bits: 12–13)
    BIT_COND_INV      = 14,  ///< Invert condition
    BIT_JUMP_ADDR     = 0    ///< Jump address (bits 0–7)
} InstructionBitPositions_t;

/**
 * @brief Bit masks for extracting instruction fields.
 */
typedef enum InstructionBitMasks {
    MASK_COND_SEL     = 0x3,     ///< 2-bit mask for condition selector
    MASK_JUMP_ADDR    = 0xFF     ///< 8-bit mask for jump address
} InstructionBitMasks_t;

/**
 * @brief Returns a pointer to the internal program memory array.
 * @return Pointer to ProgMem (256-element array of uint16_t).
 */
uint16_t* SeqNet_get_program_memory(void);

/**
 * @brief Returns the current Program Counter value.
 * @return Current PC value (0–255).
 */
uint8_t SeqNet_get_pc(void);

/**
 * @brief Sets the Program Counter to a specific value.
 * @param value New PC value (0–255).
 */
void SeqNet_set_pc(uint8_t value);

#ifdef __cplusplus
}
#endif
