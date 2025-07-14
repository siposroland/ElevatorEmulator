/**
 * @file main.c
 * @brief Entry point for the elevator controller emulator.
 */

#include <stdio.h>
#include "version.h"

/**
 * @brief Main function to test the build system.
 * 
 * @return int Returns 0 on successful execution.
 */
int main(void)
{
    printf("Hello, Elevator Emulator!\n");
    printf("Version: %s\n", Version_get());
    printf("Git hash: %s\n", Version_git_hash());
    return 0;
}
