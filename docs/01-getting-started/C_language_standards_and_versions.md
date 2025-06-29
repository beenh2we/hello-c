# C Language Standards and Versions

C language, a system-level programming language born in the early 1970s, has undergone multiple standardization improvements, continuously evolving to adapt to new programming needs and hardware environments. This article will introduce in detail the evolution of C language version standards, feature differences, and practical application scenarios.

## I. Basic Concepts of C Language Standards

C language standards are specifications defined by the International Organization for Standardization (ISO) and the American National Standards Institute (ANSI), which clearly define:

- **Language syntax**: keywords, expressions, statement structures, etc.
- **Standard library**: functions, macros, type definitions, etc.
- **Semantics**: precise meaning of program constructs
- **Implementation constraints**: minimum requirements that compilers must support

The purpose of standardization is to ensure **portability** and **consistency** of C language programs, allowing the same code to run reliably across different platforms and compilers.

## II. Evolution History of C Language Standards

| Standard Name | Release Year  | Official Name          | Main Features                                                        |
| ------------- | ------------- | ---------------------- | -------------------------------------------------------------------- |
| K&R C         | 1978          | Unofficial standard    | Initial C language, described by Kernighan and Ritchie in their book |
| C89/ANSI C    | 1989          | ANSI X3.159-1989       | First official standard, unified C language implementation           |
| C90/ISO C     | 1990          | ISO/IEC 9899:1990      | Basically the same as C89, internationalized ISO version             |
| C95           | 1995          | ISO/IEC 9899/AMD1:1995 | Correction to C90, added internationalization support                |
| C99           | 1999          | ISO/IEC 9899:1999      | Major update, added many modern features                             |
| C11           | 2011          | ISO/IEC 9899:2011      | Added multithreading support, improved security                      |
| C17/C18       | 2018          | ISO/IEC 9899:2018      | Technical correction to C11, no major new features                   |
| C23           | Expected 2023 | ISO/IEC 9899:2023      | Added new features, simplified programming, improved security        |

## III. Detailed Explanation of Major C Standards

### K&R C (1978)

- **Historical Position**: Created by Dennis Ritchie, described by Brian Kernighan and Ritchie in "The C Programming Language"
- **Main Features**:
  - Basic data types: `char`, `int`, `float`, `double`
  - Basic control structures: `if-else`, `while`, `for`, `do-while`
  - Function declarations do not include parameter type information
- **Limitations**:
  - Lack of strict type checking
  - Lack of standard library definitions
  - Large differences in compiler implementations

### C89/C90 (ANSI C/ISO C)

- **Importance**: The first formally standardized version of C language, laid the foundation for modern C
- **Main Features**:
  - Introduced function prototypes (function declarations with parameter types)
  - Unified standard library: `<stdio.h>`, `<stdlib.h>`, etc.
  - `const` and `volatile` qualifiers
  - Standardized preprocessing directives
- **Code Example**:

  ```c
  /* C89 function prototype */
  int add(int a, int b);  // Explicit parameter type declaration

  /* K&R style */
  int old_add(a, b)
  int a, b;
  {
      return a + b;
  }
  ```

### C99 (1999)

- **Key Improvements**: Modernized C language, introduced many new features
- **Main Features**:
  - `//` single-line comment support
  - `inline` functions
  - Variable-length arrays (VLA)
  - Compound literals
  - Designated initializers
  - Enhanced integer types: `long long`, `_Bool`
  - `<stdint.h>` header (defines integer types of exact width like `int32_t`)
  - New standard libraries: complex number support (`<complex.h>`)
  - C++ style variable declarations (no need to declare at the beginning of a code block)
- **Code Example**:

  ```c
  // C99 single-line comment

  /* Variable-length arrays */
  void process(int n) {
      int array[n];  // Size determined by runtime variable

      /* Variable declaration inside loop */
      for (int i = 0; i < n; i++) {
          array[i] = i;
      }
  }

  /* Designated initializers */
  struct Point {
      int x;
      int y;
  };

  struct Point p = { .y = 2, .x = 1 }; // Initialize by name
  ```

### C11 (2011)

- **Core Improvements**: Added multithreading support and enhanced security
- **Main Features**:
  - Multithreading support: `<threads.h>`
  - Atomic operations: `<stdatomic.h>`
  - Generic type aliases: Extensions in `<stdint.h>`
  - Anonymous structures and unions
  - Static assertions: `_Static_assert`
  - Memory alignment control: `_Alignas`, `_Alignof`
  - Unicode support: `char16_t` and `char32_t`
  - `_Generic` generic selectors
  - `_Noreturn` function specifier
  - Conditional test macros for optional compiler features
