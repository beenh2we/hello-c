#include <stdio.h>

int main(int argc, char const *argv[])
{
    // printf returns the number of characters printed
    int chars_written = printf("Hello World!\n");
    // output 13, printf returns the number of characters successfully printed (including '\n')
    printf("Number of characters printed: %d\n", chars_written);

    printf("\n");

    // Basic Integer Formatting
    printf("|%d| <- Standard integer\n", 42);
    printf("|%d| <- Negative integer\n", -37);

    // Using + flag to always show sign
    printf("|%+d| <- With + flag (shows sign for positive numbers)\n", 42);
    printf("|%+d| <- With + flag (negative numbers always show sign)\n", -37);

    printf("\n");

    // Floating Point Formatting
    // Default float format shows 6 decimal places
    printf("|%f| <- Default float format (6 decimal places)\n", 3.1415926);
    // Precision controls decimal places
    printf("|%.2f| <- Float with precision 2\n", 3.1415926);

    printf("\n");

    // Different Number Bases
    // Octal (base 8) representation
    printf("|%o| <- Octal format (255 in base 8)\n", 255);
    // Hexadecimal (base 16) representation
    printf("|%X| <- Hexadecimal format (uppercase)\n", 255);
    // Using # flag adds the 0x/0X prefix for hex values
    printf("|%#X| <- Hex with # flag (adds 0X prefix)\n", 255);

    printf("\n");

    // Width and Alignment Control
    printf("|%d| <- No width specified\n", 42);
    // Width of 3, right-aligned by default
    printf("|%3d| <- Width of 3 (right-aligned)\n", 42);
    // - flag (left-align) has no effect without width
    printf("|%-d| <- - flag alone (no effect without width)\n", 42);
    // - flag with width creates left alignment
    printf("|%-3d| <- Width of 3, left-aligned with - flag\n", 42);

    printf("\n");

    // Spacing and Padding Options
    // Space flag adds a space before positive numbers
    printf("|% d| <- Space flag (adds space before positive numbers)\n", 42);
    printf("|% d| <- Space flag with negative (no additional space needed)\n", -37);
    // Space flag combined with width
    printf("|% 10d| <- Space flag with width 10\n", 128);
    // 0 flag pads with zeros instead of spaces
    printf("|%010d| <- Zero padding with width 10\n", 128);
    // * lets you specify width as a parameter
    printf("|%*d| <- Width specified as argument (10)\n", 10, 42);

    printf("\n");

    // Precision for Integers
    // Precision sets minimum number of digits
    printf("|%.4d| <- Precision 4 (minimum 4 digits, zero-padded)\n", 42);
    // Combining width and precision
    printf("|%10.4d| <- Width 10, precision 4\n", 42);
    // Width, precision and + flag together
    printf("|%+10.4d| <- +flag, width 10, precision 4\n", 42);

    printf("\n");

    // Variable Width and Precision
    int width = 10;
    int precision = 3;
    // Both width and precision can be specified as arguments using *
    printf("|%*.*f| <- Width %d, precision %d from variables\n",
           width,
           precision,
           3.1415926,
           width,
           precision);

    printf("\n");

    // Pointer and String Formatting
    char *name = "beenh2we";
    // %p shows pointer address in hex
    printf("|%p| <- Pointer address\n", name);
    // String with fixed width (right-aligned)
    printf("|%10s| <- String with width 10 (right-aligned)\n", name);
    // String with fixed width (left-aligned)
    printf("|%-10s| <- String with width 10 (left-aligned)\n", name);
    // Precision limits maximum characters printed
    printf("|%.4s| <- String with precision 4 (max 4 chars)\n", name);
    return 0;
}
