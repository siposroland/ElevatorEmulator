#ifndef CONDSEL_INTERNAL_H
#define CONDSEL_INTERNAL_H

#include <stdint.h>
#include <stdbool.h>

/// Indexed condition selector simulation scenario
typedef enum ConditionSelectorIndexes
{
    CONDSEL_ENUM_PEND_ANY           = 0,  ///< Any pending call: below OR same OR above
    CONDSEL_ENUM_PEND_ONLY_BELOW    = 1,  ///< Only below call is pending
    CONDSEL_ENUM_PEND_ONLY_SAME     = 2,  ///< Only same-level call is pending
    CONDSEL_ENUM_PEND_ONLY_ABOVE    = 3,  ///< Only above call is pending
    CONDSEL_ENUM_DOOR_CLOSED        = 4,  ///< Door is fully closed and locked
    CONDSEL_ENUM_DOOR_OPENED        = 5,  ///< Door is fully open
    CONDSEL_ENUM_RESERVED           = 6,  ///< Reserved index – always returns false (as per spec)
    CONDSEL_ENUM_CONST_FALSE        = 7   ///< Constant false (logical zero)
} ConditionSelectorIndexes_t;

#endif // CONDSEL_INTERNAL_H