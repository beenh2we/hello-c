#include <stdio.h>
#include <limits.h>
#include <float.h>

/**
 * char                 1 bytes     [-128, 127]
 * unsigned char        1 bytes     [0, 255]
 * short                2 bytes     [-32768, 32767]
 * unsigned short       2 bytes     [0, 65535]
 * int                  4 bytes     [-2147483648, 2147483647]
 * unsigned int         4 bytes     [0, 4294967295]
 * long                 8 bytes     [-9223372036854775808, 9223372036854775807]
 * unsigned long        8 bytes     [0, 18446744073709551615]
 * long long            8 bytes     [-9223372036854775808, 9223372036854775807]
 * unsigned long long   8 bytes     [0, 18446744073709551615]
 * float                4 bytes     [1.175494e-38, 3.402823e+38]
 * double               8 bytes     [2.225074e-308, 1.797693e+308]
 * long double          16 bytes    [3.362103e-4932, 1.189731e+4932]
 */
int main(int argc, char *argv[]) {
    // Character types
    char c = 'A'; // 1 bytes [-128, 127]
    printf(
        "char: %zu bytes [%d, %d] - Used for characters or small integers\n",
        sizeof(char),
        CHAR_MIN,
        CHAR_MAX
    );

    unsigned char uc = 255; // 1 bytes [0, 255]
    printf(
        "unsigned char: %zu bytes [0, %u] - Used for small positive integers\n",
        sizeof(unsigned char),
        UCHAR_MAX
    );

    // Integer type
    short s = 32767; // 2 bytes [-32768, 32767]
    printf(
        "short: %zu bytes [%d, %d] - Used for small integers\n",
        sizeof(short),
        SHRT_MIN,
        SHRT_MAX
    );

    unsigned short us = 65535; // 2 bytes [0, 65535]
    printf(
        "unsigned short: %zu bytes [0, %u] - Used for small positive integers\n",
        sizeof(unsigned short),
        USHRT_MAX
    );

    int i = 42; // 4 bytes [-2147483648, 2147483647]
    printf(
        "int: %zu bytes [%d, %d] - Standard integer type\n",
        sizeof(int),
        INT_MIN,
        INT_MAX
    );

    unsigned int ui = 1; // 4 bytes [0, 4294967295]
    printf(
        "unsigned int: %zu bytes [0, %u] - Used for larger positive integers\n",
        sizeof(unsigned int),
        UINT_MAX
    );

    long l = 2147483647; // 8 bytes [-9223372036854775808, 9223372036854775807]
    printf(
        "long: %zu bytes [%ld, %ld] - Used for larger integers\n",
        sizeof(long),
        LONG_MIN,
        LONG_MAX
    );

    unsigned long ul = 6854775807UL; // 8 bytes [0, 18446744073709551615]
    printf(
        "unsigned long %zu bytes [0, %lu] -Used for larger positive integers\n",
        sizeof(unsigned long),
        ULONG_MAX
    );

    long long ll = 9223372036854775807LL; // 8 bytes [-9223372036854775808, 9223372036854775807]
    printf(
        "long long: %zu bytes [%lld, %lld] - Used for very large integers\n",
        sizeof(long long),
        LLONG_MIN,
        LLONG_MAX
    );

    unsigned long long ull = 18446744073709551615ULL; // 8 bytes [0, 18446744073709551615]
    printf(
        "unsigned long long: %zu bytes [0, %llu] - Used for very large positive integers\n",
        sizeof(unsigned long long),
        ULLONG_MAX
    );

    float f = 3.14f; // 4 bytes [1.175494e-38, 3.402823e+38]
    printf(
        "float: %zu bytes [%e, %e] - Single precision floating-point\n",
        sizeof(float),
        FLT_MIN,
        FLT_MAX
    );

    double d = 3.141592653589793; // 8 bytes [2.225074e-308, 1.797693e+308]
    printf(
        "double: %zu bytes [%e, %e] - Double precision floating-point\n",
        sizeof(double),
        DBL_MIN,
        DBL_MAX
    );

    long double ld = 3.141592653589793238L; // 16 bytes [3.362103e-4932, 1.189731e+4932]
    printf(
        "long double: %zu bytes [%Le, %Le] - Extended precision floating-point\n",
        sizeof(long double),
        LDBL_MIN,
        LDBL_MAX
    );
    return 0;
}
