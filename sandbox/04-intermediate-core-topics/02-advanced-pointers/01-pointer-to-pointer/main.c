#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void basic_double_pointer_example()
{
    printf("--- Basic Double Pointer Example ---\n");
    int value = 42;     // Regular variable
    int *ptr = &value;  // Pointer to variable
    int **pptr = &ptr;  // Pointer to pointer

    printf("value: %d\n", value);
    printf("*ptr: %d\n", *ptr);
    printf("**pptr: %d\n", **pptr);

    // Modifying the value through different levels of indirection
    **pptr = 100;
    printf("After modification, value = %d\n\n", value);

    // Memory addresses visualization
    printf("Memory addresses:\n");
    printf("&value: %p  (address of the variable)\n", (void *) &value);
    printf("ptr: %p     (value of the pointer = address of the variable)\n",
           (void *) ptr);
    printf("&ptr: %p    (address of the pointer)\n", (void *) &ptr);
    printf("pptr: %p    (value of pointer-to-pointer = address of pointer)\n\n",
           (void *) pptr);
}

// Function to dynamically allocate a 2D array using pointers to pointers
int **allocate_2d_array(int rows, int cols)
{
    int **matrix = (int **) malloc(rows * sizeof(int *));

    if (matrix == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }

    for (int i = 0; i < rows; i++)
    {
        matrix[i] = (int *) malloc(cols * sizeof(int));
        if (matrix[i] == NULL)
        {
            // Free already allocated memory before returning
            for (int j = 0; j < i; j++)
            {
                free(matrix[j]);
            }
            free(matrix);
            printf("Memory allocation failed for row %d\n", i);
            return NULL;
        }
        // Initialize with values
        for (int j = 0; j < cols; j++)
        {
            matrix[i][j] = i * cols + j + 1;
        }
    }
    return matrix;
}

// Function to free a dynamically allocated 2D array
void free_2d_array(int **matrix, int rows)
{
    for (int i = 0; i < rows; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

// Function to print a 2D array
void print_2d_array(int **matrix, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%3d", matrix[i][j]);
        }
        printf("\n");
    }
}

// Function to demonstrate array of strings using pointer to pointers
void array_of_strings_example()
{
    printf("\n--- Array of Strings Example ---\n");

    // Array of string literals (stored in read-only memory)
    const char *fruits[] = {"Apple", "Banana", "Cherry", "Date", "Elderberry"};

    printf("String array using constant pointers:\n");
    for (int i = 0; i < 5; i++)
    {
        printf("fruits[%d] = %s\n", i, fruits[i]);
    }

    // Dynamic allocation of string array
    printf("\nString array using dynamically allocated memory:\n");
    char **names = (char **) malloc(3 * sizeof(char *));

    // Allocate and copy strings
    names[0] = strdup("John");  // strdup allocates memory and copies the string
    names[1] = strdup("Alice");
    names[2] = strdup("Bob");

    // Modify a string
    free(names[0]);               // Free the old string
    names[0] = strdup("Johnny");  // Allocate a new one

    printf("\nAfter modification:\n");
    printf("names[0] = %s\n", names[0]);

    // Free all allocated strings
    for (int i = 0; i < 3; i++)
    {
        free(names[i]);
    }
    free(names);
}

// Function to swap integers using double pointer
void swap_int_ptr(int **a, int **b)
{
    int *temp = *a;
    *a = *b;
    *b = temp;
}

int main(int argc, char *argv[])
{
    // Basic example
    basic_double_pointer_example();

    // 2D array example
    printf("\n--- 2D Array Example ---\n");
    int rows = 4;
    int cols = 5;

    int **matrix = allocate_2d_array(rows, cols);
    if (matrix != NULL)
    {
        printf("2D Array (%dx%d):\n", rows, cols);
        print_2d_array(matrix, rows, cols);
        free_2d_array(matrix, rows);
    }

    // Array of strings example
    array_of_strings_example();

    // Pointer swapping example
    printf("\n--- Pointer Swapping Example ---\n");
    int x = 10, y = 20;
    int *p_x = &x;
    int *p_y = &y;

    printf("Before swap: *p_x = %d, *p_y = %d\n", *p_x, *p_y);
    swap_int_ptr(&p_x, &p_y);
    printf("After swap: *p_x = %d, *p_y = %d\n", *p_x, *p_y);
    return 0;
}
