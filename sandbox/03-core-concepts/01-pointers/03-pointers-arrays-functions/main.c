#include <stdio.h>

// Function declarations illustrating pointer usage patterns
void printArray(int *arr, int size);
void incrementByValue(int num);
void incrementByReference(int *num);
void swapIntegers(int *a, int *b);
void calculateBoth(int a, int b, int *sum, int *product);
void analyzeArray(int *arr, int size, int *min, int *max, double *avg);

int main() {
    printf("\n--------------- POINTERS WITH ARRAYS AND FUNCTIONS ---------------\n\n");
    
    /*** 1. ARRAY-POINTER RELATIONSHIP ***/
    printf("===== ARRAY-POINTER RELATIONSHIP =====\n");
    int numbers[5] = {10, 20, 30, 40, 50};
    int *ptr = numbers;  // Same as &numbers[0]
    
    // Demonstrating array-pointer equivalence
    printf("Array access via different notations:\n");
    printf("numbers[2] = %d, *(ptr+2) = %d\n", numbers[2], *(ptr+2));
    printf("*(numbers+3) = %d, ptr[3] = %d\n", *(numbers+3), ptr[3]);
    
    // Important differences between arrays and pointers
    printf("\nKey differences:\n");
    printf("sizeof(numbers) = %zu (entire array)\n", sizeof(numbers));
    printf("sizeof(ptr) = %zu (just the pointer)\n\n", sizeof(ptr));
    
    // Arrays are constant pointers, regular pointers are not
    // numbers++; // Error! Array name is constant pointer
    ptr++;      // Valid! Pointer now points to numbers[1]
    printf("After ptr++, *ptr = %d (now points to numbers[1])\n\n", *ptr);
    
    /*** 2. PASSING ARRAYS TO FUNCTIONS ***/
    printf("===== PASSING ARRAYS TO FUNCTIONS =====\n");
    ptr = numbers;  // Reset ptr to point to start of array
    printf("Array elements: ");
    printArray(numbers, 5);  // Array name decays to pointer when passed
    
    /*** 3. PASS BY VALUE VS REFERENCE ***/
    printf("\n===== PASS BY VALUE VS REFERENCE =====\n");
    int x = 10;
    
    incrementByValue(x);
    printf("After incrementByValue: x = %d (unchanged)\n", x);
    
    incrementByReference(&x);
    printf("After incrementByReference: x = %d (modified)\n\n", x);
    
    /*** 4. POINTER APPLICATIONS ***/
    printf("===== POINTER APPLICATIONS =====\n");
    
    // Example 1: Swapping values
    int a = 5, b = 10;
    printf("Before swap: a = %d, b = %d\n", a, b);
    swapIntegers(&a, &b);
    printf("After swap: a = %d, b = %d\n\n", a, b);
    
    // Example 2: Multiple return values
    int c = 5, d = 7;
    int sum, product;
    calculateBoth(c, d, &sum, &product);
    printf("Multiple outputs: %d + %d = %d, %d * %d = %d\n\n", c, d, sum, c, d, product);
    
    // Example 3: Array analysis
    int data[] = {5, 8, 2, 10, 7, 1, 9};
    int size = sizeof(data) / sizeof(data[0]);
    int min, max;
    double avg;
    
    analyzeArray(data, size, &min, &max, &avg);
    printf("Array analysis:\n");
    printf("- Minimum: %d\n", min);
    printf("- Maximum: %d\n", max);
    printf("- Average: %.2f\n\n", avg);
    
    /*** 5. CHARACTER ARRAYS AND POINTERS ***/
    printf("===== CHARACTER ARRAYS AND POINTERS =====\n");
    
    // Character array (string) example
    char name[] = "Hello World!";
    printf("String: \"%s\"\n", name);
    printf("Address of name: %p\n", name);
    printf("First character: '%c'\n", name[0]);
    printf("Size of string: %zu bytes (includes '\\0')\n\n", sizeof(name));
    
    // Character pointer operations
    char *p_str = name;
    printf("p_str points to: %p\n", p_str);
    printf("First char via pointer: '%c'\n", *p_str);
    printf("Second char via pointer: '%c'\n\n", *(p_str + 1));
    
    /*** 6. ARRAY OF POINTERS FOR STRINGS ***/
    printf("===== ARRAY OF POINTERS FOR STRINGS =====\n");
    
    char *names[] = {"Wht", "Tht", "Fek"};
    /*
     * Memory visualization:
     *          +---+---+---+
     * names -> | * | * | * |  Array of 3 pointers
     *          +-|-+-|-+-|-+
     *            |   |   |
     *            v   v   v
     *          "Wht" "Tht" "Fek"  String literals in memory
     */
    
    printf("Array of string pointers:\n");
    printf("names:      %p (address of pointer array)\n", names);
    
    // First string
    printf("\nAccessing first string:\n");
    printf("names[0]:   %s (string via array notation)\n", names[0]);
    printf("*(names):   %s (string via pointer dereferencing)\n", *(names));
    
    // Double dereferencing - accessing a character in a string
    printf("\nDouble dereferencing - character access:\n");
    printf("Second char in \"Fek\": %c\n", *(*(names + 2) + 1));
    printf("Explanation: *(names+2) gets \"Fek\", then +1 and * accesses 'e'\n");
    
    return 0;
}

// Function implementations
void printArray(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void incrementByValue(int num) {
    num++;  // Only modifies local copy
}

void incrementByReference(int *num) {
    (*num)++;  // Modifies value at the address
}

void swapIntegers(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void calculateBoth(int a, int b, int *sum, int *product) {
    *sum = a + b;
    *product = a * b;
}

void analyzeArray(int *arr, int size, int *min, int *max, double *avg) {
    *min = *max = arr[0];
    int sum = arr[0];
    
    for (int i = 1; i < size; i++) {
        if (arr[i] < *min) *min = arr[i];
        if (arr[i] > *max) *max = arr[i];
        sum += arr[i];
    }
    
    *avg = (double)sum / size;
}