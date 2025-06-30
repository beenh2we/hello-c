#include <complex.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Basic type-generic macro
#define TYPE_NAME(x)      \
    _Generic((x),         \
        int: "int",       \
        float: "float",   \
        double: "double", \
        char: "char",     \
        char *: "char*",  \
        void *: "void*",  \
        default: "unknown type")

// Type-generic absolute value function
#define ABS(x)            \
    _Generic((x),         \
        int: abs,         \
        long: labs,       \
        long long: llabs, \
        float: fabsf,     \
        double: fabs,     \
        long double: fabsl)(x)

// Type-generic maximum function
#define MAX(a, b)                     \
    _Generic((a),                     \
        int: _Generic((b),            \
            int: max_int,             \
            float: max_int_float,     \
            double: max_int_double,   \
            default: max_int),        \
        float: _Generic((b),          \
            int: max_float_int,       \
            float: max_float,         \
            double: max_float_double, \
            default: max_float),      \
        double: _Generic((b),         \
            int: max_double_int,      \
            float: max_double_float,  \
            double: max_double,       \
            default: max_double),     \
        default: max_int)(a, b)

// Type-specific max implementations
int max_int(int a, int b)
{
    return a > b ? a : b;
}
float max_float(float a, float b)
{
    return a > b ? a : b;
}
double max_double(double a, double b)
{
    return a > b ? a : b;
}

// Mixed-type max implementations
float max_int_float(int a, float b)
{
    return a > b ? a : b;
}
double max_int_double(int a, double b)
{
    return a > b ? a : b;
}
float max_float_int(float a, int b)
{
    return a > b ? a : b;
}
double max_float_double(float a, double b)
{
    return a > b ? a : b;
}
double max_double_int(double a, int b)
{
    return a > b ? a : b;
}
double max_double_float(double a, float b)
{
    return a > b ? a : b;
}

// Type-generic print function
#define PRINT(x)                     \
    _Generic((x),                    \
        int: printf_int,             \
        unsigned int: printf_uint,   \
        long: printf_long,           \
        unsigned long: printf_ulong, \
        float: printf_float,         \
        double: printf_double,       \
        char *: printf_str,          \
        void *: printf_ptr,          \
        default: printf_unknown)(x)

// Type-specific print implementations
void printf_int(int x)
{
    printf("Integer: %d\n", x);
}
void printf_uint(unsigned int x)
{
    printf("Unsigned integer: %u\n", x);
}
void printf_long(long x)
{
    printf("Long: %ld\n", x);
}
void printf_ulong(unsigned long x)
{
    printf("Unsigned long: %lu\n", x);
}
void printf_float(float x)
{
    printf("Float: %f\n", x);
}
void printf_double(double x)
{
    printf("Double: %lf\n", x);
}
void printf_str(char *x)
{
    printf("String: %s\n", x);
}
void printf_ptr(void *x)
{
    printf("Pointer: %p\n", x);
}
void printf_unknown(void *x)
{
    printf("Unknown type at: %p\n", (void *) x);
}

// Basic _Generic demonstration
void basic_generic_demo()
{
    printf("=== Basic _Generic Demonstration ===\n");

    int i = 10;
    float f = 20.5f;
    double d = 30.25;
    char c = 'A';
    char *s = "Hello";
    void *p = &i;

    printf("i is of type: %s\n", TYPE_NAME(i));
    printf("f is of type: %s\n", TYPE_NAME(f));
    printf("d is of type: %s\n", TYPE_NAME(d));
    printf("c is of type: %s\n", TYPE_NAME(c));
    printf("s is of type: %s\n", TYPE_NAME(s));
    printf("p is of type: %s\n", TYPE_NAME(p));
    printf("42 is of type: %s\n", TYPE_NAME(42));
    printf("42.0 is of type: %s\n", TYPE_NAME(42.0));
}

// Type-generic absolute value function demonstration
void abs_demo()
{
    printf("\n=== Type-Generic Absolute Value ===\n");

    int i = -10;
    long l = -20L;
    long long ll = -30LL;
    float f = -40.5f;
    double d = -50.75;

    printf("abs(%d) = %d\n", i, ABS(i));
    printf("abs(%ld) = %ld\n", l, ABS(l));
    printf("abs(%lld) = %lld\n", ll, ABS(ll));
    printf("abs(%f) = %f\n", f, ABS(f));
    printf("abs(%lf) = %lf\n", d, ABS(d));
}

// Type-generic maximum function demonstration
void max_demo()
{
    printf("\n=== Type-Generic Maximum Function ===\n");

    int i1 = 10, i2 = 20;
    float f1 = 15.5f, f2 = 25.5f;
    double d1 = 30.25, d2 = 40.75;

    // Same-type comparisons
    printf("MAX(%d, %d) = %d\n", i1, i2, MAX(i1, i2));
    printf("MAX(%f, %f) = %f\n", f1, f2, MAX(f1, f2));
    printf("MAX(%lf, %lf) = %lf\n", d1, d2, MAX(d1, d2));

    // Mixed-type comparisons
    printf("MAX(%d, %f) = %f\n", i1, f2, MAX(i1, f2));
    printf("MAX(%f, %d) = %f\n", f1, i2, MAX(f1, i2));
    printf("MAX(%d, %lf) = %lf\n", i1, d2, MAX(i1, d2));
    printf("MAX(%lf, %d) = %lf\n", d1, i2, MAX(d1, i2));
    printf("MAX(%f, %lf) = %lf\n", f1, d2, MAX(f1, d2));
    printf("MAX(%lf, %f) = %lf\n", d1, f2, MAX(d1, f2));
}

// Type-generic print function demonstration
void print_demo()
{
    printf("\n=== Type-Generic Print Function ===\n");

    int i = 42;
    unsigned int ui = 123u;
    long l = 1000L;
    unsigned long ul = 2000UL;
    float f = 3.14f;
    double d = 2.71828;
    char *s = "Type-generic programming";
    void *p = &i;

    PRINT(i);
    PRINT(ui);
    PRINT(l);
    PRINT(ul);
    PRINT(f);
    PRINT(d);
    PRINT(s);
    PRINT(p);
}

// Creating a complete type-generic math library
void generic_math_library()
{
    printf("\n=== Type-Generic Math Library ===\n");

// Type-generic sine function
#define SIN(x) \
    _Generic((x), float: sinf, double: sin, long double: sinl, default: sin)(x)

// Type-generic cosine function
#define COS(x) \
    _Generic((x), float: cosf, double: cos, long double: cosl, default: cos)(x)

// Type-generic square root function
#define SQRT(x)             \
    _Generic((x),           \
        float: sqrtf,       \
        double: sqrt,       \
        long double: sqrtl, \
        default: sqrt)(x)

    // Testing our generic math functions
    float f = 1.0f;
    double d = 2.0;

    printf("sin(%f) = %f\n", f, SIN(f));
    printf("sin(%lf) = %lf\n", d, SIN(d));

    printf("cos(%f) = %f\n", f, COS(f));
    printf("cos(%lf) = %lf\n", d, COS(d));

    printf("sqrt(%f) = %f\n", f, SQRT(f));
    printf("sqrt(%lf) = %lf\n", d, SQRT(d));
}

int main()
{
    printf("==== TYPE GENERIC PROGRAMMING WITH _GENERIC ====\n\n");

    basic_generic_demo();
    abs_demo();
    max_demo();
    print_demo();
    generic_math_library();

    return 0;
}
