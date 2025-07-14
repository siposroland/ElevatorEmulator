/**
 * @file myassert.h
 * @brief Simple assert mechanism for debugging, toggleable by macro.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>

#ifdef ENABLE_ASSERT

/**
 * @brief Checks a condition and aborts with message if false.
 * 
 * @param cond The condition to check.
 */
#define LIFT_ASSERT(cond)                                                      \
    do {                                                                     \
        if (!(cond)) {                                                       \
            fprintf(stderr, "[ASSERT FAILED] %s:%d: %s\n",                   \
                    __FILE__, __LINE__, #cond);                              \
            abort();                                                         \
        }                                                                    \
    } while (0)

#else

/// Asserts are disabled at compile time.
#define LIFT_ASSERT(cond) ((void)0)

#endif
