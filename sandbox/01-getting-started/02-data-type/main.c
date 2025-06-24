#include <float.h>
#include <limits.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    // Character
    char c = 'A';
    size_t size_char = sizeof(char);
    printf("char: %c,               size: %zu bytes - [%d, %d]\n",
           c,
           size_char,
           CHAR_MIN,
           CHAR_MAX);

    unsigned char uc = 'B';
    size_t size_uchar = sizeof(unsigned char);
    printf("unsigned char: %c,      size: %zu bytes - [%d, %d]\n",
           uc,
           size_uchar,
           0,
           UCHAR_MAX);

    // Integer
    int i = 128;
    size_t size_int = sizeof(int);
    printf("int: %d,                size: %zu bytes - [%d, %d]\n",
           i,
           size_int,
           INT_MIN,
           INT_MAX);

    unsigned int ui = -128;
    size_t size_uint = sizeof(unsigned int);
    printf("unsigned int: %d,      size: %zu bytes - [%d, %d]\n",
           ui,
           size_int,
           0,
           UINT_MAX);

    short s = 32767;
    size_t size_short = sizeof(short);
    printf("short: %d,             size: %zu bytes - [%d, %d]\n",
           s,
           size_short,
           SHRT_MIN,
           SHRT_MAX);

    unsigned short us = 65535;
    size_t size_ushort = sizeof(unsigned short);
    printf("unsigned short: %d,    size: %zu bytes - [%d, %d]\n",
           us,
           size_ushort,
           0,
           USHRT_MAX);

    long l = 2147483647L;
    size_t size_long = sizeof(long);
    printf("long: %ld,              size: %zu bytes - [%ld, %ld]\n",
           l,
           size_long,
           LONG_MIN,
           LONG_MAX);

    unsigned long ul = 6854775807UL;
    size_t size_ulong = sizeof(unsigned long);
    printf("unsigned long: %lu,     size: %zu bytes - [%d, %lu]\n",
           ul,
           size_ulong,
           0,
           ULONG_MAX);

    // Floating Point
    float f = 3.1415926f;
    size_t size_float = sizeof(float);
    printf("float: %f,              size: %zu bytes - [%e, %e]\n",
           f,
           size_float,
           FLT_MIN,
           FLT_MAX);

    double d = 3.141592653589793;
    size_t size_double = sizeof(double);
    printf("double: %f,             size: %zu bytes - [%e, %e]\n",
           d,
           size_double,
           DBL_MIN,
           DBL_MAX);
    return 0;
}
