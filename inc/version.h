/**
 * @file version.h
 * @brief Provides versioning information for the build.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/** @brief Returns the version string of the software. */
const char* Version_get(void);

/** @brief Returns the Git commit hash used at build time. */
const char* Version_git_hash(void);

#ifdef __cplusplus
}
#endif
