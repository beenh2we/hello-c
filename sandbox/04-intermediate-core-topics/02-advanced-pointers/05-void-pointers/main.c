#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Generic swap function that can swap any type of data
void swap_generic(void *a, void *b, size_t size) {
    // Allocate temporary buffer to hold the data
    void *temp = malloc(size);
    if (temp == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    // Copy a to temp
    memcpy(temp, a, size);
    // Copy b to a
    memcpy(a, b, size);
    // Copy temp to b
    memcpy(b, temp, size);
    
    // Free temp buffer
    free(temp);
}

// Function to print different data types
void print_value(void *ptr, char type) {
    switch(type) {
        case 'i': // int
            printf("%d", *(int*)ptr);
            break;
        case 'c': // char
            printf("%c", *(char*)ptr);
            break;
        case 'f': // float
            printf("%f", *(float*)ptr);
            break;
        case 'd': // double
            printf("%lf", *(double*)ptr);
            break;
        default:
            printf("Unknown type");
    }
}

// Generic comparison function for qsort
int compare_ints(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// Generic array printing function
void print_array(const void *array, size_t element_size, size_t count, char type) {
    const char *byte_ptr = (const char*)array;
    
    printf("[");
    for (size_t i = 0; i < count; i++) {
        if (i > 0) {
            printf(", ");
        }
        
        // Calculate pointer to current element
        const void *element_ptr = byte_ptr + (i * element_size);
        print_value((void*)element_ptr, type);
    }
    printf("]\n");
}

int main() {
    printf("=== Void Pointer Examples ===\n\n");
    
    // 1. Basic void pointer example
    printf("--- Basic Void Pointer Example ---\n");
    
    int i = 42;
    float f = 3.14f;
    char c = 'A';
    
    // Void pointer can point to any type
    void *ptr;
    
    ptr = &i;
    printf("Integer value: %d\n", *(int*)ptr);  // Cast to int* before dereferencing
    
    ptr = &f;
    printf("Float value: %f\n", *(float*)ptr);  // Cast to float* before dereferencing
    
    ptr = &c;
    printf("Char value: %c\n", *(char*)ptr);    // Cast to char* before dereferencing
    
    // 2. Generic swap function
    printf("\n--- Generic Swap Function ---\n");
    
    int a = 5, b = 10;
    printf("Before swap: a = %d, b = %d\n", a, b);
    swap_generic(&a, &b, sizeof(int));
    printf("After swap: a = %d, b = %d\n", a, b);
    
    float x = 2.5f, y = 7.8f;
    printf("Before swap: x = %f, y = %f\n", x, y);
    swap_generic(&x, &y, sizeof(float));
    printf("After swap: x = %f, y = %f\n", x, y);
    
    // 3. Using qsort with void pointers for comparison
    printf("\n--- qsort with Void Pointers ---\n");
    
    int numbers[] = {42, 13, 7, 87, 42, 16};
    int nums_size = sizeof(numbers) / sizeof(numbers[0]);
    
    printf("Before sorting: ");
    print_array(numbers, sizeof(int), nums_size, 'i');
    
    // qsort uses void pointers for generic sorting
    qsort(numbers, nums_size, sizeof(int), compare_ints);
    
    printf("After sorting: ");
    print_array(numbers, sizeof(int), nums_size, 'i');
    
    // 4. Generic function for printing arrays
    printf("\n--- Generic Array Printing ---\n");
    
    char char_array[] = {'H', 'e', 'l', 'l', 'o'};
    float float_array[] = {1.1f, 2.2f, 3.3f, 4.4f, 5.5f};
    
    printf("Characters: ");
    print_array(char_array, sizeof(char), 5, 'c');
    
    printf("Floats: ");
    print_array(float_array, sizeof(float), 5, 'f');
    
    // 5. Void pointer limitations
    printf("\n--- Void Pointer Limitations ---\n");
    
    void *generic_ptr = &a;
    
    // Cannot dereference directly
    // printf("%d\n", *generic_ptr);  // Error: dereferencing 'void *' pointer
    
    // Cannot perform pointer arithmetic directly
    // generic_ptr++;  // Error: arithmetic on void pointer
    
    // Must cast to a specific type first
    printf("Correct way to dereference: %d\n", *(int*)generic_ptr);
    
    // Correct way to do pointer arithmetic
    int *int_ptr = (int*)generic_ptr;
    int_ptr++;
    printf("After pointer arithmetic: %p\n", (void*)int_ptr);
    
    // 6. Dynamic memory allocation with void pointers
    printf("\n--- Dynamic Memory with Void Pointers ---\n");
    
    // malloc returns void* which can be assigned to any pointer type
    int *dynamic_array = (int*)malloc(5 * sizeof(int));
    
    if (dynamic_array == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    // Initialize array
    for (int i = 0; i < 5; i++) {
        dynamic_array[i] = i * 10;
    }
    
    printf("Dynamic array: ");
    print_array(dynamic_array, sizeof(int), 5, 'i');
    
    // Free memory
    free(dynamic_array);
    
    return 0;
}