- **Code Example**:

  ```c
  #include <threads.h>
  #include <stdio.h>

  int thread_func(void* arg) {
      printf("Thread executing\n");
      return 0;
  }

  int main(void) {
      thrd_t thread;
      thrd_create(&thread, thread_func, NULL);
      thrd_join(thread, NULL);

      /* Static assertion */
      _Static_assert(sizeof(int) >= 4, "Int too small");

      /* Anonymous structure */
      struct {
          int x;
          struct {
              int y, z;
          }; // Anonymous structure
      } point;

      point.x = 1;
      point.y = 2; // Direct access to inner anonymous structure member

      return 0;
  }
  ```

### C17/C18 (2018)

- **Nature**: Technical correction version of C11, no major new features
- **Main Changes**:
  - Eliminated known defects and ambiguities
  - Improved explanation of existing features
  - Removed some outdated features

### C23 (Expected 2023)

- **Expected Features**:
  - Binary literals: `0b101010`
  - UTF-8 string literals: `u8"UTF-8 string"`
  - Added reserved words: `true`, `false`, `nullptr`, `constexpr`
  - `#elifdef` and `#elifndef` preprocessing directives
  - Simpler indexing method for multidimensional arrays
  - Type inference: `auto` (similar to C++)
  - Improved structure initialization syntax
  - Enhanced C++ compatibility

## IV. Evolution of the Standard Library

| Standard | Major New Libraries                                   | Functionality                                                                            |
| -------- | ----------------------------------------------------- | ---------------------------------------------------------------------------------------- |
| C89/90   | `<stdlib.h>`, `<string.h>`, etc.                      | Basic function libraries                                                                 |
| C95      | `<wchar.h>`, `<wctype.h>`                             | Wide character and internationalization support                                          |
| C99      | `<complex.h>`, `<fenv.h>`, `<tgmath.h>`, `<stdint.h>` | Complex number operations, floating-point environment control, fixed-width integer types |
| C11      | `<threads.h>`, `<stdatomic.h>`, `<uchar.h>`           | Multithreading, atomic operations, Unicode encoding conversion                           |
| C17/18   | No major additions                                    | Corrections and clarifications to existing libraries                                     |
| C23      | Expected memory function safety enhancements, etc.    | Improved memory safety                                                                   |

## V. Compiler Support for C Standards

| Compiler    | C89/C90      | C99                 | C11              | C17/C18         | C23                         |
| ----------- | ------------ | ------------------- | ---------------- | --------------- | --------------------------- |
| GCC         | Full support | Almost full support | Mostly supported | Supported       | Partially in implementation |
| Clang       | Full support | Almost full support | Mostly supported | Supported       | Partially in implementation |
| MSVC        | Full support | Partial support     | Partial support  | Partial support | In progress                 |
| ICC (Intel) | Full support | Almost full support | Mostly supported | Supported       | In progress                 |

## VI. Practical Tools and Commands

### Compilation Options to Specify C Standard:

```bash
# GCC compiler
gcc -std=c89 mycode.c -o mycode    # Use C89 standard
gcc -std=c99 mycode.c -o mycode    # Use C99 standard
gcc -std=c11 mycode.c -o mycode    # Use C11 standard
gcc -std=c17 mycode.c -o mycode    # Use C17 standard
gcc -std=c2x mycode.c -o mycode    # Use C23 draft features

# Clang compiler
clang -std=c89 mycode.c -o mycode  # Similar usage as GCC
clang -std=c11 mycode.c -o mycode  # Use C11 standard

# MSVC compiler
cl /std:c11 mycode.c     # Specify C11 standard (supported in newer versions)
```

### Check Features Supported by the Compiler:

```c
#if __STDC_VERSION__ >= 201112L
    // Code supported by C11 or newer standards
#elif __STDC_VERSION__ >= 199901L
    // Code supported by C99
#else
    // C89/C90 code
#endif
```

### Check the C Standard Used by the Current Compiler:

```bash
gcc -dM -E - < /dev/null | grep __STDC_VERSION__
```

## VII. Frequently Asked Questions (FAQ)

### Q1: How do I determine if my code complies with a specific C standard?

You can verify by specifying the standard during compilation (e.g., `-std=c99`) and checking for warnings and errors. Many static analysis tools can also check code compliance with specific standards.

### Q2: Is there backward compatibility between different standards?

