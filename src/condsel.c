/**
 * @file condsel.c
 * @brief Implements the condition selector logic for the elevator controller.
 */

#include "condsel.h"
#include "lift_assert.h"
#include "condsel_internal.h"

#define CONDSEL_MAXIMUM_INDEX 7  // Maximum index for condition selection (0-7)

/**
 * @brief Returns the result of a condition check based on the input values and selector index.
 * 
 * This function selects one of the input conditions or a constant value based on the index parameter,
 * then optionally inverts the result. The selection logic is based on the specification in condsel.h.
 * 
 * @param[in] invert  If true, the selected result will be logically negated before return.
 * @param[in] index   Index of the condition to evaluate (0–7).
 * @param[in] values  Structure containing all condition inputs.
 * @return bool       The result of the selected condition (possibly inverted).
 */
bool CondSel_calc(const bool invert, const uint8_t index, const CondSel_In values)
{
    bool result = false;
    ConditionSelectorIndexes_t cIndex = (ConditionSelectorIndexes_t)index;

    // Validate selector index range (0–7)
    LIFT_ASSERT( (CONDSEL_MAXIMUM_INDEX + 1) >= index);

    switch (cIndex) 
    {
        case CONDSEL_ENUM_PEND_ANY:
            // Any pending call: below OR same OR above
            result = 
            (
                (true == values.call_pending_below )    ||
                (true == values.call_pending_same)      ||
                (true == values.call_pending_above)
            );
            break;

        case CONDSEL_ENUM_PEND_ONLY_BELOW:
            // Only below call is pending
            result = (true == values.call_pending_below);
            break;

        case CONDSEL_ENUM_PEND_ONLY_SAME:
            // Only same-level call is pending
            result = (true == values.call_pending_same);
            break;

        case CONDSEL_ENUM_PEND_ONLY_ABOVE:
            // Only above call is pending
            result = (true == values.call_pending_above);
            break;

        case CONDSEL_ENUM_DOOR_CLOSED:
            // Door is fully closed and locked
            result = (true == values.door_closed);
            break;

        case CONDSEL_ENUM_DOOR_OPENED:
            // Door is fully open
            result = (true == values.door_open);
            break;

        case CONDSEL_ENUM_RESERVED:
            // Reserved index – always returns false (as per spec)
            result = false;
            break;

        case CONDSEL_ENUM_CONST_FALSE:
            // Constant false (logical zero)
            result = false;
            break;

        default:
            // This should never occur due to assertion, added for completeness
            result = false;
            break;
    }

    // Invert the result if requested
    if (true == invert) 
    {
        result = !result;
    }

    return result;
}
