#include <stdio.h>

int main(int argc, char const *argv[])
{
    // printf returns the number of characters printed
    int chars_written = printf("Hello World!\n");
    // output 13 (including '\n')
    printf("Number of characters printed: %d\n", chars_written);

    return 0;
}