Overall, new standards mostly maintain backward compatibility, but there are exceptions. For instance, C99 introduced new keywords that might conflict with identifiers in C89 programs.

### Q3: Why can some code only be compiled under specific standards?

Different standards support different language features. For example, C99 introduced variable-length arrays; if you use this feature, your code cannot be compiled in C89 mode.

### Q4: What are the differences in C standard support between Windows and Unix/Linux systems?

Traditionally, Windows' MSVC compiler has had weaker support for C99 and C11, while GCC/Clang on Unix/Linux have more comprehensive support for newer standards. However, in recent years, MSVC has been improving its C standard support.

### Q5: What is "implementation-defined behavior"?

The C standard sometimes allows compiler implementers to determine certain behaviors themselves; this is "implementation-defined behavior." For example, the exact size of the `int` type is implementation-defined (as long as it's at least 16 bits).

### Q6: Which version of the C language should I learn?

If you're just starting to learn C, it's recommended to begin with C99 or C11, as these versions are more modern and have more friendly syntax. If it's for a specific project, you should follow the standard already used in the project.

## VIII. Factors to Consider When Choosing a C Standard

### Compatibility Considerations

- **Target Platform**: Some platforms may only support specific versions of the standard
- **Existing Code Base**: When interacting with old code, you might need to use earlier standards
- **Third-Party Libraries**: Libraries used may require specific standard versions

### Functional Requirements

- **Multithreading**: Need for native thread support should choose C11 or higher
- **Unicode Support**: C11 provides better Unicode support
- **Variable-Length Arrays**: Introduced in C99 (became optional in C11)
- **Designated Initializers**: Require C99 or higher

### Development Efficiency

- Newer standards typically provide more convenient features, such as single-line comments and in-block variable declarations in C99
- C11's anonymous structures make data organization more flexible
- C23 will provide more convenience features, reducing boilerplate code

## IX. Practical Application Scenarios of C Language Standards

| Standard | Applicability                           | Typical Application Scenarios                                                                         |
| -------- | --------------------------------------- | ----------------------------------------------------------------------------------------------------- |
| C89/C90  | Outdated, but still has applications    | Embedded systems, legacy system maintenance, scenarios where compatibility is extremely important     |
| C99      | Moderately modern, widely supported     | Commonly used in embedded development, scientific computing, projects requiring more flexible syntax  |
| C11      | Modern standard, mainstream usage       | Multithreaded applications, systems concerned with security and atomic operations, modern development |
| C17/C18  | Similar to C11, fixed issues            | Projects with high stability requirements, similar to C11                                             |
| C23      | Latest standard, not yet widely applied | Cutting-edge projects, codebases coexisting with modern C++, experimental projects                    |

## X. Future Trends

- **Enhanced Security**: New standards increasingly emphasize memory safety and prevention of common vulnerabilities
- **Co-evolution with C++**: C and C++ standards are converging in certain aspects
- **Modular Support**: Features similar to C++ modules might be introduced in future versions
- **Parallel Computing Support**: Enhanced native support for multi-core and distributed computing
- **Maintaining Backward Compatibility**: C language's design philosophy is not to break existing code

## XI. How to Quickly Determine the C Standard Being Used

| Content to Determine                  | Command or Tool                                     |
| ------------------------------------- | --------------------------------------------------- |
| Default standard of current compiler  | `gcc -dM -E - < /dev/null \| grep __STDC_VERSION__` |
| Standard features used in source file | Static analysis tools like Clang-Tidy               |
| Check standard compatibility          | Try compiling with different standard options       |
| Preprocessor version check            | Use the `__STDC_VERSION__` macro                    |

## Recommended Further Reading

- "C Programming Language" (K&R, 2nd edition updated to ANSI C)
- "C: A Reference Manual" (Harbison & Steele)
- "Modern C" (Jens Gustedt)
- "Extreme C" (Kamran Amini)
- ISO/IEC official C language standard documents
- [C Standards Committee Working Draft Documents](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf) (C11 draft, free)

## Conclusion

The evolution of C language standards reflects how programming languages need to adapt to new computing needs while maintaining their core design principles. From K&R C in 1978 to the upcoming C23, C language has always maintained its design simplicity, operational efficiency, and closeness to hardware, while gradually adding more safety features and modern programming conveniences.

Understanding different C standards not only helps write more compatible and efficient code but is also an important part of understanding the history of programming language development. Whether maintaining legacy systems or developing cutting-edge applications, choosing the appropriate C language standard is a crucial technical decision.
