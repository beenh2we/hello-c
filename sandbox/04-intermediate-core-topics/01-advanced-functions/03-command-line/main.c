#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int calculate_sum(int argc, char *argv[], int start_index)
{
    int sum = 0;
    for (int i = start_index; i < argc; i++)
    {
        sum += atoi(argv[i]);
    }

    return sum;
}

/*
 * This program demonstrates how to handle command-line arguments in C.
 * Usage examples:
 *   ./program_name                       - No arguments
 *   ./program_name arg1 arg2 arg3        - Simple arguments
 *   ./program_name -n John -a 25         - Flag-style arguments
 *   ./program_name -sum 10 20 30         - Numeric arguments
 */
int main(int argc, char *argv[])
{
    // argc (argument count) is the number of arguments including program name
    // argv (argument vector) is an array of strings containing the arguments

    printf("Program name: %s\n", argv[0]);
    printf("Number of arguments: %d\n\n",
           argc - 1);  // -1 to exclude program name

    if (argc > 1) printf("All arguments: \n");
    for (int i = 1; i < argc; i++)
    {
        printf("    argv[%d]: %s\n", i, argv[i]);
    }
    printf("\n");

    // Parse flag-style arguments
    for (int i = 1; i < argc; i++)
    {
        // Check for flags
        if (argv[i][0] == '-')
        {
            // Check specific flags
            if (strcmp(argv[i], "-n") == 0 && i + 1 < argc)
            {
                printf("Name flag found: %s\n", argv[i + 1]);
                i++;  // Skip the next argument as we've processed it
            }
            else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc)
            {
                int age = atoi(argv[i + 1]);
                printf("Age flag found: %d\n", age);
                i++;
            }
            else if (strcmp(argv[i], "-sum") == 0)
            {
                int sum = calculate_sum(argc, argv, i + 1);
                printf("Sum of numbers: %d\n", sum);
                break;  // Exit the loop as we've processed all remaining args
            }
        }
    }

    return 0;
}
