#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("\n------------------- POINTER FUNDAMENTALS -------------------\n\n");
    
    /*
     * Memory Model Concept:
     * ┌───────────────────┐
     * │      Memory       │
     * ├─────────┬─────────┤
     * │ Address │ Value   │
     * ├─────────┼─────────┤
     * │  1000   │   25    │ ← int age = 25;
     * ├─────────┼─────────┤
     * │  1004   │  3.14   │ ← float pi = 3.14;
     * ├─────────┼─────────┤
     * │  1008   │  'A'    │ ← char grade = 'A';
     * └─────────┴─────────┘
     */
    
    /*** 1. Memory Addresses ***/
    printf("===== MEMORY ADDRESSES =====\n");
    int age = 80;
    char tag = 'Z';
    float height = 3.5;

    printf("age:    value = %d,  address = %p\n", age, &age);
    printf("tag:    value = %c,  address = %p\n", tag, &tag);
    printf("height: value = %.1f, address = %p\n\n", height, &height);
    
    /*** 2. Declaring and Using Pointers ***/
    printf("===== POINTER BASICS =====\n");
    int num = 1988;
    int *p_num = &num;  // p_num holds the address of num
    
    printf("num:          %d\n", num);
    printf("&num:         %p (address of num)\n", &num);
    printf("p_num:        %p (same as &num)\n", p_num);
    printf("*p_num:       %d (value at address p_num)\n", *p_num);
    printf("sizeof(num):  %zu bytes\n", sizeof(num));
    printf("sizeof(p_num): %zu bytes (pointer size)\n\n", sizeof(p_num));
    
    /*** 3. Modifying Values via Pointers ***/
    printf("===== MODIFYING VALUES THROUGH POINTERS =====\n");
    int x = 10;
    int *p = &x;

    printf("Original: x = %d, *p = %d\n", x, *p);
    
    *p = 20;  // Modify the value indirectly through the pointer
    printf("After *p = 20: x = %d, *p = %d\n\n", x, *p);
    
    /*
     * Memory visualization after modification:
     * ┌─────────┬─────────┐
     * │ Address │ Value   │
     * ├─────────┼─────────┤ 
     * │  &x     │   20    │ ← Modified through *p
     * └─────────┴─────────┘
     */
    
    /*** 4. Different Pointer Types ***/
    printf("===== POINTER TYPES =====\n");
    int number = 42;
    char letter = 'A';
    double price = 99.99;

    int *p_int = &number;     // Integer pointer
    char *p_char = &letter;   // Character pointer
    double *p_double = &price; // Double pointer
    
    printf("Type impacts pointer arithmetic and dereferencing:\n");
    printf("*p_int:    %d    (size: %zu bytes)\n", *p_int, sizeof(*p_int));
    printf("*p_char:   %c    (size: %zu bytes)\n", *p_char, sizeof(*p_char));
    printf("*p_double: %.2f (size: %zu bytes)\n\n", *p_double, sizeof(*p_double));
    
    // Void pointer - generic pointer type
    void *ptr_void = &number;  // Can point to any data type
    printf("void* can point to any data type but can't be dereferenced directly\n");
    // printf("%d\n", *ptr_void);  // ERROR: Can't dereference void*
    printf("Cast needed: %d\n\n", *(int*)ptr_void);  // Cast required
    
    /*** 5. NULL Pointers and Safety ***/
    printf("===== NULL POINTERS AND SAFETY =====\n");
    int *safe_ptr = NULL;  // Good practice: initialize pointers to NULL
    
    printf("NULL pointer value: %p\n", safe_ptr);
    
    // Safety check before dereferencing
    if (safe_ptr != NULL) {
        printf("Value: %d\n", *safe_ptr);
    } else {
        printf("Pointer is NULL, cannot dereference safely.\n");
    }

    return 0;
}