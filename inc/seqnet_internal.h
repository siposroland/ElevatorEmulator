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
typedef enum InstructionBitPositions_t {
    BIT_MOVE_UP       = 8,   ///< Request to move the elevator upward
    BIT_MOVE_DOWN     = 9,   ///< Request to move the elevator downward
    BIT_DOOR_STATE    = 10,  ///< Target door state (1 = open, 0 = close)
    BIT_REQ_RESET     = 11,  ///< Request to delete pending call
    BIT_COND_SEL      = 12,  ///< Condition selector (2 bits: 12–13)
    BIT_COND_INV      = 15,  ///< Invert condition
    BIT_JUMP_ADDR     = 0    ///< Jump address (bits 0–7)
} InstructionBitPositions_t;

/**
 * @brief Bit masks for extracting instruction fields.
 */
typedef enum InstructionBitMasks_t {
    MASK_COND_SEL     = 0x7,     ///< 2-bit mask for condition selector
    MASK_JUMP_ADDR    = 0xFF     ///< 8-bit mask for jump address
} InstructionBitMasks_t;

/**
 * @brief Boolean flags for door request
 */
typedef enum DoorRequest_t {
    DOOR_REQ_CLOSE   = 0,        ///< Need to close the door
    DOOR_REQ_OPEN    = 1         ///< Need to open the door
} DoorRequest_t;

/**
 * @brief Returns a pointer to the internal program memory array.
 * @return Pointer to ProgMem (256-element array of uint16_t).
 */
uint16_t* SeqNetProgramMemory_get(void);

/**
 * @brief Returns the current Program Counter value.
 * @return Current PC value (0–255).
 */
uint8_t SeqNetPC_get(void);

/**
 * @brief Sets the Program Counter to a specific value.
 * @param value New PC value (0–255).
 */
void SeqNetPC_set(uint8_t value);

/**
 * @brief Convert a 16-bit instruction to SeqNet_Out structure.
 * 
 * @param instr 16-bit instruction
 * @return Converted instruction structure
 */
SeqNet_Out SeqNetInstruction_convert(uint16_t instruction);

/**
 * @brief Convert a SeqNet_Out structure into a 16-bit instruction.
 *
 * @param[in] out Pointer to the structure to encode.
 * @return Converted 16-bit instruction.
 */
uint16_t SeqNetOut_convert(const SeqNet_Out* out);

#ifdef __cplusplus
}
#endif
