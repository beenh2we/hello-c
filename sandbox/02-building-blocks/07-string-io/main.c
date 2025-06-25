#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    printf("\n--- String Input/Output in C ---\n\n");

    // 1. STRING INPUT
    printf("1. STRING INPUT\n");

    char name[20];
    printf("Enter your first name: ");
    fgets(name, sizeof(name), stdin);

    // Remove newline character from fgets
    for (int i = 0; name[i] != '\0'; i++)
    {
        if (name[i] == '\n')
        {
            name[i] = '\0';  // Replace newline with null terminator
            break;
        }
    }
    printf("fgets():        \"%s\" (reads entire line)\n", name);

    // scanf with field width limit
    char lastname[20];
    printf("Enter your last name: ");
    scanf("%19s", lastname);  // %19s prevents buffer overflow
    printf("scanf():        \"%s\" (reads until whitespace)\n", lastname);

    printf("Full name:      \"%s %s\"\n\n", name, lastname);

    // 2. STRING OUTPUT
    printf("2. STRING OUTPUT\n");
    char message[] = "Learning C programming";

    // Method 1: printf
    printf("printf():       \"%s\" (flexible formatting)\n", message);

    // Method 2: puts
    printf("puts():         ");
    puts(message);  // Note: puts adds newline automatically

    // Method 3: putchar (character by character)
    printf("putchar():      ");
    for (int i = 0; message[i] != '\0'; i++)
    {
        putchar(message[i]);
    }
    putchar('\n');
    printf("               (prints character by character)\n");

    return 0;
}
