# C Data Types

## Introduction

Data types in C define the type of data a variable can hold and how the computer interprets bit patterns in memory. Understanding C's type system is fundamental to writing efficient and correct C programs.

## Basic Data Types

### Integer Types

```c
char a;        // Typically 1 byte
short b;       // Typically 2 bytes
int c;         // Typically 4 bytes (platform dependent)
long d;        // 4 bytes on 32-bit systems, 8 bytes on 64-bit Unix/Linux
long long e;   // At least 8 bytes (C99 and later)
```

### Floating-Point Types

```c
float f;         // Single precision, typically 4 bytes
double g;        // Double precision, typically 8 bytes
long double h;   // Extended precision (size varies by platform)
```

### Character Types

```c
char ch = 'A';            // Character literal
char str[] = "Hello";     // String literal (array of chars)
```

### Boolean Type (C99 and later)

```c
#include <stdbool.h>

bool flag = true;   // Evaluates to 1
flag = false;       // Evaluates to 0
```

## Derived Data Types

### Arrays

```c
int numbers[10];           // Array of 10 integers
char name[50];             // Array of 50 characters
int matrix[3][4];          // 2D array (3 rows, 4 columns)
```

### Pointers

```c
int *p;              // Pointer to an integer
char *str;           // Pointer to a character (often used for strings)
void *ptr;           // Generic pointer (no specific type)
```

### Structures

```c
struct Person {
    char name[50];
    int age;
    float height;
};

struct Person person1 = {"John", 30, 5.9};
```

### Unions

```c
union Data {
    int i;
    float f;
    char str[20];
};

union Data data;
data.i = 10;    // Only i has a valid value now
```

### Enumerations

```c
enum Days {SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY};
enum Days today = WEDNESDAY;
```

### Typedef

```c
typedef unsigned long ulong;
ulong counter = 0;  // Same as: unsigned long counter = 0;

typedef struct {
    char name[50];
    int age;
} Person;

Person p1;  // No need for 'struct' keyword
```

## Type Modifiers

### Size Modifiers

```c
short int a;         // Short integer (at least 16 bits)
long int b;          // Long integer (at least 32 bits)
long long int c;     // Long long integer (at least 64 bits)
```

### Sign Modifiers

```c
signed int a;         // Same as 'int', can represent negative numbers
unsigned int b;       // Only positive numbers and zero, double the positive range
signed char c;        // Explicitly signed character (-128 to 127 typically)
unsigned char d;      // Unsigned character (0 to 255 typically)
```

## Type Qualifiers

```c
const int MAX = 100;           // Value cannot be changed
volatile int status;           // May change unexpectedly
restrict int *ptr;             // Only this pointer accesses the data (C99)
```

## Fixed-Width Integer Types

From `<stdint.h>` (C99 and later):

```c
#include <stdint.h>

int8_t a;         // Exactly 8 bits signed
uint8_t b;        // Exactly 8 bits unsigned
int16_t c;        // Exactly 16 bits signed
uint16_t d;       // Exactly 16 bits unsigned
int32_t e;        // Exactly 32 bits signed
uint32_t f;       // Exactly 32 bits unsigned
int64_t g;        // Exactly 64 bits signed
uint64_t h;       // Exactly 64 bits unsigned

// Minimum-width types
int_least8_t i;
uint_least8_t j;

// Fast minimum-width types
int_fast8_t k;
uint_fast8_t l;

// Integer type capable of holding pointer
intptr_t m;
uintptr_t n;

// Maximum width integer types
intmax_t o;
uintmax_t p;
```

## Type Conversions

### Implicit Conversions

```c
int i = 10;
float f = i;    // Implicit conversion from int to float

char c = 'A';
int j = c;      // Char is promoted to int

// In expressions, smaller types are promoted to int
short s1 = 10, s2 = 20;
int result = s1 + s2;  // s1 and s2 are promoted to int before addition
```

### Explicit Conversions (Casting)

```c
float f = 3.14;
int i = (int)f;    // Explicitly cast float to int (becomes 3)

// Pointer casting
void *generic = malloc(10);
char *str = (char *)generic;  // Cast void* to char*
```

## Memory Alignment and Padding

```c
struct Aligned {
    char c;     // 1 byte
    int i;      // 4 bytes, but may start at offset 4 (with 3 padding bytes)
    short s;    // 2 bytes, may have padding after
};  // Total size might be 12 bytes due to alignment

// Force packed structure (no padding)
struct __attribute__((packed)) Packed {
    char c;     // 1 byte
    int i;      // 4 bytes, starts at offset 1
    short s;    // 2 bytes, starts at offset 5
};  // Total size will be 7 bytes (platform-dependent)

// Control alignment with pragmas
#pragma pack(push, 1)
struct Packed2 {
    char c;
    int i;
    short s;
}; // Also packed with 1-byte alignment
#pragma pack(pop)
```

## Common Pitfalls and Best Practices

### Integer Overflow

```c
unsigned char byte = 255;
byte++;  // Wraps around to 0 (expected for unsigned)

int i = INT_MAX;
i++;     // Undefined behavior for signed types!
```

### Division and Modulo

```c
int a = 5, b = 2;
float result = a / b;      // Result is 2.0, not 2.5 (integer division)
float correct = (float)a / b;  // Result is 2.5 (converts first operand to float)

int remainder = a % b;     // Modulo operation: 5 % 2 = 1
```

### Type Size Checking

```c
#include <stdio.h>

void print_sizes(void) {
    printf("char: %zu bytes\n", sizeof(char));
    printf("int: %zu bytes\n", sizeof(int));
    printf("long: %zu bytes\n", sizeof(long));
    printf("long long: %zu bytes\n", sizeof(long long));
    printf("float: %zu bytes\n", sizeof(float));
    printf("double: %zu bytes\n", sizeof(double));
    printf("pointer: %zu bytes\n", sizeof(void *));
}
```

### Best Practices

1. Use `size_t` for sizes and array indices
2. Use fixed-width types for portable code
3. Check for integer overflow in critical applications
4. Be explicit about sign conversions
5. Use proper specifiers in printf/scanf functions
6. Remember that C is weakly typed - check your conversions

## Platform-Specific Size Reference

| Data Type        | 32-bit x86 | 64-bit Linux/macOS | 64-bit Windows | ARM 32-bit | ARM 64-bit |
| ---------------- | ---------- | ------------------ | -------------- | ---------- | ---------- |
| char             | 1 byte     | 1 byte             | 1 byte         | 1 byte     | 1 byte     |
| short            | 2 bytes    | 2 bytes            | 2 bytes        | 2 bytes    | 2 bytes    |
| int              | 4 bytes    | 4 bytes            | 4 bytes        | 4 bytes    | 4 bytes    |
| long             | 4 bytes    | 8 bytes            | 4 bytes        | 4 bytes    | 8 bytes    |
| long long        | 8 bytes    | 8 bytes            | 8 bytes        | 8 bytes    | 8 bytes    |
| float            | 4 bytes    | 4 bytes            | 4 bytes        | 4 bytes    | 4 bytes    |
| double           | 8 bytes    | 8 bytes            | 8 bytes        | 8 bytes    | 8 bytes    |
| long double      | 12 bytes   | 16 bytes           | 8 bytes        | 8 bytes    | 16 bytes   |
| pointer (void\*) | 4 bytes    | 8 bytes            | 8 bytes        | 4 bytes    | 8 bytes    |
| size_t           | 4 bytes    | 8 bytes            | 8 bytes        | 4 bytes    | 8 bytes    |
