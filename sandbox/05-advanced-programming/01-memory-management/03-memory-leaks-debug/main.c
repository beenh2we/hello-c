#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to track allocations
typedef struct
{
    void *ptr;
    size_t size;
    const char *file;
    int line;
    const char *function;
} AllocationRecord;

#define MAX_ALLOCATIONS 1000
AllocationRecord allocations[MAX_ALLOCATIONS];
int allocation_count = 0;

// Custom allocation functions for tracking
void *track_malloc(size_t size,
                   const char *file,
                   int line,
                   const char *function)
{
    void *ptr = malloc(size);

    if (ptr != NULL && allocation_count < MAX_ALLOCATIONS)
    {
        allocations[allocation_count].ptr = ptr;
        allocations[allocation_count].size = size;
        allocations[allocation_count].file = file;
        allocations[allocation_count].line = line;
        allocations[allocation_count].function = function;
        allocation_count++;
    }

    return ptr;
}

void track_free(void *ptr, const char *file, int line, const char *function)
{
    if (ptr == NULL) return;

    // Find the allocation record
    int found = 0;
    for (int i = 0; i < allocation_count; i++)
    {
        if (allocations[i].ptr == ptr)
        {
            // Remove by swapping with the last element
            allocations[i] = allocations[allocation_count - 1];
            allocation_count--;
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("WARNING: Attempt to free untracked pointer %p at %s:%d (%s)\n",
               ptr,
               file,
               line,
               function);
    }

    free(ptr);
}

// Define macros to replace standard functions
#define DEBUG_MALLOC(size) track_malloc(size, __FILE__, __LINE__, __func__)
#define DEBUG_FREE(ptr)    track_free(ptr, __FILE__, __LINE__, __func__)

// Print memory leak report
void print_leak_report(void)
{
    printf("\n=== Memory Leak Report ===\n");

    if (allocation_count == 0)
    {
        printf("No memory leaks detected!\n");
    }
    else
    {
        printf("Detected %d memory leaks:\n", allocation_count);

        size_t total_bytes = 0;
        for (int i = 0; i < allocation_count; i++)
        {
            printf("%d) %p: %zu bytes allocated at %s:%d (%s)\n",
                   i + 1,
                   allocations[i].ptr,
                   allocations[i].size,
                   allocations[i].file,
                   allocations[i].line,
                   allocations[i].function);
            total_bytes += allocations[i].size;
        }

        printf("\nTotal leaked memory: %zu bytes\n", total_bytes);
    }
}

// Common types of memory issues
void demonstrate_memory_leak(void)
{
    printf("--- Memory Leak Example ---\n");

    // Allocate memory and "forget" to free it
    int *numbers = (int *) DEBUG_MALLOC(5 * sizeof(int));

    if (numbers == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }

    for (int i = 0; i < 5; i++)
    {
        numbers[i] = i * 10;
    }

    printf("Allocated memory but didn't free it - memory leak!\n");
    // Missing: DEBUG_FREE(numbers);
}

void demonstrate_double_free(void)
{
    printf("\n--- Double Free Example ---\n");

    char *text = (char *) DEBUG_MALLOC(50);

    if (text == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }

    strcpy(text, "This is a test string");
    printf("Allocated string: %s\n", text);

    // Free it once (correct)
    DEBUG_FREE(text);
    printf("Memory freed correctly\n");

    // Uncommenting this would cause a double-free error:
    // DEBUG_FREE(text);  // Error: memory already freed

    printf("Avoided double free error\n");
}

void demonstrate_use_after_free(void)
{
    printf("\n--- Use After Free Example ---\n");

    int *data = (int *) DEBUG_MALLOC(sizeof(int));

    if (data == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }

    *data = 42;
    printf("Original value: %d\n", *data);

    DEBUG_FREE(data);

    // DANGER: Using memory after it's freed can cause unpredictable behavior
    // Uncommenting this would demonstrate use-after-free:
    // printf("Value after free: %d\n", *data);  // Undefined behavior

    printf("Avoided use after free error\n");
    data = NULL;  // Set to NULL after free (good practice)
}

void demonstrate_memory_corruption(void)
{
    printf("\n--- Memory Corruption Example ---\n");

    char *buffer = (char *) DEBUG_MALLOC(10);

    if (buffer == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }

    printf("Allocated 10 bytes for buffer\n");

    // DANGER: Writing beyond allocated memory
    // Uncommenting this would cause memory corruption:
    // strcpy(buffer, "This string is too long for the buffer");

    // Safe alternative:
    strncpy(buffer, "Short", 9);
    buffer[9] = '\0';  // Ensure null-termination

    printf("Safely wrote: %s\n", buffer);
    DEBUG_FREE(buffer);
}

void demonstrate_null_pointer_dereference(void)
{
    printf("\n--- Null Pointer Dereference Example ---\n");

    int *ptr = NULL;

    printf("ptr is NULL\n");

    // DANGER: Dereferencing NULL causes a segmentation fault
    // Uncommenting this would crash the program:
    // *ptr = 100;

    // Safe approach:
    if (ptr != NULL)
    {
        *ptr = 100;
    }
    else
    {
        printf("Avoided null pointer dereference\n");
    }
}

int main(void)
{
    printf("==== MEMORY LEAKS AND DEBUGGING ====\n\n");

    // Demonstrate various memory issues
    demonstrate_memory_leak();
    demonstrate_double_free();
    demonstrate_use_after_free();
    demonstrate_memory_corruption();
    demonstrate_null_pointer_dereference();

    // Print memory leak report
    print_leak_report();

    return 0;
}
