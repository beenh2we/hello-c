#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes for function pointer demonstration
int add(int a, int b);
int sub(int a, int b);
int mul(int a, int b);

int main()
{
    printf("\n=============== DYNAMIC MEMORY AND ADVANCED POINTERS ===============\n");

    /*** 1. STRING POINTERS VS ARRAYS ***/
    printf("\n----- STRING MEMORY MANAGEMENT -----\n");
    char message[] = "Hello";  // Array copy on stack
    char *c_ptr_1 = "Hello";   // Points to read-only string literal
    char *c_ptr_2 = message;   // Points to mutable array on stack

    printf("message: %p (stack-based array)\n", message);
    printf("c_ptr_1: %p (read-only string literal)\n", c_ptr_1);
    printf("c_ptr_2: %p (points to stack array)\n", c_ptr_2);

    // Safe modifications
    message[0] = 'h';                           // OK - modifying mutable array
    c_ptr_2[1] = 'a';                           // OK - modifies message array through pointer
    printf("Modified message: %s\n", message);  // "hallo"

    // Unsafe operations (commented out to prevent crashes)
    // c_ptr_1[0] = 'h';     // DANGER: Modifying string literal - undefined behavior

    /*** 2. DYNAMIC MEMORY ALLOCATION ***/
    printf("\n----- MALLOC: DYNAMIC ALLOCATION -----\n");
    int size = 10;
    int *m_ptr = (int *) malloc(size * sizeof(int));

    // Always check allocation success
    if (m_ptr == NULL)
    {
        printf("Memory allocation failed! Exiting...\n");
        return 1;
    }

    // Initialize memory and demonstrate usage
    memset(m_ptr, -1, size * sizeof(int));  // Set all bytes to pattern
    printf("After memset to -1: ");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", m_ptr[i]);
    }
    printf("\n");

    // Modify using array notation
    printf("After setting values: ");
    for (int i = 0; i < size; i++)
    {
        m_ptr[i] = i * 10;
        printf("%d ", m_ptr[i]);
    }
    printf("\n");

    // Access using pointer arithmetic
    printf("Using pointer arithmetic: ");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", *(m_ptr + i));
    }
    printf("\n");

    // Always free when done
    free(m_ptr);
    m_ptr = NULL;  // Prevent use-after-free bugs

    /*** 3. CALLOC & REALLOC ***/
    printf("\n----- CALLOC: ZERO-INITIALIZED ALLOCATION -----\n");
    int *c_ptr = (int *) calloc(size, sizeof(int));

    if (c_ptr == NULL)
    {
        printf("calloc failed! Exiting...\n");
        return 1;
    }

    printf("calloc initializes to zero: ");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", c_ptr[i]);
    }
    printf("\n");

    printf("\n----- REALLOC: RESIZE MEMORY ALLOCATION -----\n");
    // Initialize the first portion with values
    for (int i = 0; i < size; i++)
    {
        c_ptr[i] = i + 100;
    }

    // Expand the allocation
    int new_size = 2 * size;
    int *re_ptr = (int *) realloc(c_ptr, new_size * sizeof(int));

    if (re_ptr == NULL)
    {
        printf("realloc failed! Freeing original memory...\n");
        free(c_ptr);
        return 1;
    }

    // Note: c_ptr may now be invalid! Use re_ptr instead
    c_ptr = re_ptr;  // Update pointer if needed

    printf("After realloc (original values preserved): ");
    for (int i = 0; i < new_size; i++)
    {
        if (i >= size)
        {
            c_ptr[i] = i + 200;  // Initialize new portion
        }
        printf("%d ", c_ptr[i]);
    }
    printf("\n");

    free(c_ptr);  // Free the reallocated memory
    c_ptr = NULL;

    /*** 4. FUNCTION POINTERS ***/
    printf("\n----- FUNCTION POINTERS -----\n");
    int a = 10, b = 34;

    // Declare function pointer
    int (*operation)(int, int);

    // Assign & call the add function
    operation = add;
    printf("Using function pointer: %d + %d = %d\n", a, b, operation(a, b));

    // Assign & call the subtract function
    operation = sub;
    printf("Using function pointer: %d - %d = %d\n", a, b, operation(a, b));

    // Function pointer arrays
    int (*math_ops[3])(int, int) = {add, sub, mul};
    char *op_names[] = {"Addition", "Subtraction", "Multiplication"};

    printf("\nFunction pointer array demonstration:\n");
    for (int i = 0; i < 3; i++)
    {
        printf("%s: %d %s %d = %d\n",
               op_names[i],
               a,
               (i == 0 ? "+" : (i == 1 ? "-" : "*")),
               b,
               math_ops[i](a, b));
    }

    /*** 5. VOID POINTERS ***/
    printf("\n----- VOID POINTERS -----\n");
    void *generic;
    int num = 99;
    char ch = 'A';
    double pi = 3.14159;

    // Void pointer with integers
    generic = &num;
    printf("Void pointer to int: %d\n", *(int *) generic);

    // Void pointer with characters
    generic = &ch;
    printf("Void pointer to char: %c\n", *(char *) generic);

    // Void pointer with doubles
    generic = &pi;
    printf("Void pointer to double: %.5f\n", *(double *) generic);

    printf("\n================================================================\n");
    printf("MEMORY MANAGEMENT BEST PRACTICES:\n");
    printf("• Always check return values from malloc/calloc/realloc\n");
    printf("• Always free dynamically allocated memory when done\n");
    printf("• Set pointers to NULL after freeing\n");
    printf("• Be cautious with string literals vs. mutable strings\n");
    printf("• Use proper casting when working with void pointers\n");
    printf("================================================================\n");

    return 0;
}

// Function implementations
int add(int a, int b)
{
    return a + b;
}

int sub(int a, int b)
{
    return a - b;
}

int mul(int a, int b)
{
    return a * b;
}
