/**
 * @file version.c
 * @brief Implementation of versioning functions.
 */

#include "version.h"

/** Project version manually set */
static const char* PROJECT_VERSION = "v0.0.1";

/** Git commit hash automatically injected at build time */
#ifndef GIT_COMMIT_HASH
#define GIT_COMMIT_HASH "unknown"
#endif

const char* Version_get(void)
{
    return PROJECT_VERSION;
}

const char* Version_git_hash(void)
{
    return GIT_COMMIT_HASH;
}
