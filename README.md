![License](https://img.shields.io/badge/license-Unlicense-blue)
![Language](https://img.shields.io/badge/language-C-blue)
![Status](https://img.shields.io/badge/status-stable-brightgreen)
![Version](https://img.shields.io/badge/version-1.1.0-orange)

# c-safeinput

**Version:** 1.1.0  
**License:** [The Unlicense](https://unlicense.org/)  
**Author:** [bustyanimebabesdotcom](https://github.com/bustyanimebabesdotcom)

---

### What is this?

`c-safeinput` is a minimal C input-handling library that replaces the horror of `scanf()` with something **actually safe** and **usable**.

It validates input, avoids buffer overflows, and gives you **simple APIs** for reading typed values, strings, and single characters from `stdin`.

No macros, no dependencies, and functional on both X86 and ARM systems.

---

### Features

- Input size limits to prevent buffer overflows.
- Typed input functions with built-in validation.
- EOF handling.
- Optional null-terminated or byte-counted string support.
- Minimal, portable, and fully GNU99 compatible.
- Maintainable code.

---

### Functions

```c
int si_getInt                       ( void );
unsigned int si_getUInt             ( void );

float si_getFloat                   ( void );
double si_getDouble                 ( void );

long si_getLong                     ( void );
unsigned long si_getULong           ( void );
long long si_getLongLong            ( void );
unsigned long long si_getULongLong  ( void );

int si_getChar                      ( void );
int si_getCharFiltered              ( const char *allowed );

char *si_getCString                 ( void ); // null terminated
si_string si_getString              ( void ); // byte counted

bool si_getBool                     ( void ); // accepts y/n
```


All functions are safe, loop until valid input is received, and print errors to `stderr`.

---

### Example Usage

```c
#include <stdio.h>
#include <stdlib.h>
#include <safeinput/safeinput.h>

int main ( void ) {

    int dong_counter = si_getInt();
    printf( "You have %d dongs.", dong_counter );

    si_string dong_name = si_getString();
    printf( "My dongs name is %.*s!\n", (int)dong_name.len, dong_name.data );
    free(dong_name.data);

    return 0;
}
```

---

### Building

This is a pure C file + header. Either include it in your project or run `make` to drop a .o and .a file in the build directory.

---

### License

[The Unlicense](https://unlicense.org/)  
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

---

### Don’t use `scanf()`  
You’re better than that.  
