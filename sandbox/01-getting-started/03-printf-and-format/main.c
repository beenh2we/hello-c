#include <stdio.h>

int main(int argc, char const *argv[])
{
    // Basic Integer Formatting
    printf("|%d|\n", 42);
    printf("|%d|\n", -37);

    // Using + flag to always show sign for positive numbers
    printf("|%+d|\n", 42);
    printf("|%+d|\n", -37);

    printf("\n");

    // Floating Point Formatting
    // Default float format shows 6 decimal places
    printf("|%f|\n", 3.1415926);
    // Precision controls with precision 2
    printf("|%.2f|\n", 3.1415926);

    printf("\n");

    // Different Number Bases
    // Octal (base 8) representation
    printf("|%o|\n", 255);
    // Hexadecimal (base 16) representation (uppercase)
    printf("|%X|\n", 255);
    // Using # flag adds the 0x/0X prefix for hex values
    printf("|%#X|\n", 255);

    printf("\n");

    // Width and Alignment Control
    printf("|%d|\n", 42);
    // Width of 3, right-aligned by default
    printf("|%3d|\n", 42);
    // - flag (left-align) has no effect without width
    printf("|%-d|)\n", 42);
    // - flag with width creates left alignment
    printf("|%-3d|\n", 42);

    printf("\n");

    // Spacing and Padding Options
    // Space flag adds a space
    printf("|% d|\n", 42);
    printf("|% d|)\n", -37);
    // Space flag combined with width
    printf("|% 10d|\n", 128);
    // 0 flag pads with zeros instead of spaces
    printf("|%010d|\n", 128);
    // * lets you specify width as a parameter
    printf("|%*d|\n", 10, 42);

    printf("\n");

    // Precision for Integers
    // Precision sets minimum number of digits (minimum 4 digits, zero-padded)
    printf("|%.4d|\n", 42);
    // Combining width and precision
    printf("|%10.4d|\n", 42);
    // Width, precision and + flag together
    printf("|%+10.4d|\n", 42);

    printf("\n");

    // Variable Width and Precision
    int width = 10;
    int precision = 3;
    // Both width and precision can be specified as arguments using *
    printf("|%*.*f|\n", width, precision, 3.1415926);

    printf("\n");

    // Pointer and String Formatting
    char *name = "beenh2we";
    // %p shows pointer address in hex
    printf("|%p|\n", name);
    // String with fixed width (right-aligned)
    printf("|%10s|\n", name);
    // String with fixed width (left-aligned)
    printf("|%-10s|\n", name);
    // Precision limits maximum characters printed (max 4 chars)
    printf("|%.4s|\n", name);
    return 0;
}
