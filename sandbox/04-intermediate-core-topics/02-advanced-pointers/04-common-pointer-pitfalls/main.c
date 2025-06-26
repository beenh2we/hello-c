#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Note: This file intentionally contains examples of BAD PRACTICES
// to demonstrate common pointer errors. DO NOT use these patterns
// in real code!

void null_pointer_dereference()
{
    printf("--- Null Pointer Dereference Example ---\n");

    int *ptr = NULL;

    printf("About to dereference a NULL pointer...\n");

    // DANGER: This will cause a segmentation fault!
    // Uncomment to see the error:
    // *ptr = 42;  // Crash!

    printf("Instead, always check pointers before using them:\n");
    if (ptr != NULL)
    {
        *ptr = 42;  // Safe now, but will be skipped
    }
    else
    {
        printf("Avoided dereferencing NULL pointer!\n");
    }
}

void dangling_pointer_example()
{
    printf("\n--- Dangling Pointer Example ---\n");

    int *ptr = (int *) malloc(sizeof(int));

    if (ptr == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }

    *ptr = 42;
    printf("Value before free: %d\n", *ptr);

    free(ptr);  // Memory is released back to the heap

    // DANGER: ptr is now a dangling pointer!
    printf("ptr is now dangling (points to freed memory)\n");

    // DANGER: Using the pointer after free is undefined behavior
    // Uncomment to see potential problems:
    // printf("Value after free: %d\n", *ptr);  // BAD! Unpredictable result

    // BEST PRACTICE: Set pointer to NULL after freeing
    ptr = NULL;

    if (ptr != NULL)
    {
        printf("This won't print because ptr is NULL\n");
    }
    else
    {
        printf("Good practice: ptr is safely set to NULL after free\n");
    }
}

void memory_leak_example()
{
    printf("\n--- Memory Leak Example ---\n");

    // DANGER: This function allocates memory but never frees it

    // Memory allocation that's never freed (in a loop to show the concept)
    printf("Allocating memory multiple times without freeing:\n");

    for (int i = 0; i < 3; i++)
    {
        int *ptr =
            (int *) malloc(1000000 * sizeof(int));  // Allocate a large chunk

        if (ptr == NULL)
        {
            printf("Memory allocation failed\n");
            return;
        }

        printf(
            "Allocated %zu bytes at %p\n", 1000000 * sizeof(int), (void *) ptr);

        // DANGER: No corresponding free(ptr) here!
        // This memory will be inaccessible but still allocated
        // until the program terminates

        // CORRECT way would be:
        // free(ptr);
    }

    printf("Memory leaked! In a real program, this would waste resources\n");
    printf(
        "In practice, the OS will reclaim all memory when the program exits\n");
}

void buffer_overflow_example()
{
    printf("\n--- Buffer Overflow Example ---\n");

    int buffer[5] = {1, 2, 3, 4, 5};
    int *ptr = buffer;

    printf("Buffer contains 5 integers: ");
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", buffer[i]);
    }
    printf("\n");

    // DANGER: Writing beyond array bounds
    printf("About to write beyond array bounds...\n");

    // DANGER: This causes a buffer overflow!
    // In this simple example, it might "work" but is undefined behavior
    // Uncomment to see potential issues:
    /*
    for (int i = 0; i < 10; i++) {  // Going beyond array size of 5
        ptr[i] = i * 10;
        printf("Written %d to position %d\n", ptr[i], i);
    }
    */

    printf("Instead, always check bounds before accessing arrays:\n");
    int size = 5;  // Known size of buffer
    for (int i = 0; i < 10; i++)
    {
        if (i < size)
        {
            ptr[i] = i * 10;
            printf("Safely written %d to position %d\n", ptr[i], i);
        }
        else
        {
            printf("Prevented write to invalid position %d\n", i);
        }
    }
}

void double_free_example()
{
    printf("\n--- Double Free Example ---\n");

    int *ptr = (int *) malloc(sizeof(int));

    if (ptr == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }

    *ptr = 42;
    printf("Allocated memory with value: %d\n", *ptr);

    // First free is correct
    printf("Freeing memory first time (correct)\n");
    free(ptr);

    // DANGER: Double free causes undefined behavior!
    printf("About to attempt double free (very bad practice)...\n");

    // Uncomment to see potential issues:
    // free(ptr);  // WRONG! Memory already freed

    // BEST PRACTICE: Set to NULL after free and check before freeing
    ptr = NULL;

    if (ptr != NULL)
    {
        free(ptr);  // Won't happen because ptr is NULL
    }
    else
    {
        printf("Prevented double free by checking for NULL\n");
    }
}

void use_after_free_example()
{
    printf("\n--- Use After Free Example ---\n");

    char *str = (char *) malloc(20 * sizeof(char));

    if (str == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }

    strcpy(str, "Hello World");
    printf("String before free: %s\n", str);

    free(str);

    // DANGER: Using memory after it's been freed is undefined behavior
    printf("After free, the pointer still contains an address: %p\n",
           (void *) str);
    printf("But the memory it points to is no longer valid!\n");

    // DANGER: Don't do this!
    // Uncomment to see potential issues:
    // printf("String after free: %s\n", str);  // BAD! Unpredictable result

    // BEST PRACTICE: Set to NULL after freeing
    str = NULL;

    if (str != NULL)
    {
        printf("This won't print because str is NULL\n");
    }
    else
    {
        printf("Good practice: str is safely set to NULL after free\n");
    }
}

void uninitialized_pointer_example()
{
    printf("\n--- Uninitialized Pointer Example ---\n");

    int *ptr;  // Uninitialized pointer (contains garbage value)

    printf("Uninitialized pointer contains a random address\n");

    // DANGER: Dereferencing uninitialized pointer leads to undefined behavior
    // Uncomment to see potential issues:
    // printf("Value at uninitialized pointer: %d\n", *ptr);  // BAD! Unpredictable result

    // BEST PRACTICE: Initialize pointers to NULL or a valid address
    ptr = NULL;
    printf("Initialized ptr to NULL for safety\n");

    // Safe check before use
    if (ptr != NULL)
    {
        printf("Value: %d\n", *ptr);
    }
    else
    {
        printf("Pointer is NULL, cannot dereference\n");
    }
}

int main()
{
    printf("==== COMMON POINTER PITFALLS ====\n");
    printf("This program demonstrates common pointer errors.\n");
    printf(
        "Some dangerous operations are commented out to prevent crashes.\n\n");

    null_pointer_dereference();
    dangling_pointer_example();
    memory_leak_example();
    buffer_overflow_example();
    double_free_example();
    use_after_free_example();
    uninitialized_pointer_example();

    printf("\n==== END OF EXAMPLES ====\n");
    printf("Remember: The above examples show what NOT to do!\n");
    printf("Always check pointers before use, free allocated memory,\n");
    printf("and set pointers to NULL after freeing them.\n");

    return 0;
}
