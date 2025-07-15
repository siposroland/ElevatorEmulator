# Elevator Emulator – Coding Style Guide

This style guide defines the formatting, naming, and documentation conventions used throughout the Elevator Emulator codebase. It is based on the style used in `seqnet.h` and `condsel.h`.

---

## 1. General Guidelines

- **C99** standard is used
- Files must be **header-guarded** with `#pragma once`
- Must be compatible with **C and C++** (`extern "C"`)
- Only **standard integer types** (`<stdint.h>`) and `stdbool.h` used

---

## 2. Naming Conventions

| Item                        | Convention          | Example                      |
|-----------------------------|---------------------|------------------------------|
| File names                  | snake_case          | `seqnet.h`, `condsel.h`     |
| Type names (structs)        | PascalCase + `_t`   | `CondSel_In`, `SeqNet_Out`  |
| Function names              | PascalCase          | `SeqNet_init()`, `CondSel_calc()` |
| Constants / Macros          | UPPER_CASE          | `CONDSEL_API`, `SEQNET_API` |
| Boolean fields              | `is_`, `has_`, `should_` prefixes OR `verb_noun` | `is_moving`, `door_closed`  |

---

## 3. Header Files

- Must include this structure at the top:

```c
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
