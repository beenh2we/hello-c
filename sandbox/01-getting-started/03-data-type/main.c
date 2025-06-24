#include <float.h>
#include <limits.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    // Character types
    char c = 'A';  // 1 bytes [-128, 127]
    printf("char: %zu bytes [%d, %d]\n", sizeof(char), CHAR_MIN, CHAR_MAX);

    unsigned char uc = 255;  // 1 bytes [0, 255]
    printf("unsigned char: %zu bytes [0, %u]\n", sizeof(unsigned char), UCHAR_MAX);

    // Integer type
    short s = 32767;  // 2 bytes [-32768, 32767]
    printf("short: %zu bytes [%d, %d]\n", sizeof(short), SHRT_MIN, SHRT_MAX);

    unsigned short us = 65535;  // 2 bytes [0, 65535]
    printf("unsigned short: %zu bytes [0, %u]\n", sizeof(unsigned short), USHRT_MAX);

    int i = 42;  // 4 bytes [-2147483648, 2147483647]
    printf("int: %zu bytes [%d, %d]\n", sizeof(int), INT_MIN, INT_MAX);

    unsigned int ui = 1;  // 4 bytes [0, 4294967295]
    printf("unsigned int: %zu bytes [0, %u]\n", sizeof(unsigned int), UINT_MAX);

    long l = 2147483647;  // 8 bytes [-9223372036854775808, 9223372036854775807]
    printf("long: %zu bytes [%ld, %ld]\n", sizeof(long), LONG_MIN, LONG_MAX);

    unsigned long ul = 6854775807UL;  // 8 bytes [0, 18446744073709551615]
    printf("unsigned long %zu bytes [0, %lu]\n", sizeof(unsigned long), ULONG_MAX);

    long long ll = 9223372036854775807LL;  // 8 bytes [-9223372036854775808, 9223372036854775807]
    printf("long long: %zu bytes [%lld, %lld]\n", sizeof(long long), LLONG_MIN, LLONG_MAX);

    unsigned long long ull = 18446744073709551615ULL;  // 8 bytes [0, 18446744073709551615]
    printf("unsigned long long: %zu bytes [0, %llu]\n", sizeof(unsigned long long), ULLONG_MAX);

    float f = 3.14f;  // 4 bytes [1.175494e-38, 3.402823e+38]
    printf("float: %zu bytes [%e, %e]\n", sizeof(float), FLT_MIN, FLT_MAX);

    double d = 3.141592653589793;  // 8 bytes [2.225074e-308, 1.797693e+308]
    printf("double: %zu bytes [%e, %e]\n", sizeof(double), DBL_MIN, DBL_MAX);

    long double ld = 3.141592653589793238L;  // 16 bytes [3.362103e-4932, 1.189731e+4932]
    printf("long double: %zu bytes [%Le, %Le]\n", sizeof(long double), LDBL_MIN, LDBL_MAX);
    return 0;
}
