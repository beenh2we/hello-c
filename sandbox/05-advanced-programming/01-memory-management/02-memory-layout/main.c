#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global variables (stored in DATA or BSS segments)
int initialized_global = 100;  // DATA segment (initialized)
int uninitialized_global;      // BSS segment (uninitialized)

// Static variables
static int static_initialized = 200;  // DATA segment
static int static_uninitialized;      // BSS segment

// Constants
const int const_global = 300;  // Read-only DATA segment

// String literals
const char *string_literal
    = "Hello";  // String in read-only DATA, pointer in DATA

// Function to demonstrate stack memory
void stack_demo(int param)
{
    // Local variables (stored on the stack)
    int local_var = 500;
    static int static_local = 600;  // DATA segment (persists between calls)

    printf("Stack demo:\n");
    printf("  Parameter: %d at %p\n", param, (void *) &param);
    printf("  Local variable: %d at %p\n", local_var, (void *) &local_var);
    printf("  Static local: %d at %p\n", static_local, (void *) &static_local);

    // Demonstrate stack growth
    if (param > 0)
    {
        printf("\nRecursing with param-1...\n");
        stack_demo(param - 1);
        printf("Back from recursion\n\n");
    }
}

// Function to demonstrate heap memory
void heap_demo(void)
{
    // Dynamic memory (stored on the heap)
    int *heap_int = (int *) malloc(sizeof(int));
    char *heap_str = (char *) malloc(20);

    if (!heap_int || !heap_str)
    {
        printf("Memory allocation failed\n");
        if (heap_int) free(heap_int);
        if (heap_str) free(heap_str);
        return;
    }

    *heap_int = 700;
    strcpy(heap_str, "Dynamic string");

    printf("Heap demo:\n");
    printf("  Heap integer: %d at %p\n", *heap_int, (void *) heap_int);
    printf("  Heap string: %s at %p\n", heap_str, (void *) heap_str);

    free(heap_int);
    free(heap_str);
}

// Function that's not called (to demonstrate code segment)
void unused_function(void)
{
    printf("This function is in the code segment but never executed\n");
}

// Function to print memory addresses
void print_memory_addresses()
{
    printf("Memory segment addresses:\n");
    printf("  Code segment:\n");
    printf("    unused_function(): %p\n", (void *) unused_function);

    printf("  Data segment:\n");
    printf("    initialized_global: %p\n", (void *) &initialized_global);
    printf("    static_initialized: %p\n", (void *) &static_initialized);
    printf("    const_global: %p\n", (void *) &const_global);
    printf("    string_literal pointer: %p\n", (void *) &string_literal);
    printf("    string literal content: %p\n", (void *) string_literal);

    printf("  BSS segment:\n");
    printf("    uninitialized_global: %p\n", (void *) &uninitialized_global);
    printf("    static_uninitialized: %p\n", (void *) &static_uninitialized);
}

int main(int argc, char *argv[])
{
    printf("==== MEMORY LAYOUT IN C PROGRAMS ====\n\n");

    // Print addresses of variables in different memory segments
    print_memory_addresses();

    printf("\n");

    // Demonstrate stack memory
    stack_demo(2);

    // Demonstrate heap memory
    heap_demo();

    return 0;
}
