#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void malloc_example() {
    printf("--- malloc Example ---\n");
    
    // Basic malloc - allocates uninitialized memory
    int *numbers = (int*)malloc(5 * sizeof(int));
    
    if (numbers == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    // Memory is uninitialized - contains garbage values
    printf("After malloc (uninitialized values):\n");
    for (int i = 0; i < 5; i++) {
        printf("numbers[%d] = %d\n", i, numbers[i]);
    }
    
    // Initialize the memory
    for (int i = 0; i < 5; i++) {
        numbers[i] = i * 10;
    }
    
    printf("\nAfter initialization:\n");
    for (int i = 0; i < 5; i++) {
        printf("numbers[%d] = %d\n", i, numbers[i]);
    }
    
    // Free the memory
    free(numbers);
    numbers = NULL;  // Good practice to avoid dangling pointers
}

void calloc_example() {
    printf("\n--- calloc Example ---\n");
    
    // calloc - allocates and initializes memory to zero
    int *numbers = (int*)calloc(5, sizeof(int));
    
    if (numbers == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    // Memory is initialized to zero
    printf("After calloc (zero-initialized):\n");
    for (int i = 0; i < 5; i++) {
        printf("numbers[%d] = %d\n", i, numbers[i]);
    }
    
    // Initialize with new values
    for (int i = 0; i < 5; i++) {
        numbers[i] = (i + 1) * 100;
    }
    
    printf("\nAfter initialization:\n");
    for (int i = 0; i < 5; i++) {
        printf("numbers[%d] = %d\n", i, numbers[i]);
    }
    
    // Free the memory
    free(numbers);
    numbers = NULL;
}

void realloc_examples() {
    printf("\n--- realloc Examples ---\n");
    
    // Example 1: Resize array to larger size
    printf("Example 1: Expanding an array\n");
    
    // Initial allocation
    int *numbers = (int*)malloc(5 * sizeof(int));
    
    if (numbers == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    // Initialize with values
    for (int i = 0; i < 5; i++) {
        numbers[i] = i + 1;
    }
    
    printf("Initial array (size 5):\n");
    for (int i = 0; i < 5; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");
    
    // Resize to larger size
    int *new_numbers = (int*)realloc(numbers, 10 * sizeof(int));
    
    if (new_numbers == NULL) {
        printf("Memory reallocation failed\n");
        free(numbers);
        return;
    }
    
    // The original data is preserved
    numbers = new_numbers;  // Update pointer
    
    // Initialize new elements
    for (int i = 5; i < 10; i++) {
        numbers[i] = i + 1;
    }
    
    printf("After expansion (size 10):\n");
    for (int i = 0; i < 10; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n\n");
    
    // Example 2: Resize array to smaller size
    printf("Example 2: Shrinking an array\n");
    
    // Resize to smaller size
    new_numbers = (int*)realloc(numbers, 3 * sizeof(int));
    
    if (new_numbers == NULL) {
        printf("Memory reallocation failed\n");
        free(numbers);
        return;
    }
    
    numbers = new_numbers;  // Update pointer
    
    printf("After shrinking (size 3):\n");
    for (int i = 0; i < 3; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n\n");
    
    // Example 3: Using realloc to free memory
    printf("Example 3: Using realloc with size 0 (equivalent to free)\n");
    new_numbers = (int*)realloc(numbers, 0);  // Same as free(numbers)
    if (new_numbers == NULL) {
        printf("Memory freed successfully\n");
    }
    numbers = NULL;  // Avoid dangling pointer
    
    // Example 4: Using realloc with NULL pointer (equivalent to malloc)
    printf("Example 4: Using realloc with NULL pointer (equivalent to malloc)\n");
    int *data = (int*)realloc(NULL, 4 * sizeof(int));  // Same as malloc
    
    if (data == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    for (int i = 0; i < 4; i++) {
        data[i] = (i + 1) * 5;
    }
    
    printf("Data allocated with realloc(NULL, ...):\n");
    for (int i = 0; i < 4; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");
    
    free(data);
    data = NULL;
}

// Example of using calloc for a struct array
typedef struct {
    int id;
    char name[50];
    double score;
} Student;

void calloc_struct_example() {
    printf("\n--- calloc with Structs Example ---\n");
    
    int num_students = 3;
    
    // Allocate array of structures
    Student *students = (Student*)calloc(num_students, sizeof(Student));
    
    if (students == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    // All fields are zero-initialized
    printf("After calloc (zero-initialization):\n");
    for (int i = 0; i < num_students; i++) {
        printf("Student %d: id=%d, name='%s', score=%.1f\n", 
               i, students[i].id, students[i].name, students[i].score);
    }
    
    // Initialize with data
    students[0].id = 1001;
    strcpy(students[0].name, "Alice");
    students[0].score = 92.5;
    
    students[1].id = 1002;
    strcpy(students[1].name, "Bob");
    students[1].score = 85.0;
    
    students[2].id = 1003;
    strcpy(students[2].name, "Charlie");
    students[2].score = 77.5;
    
    printf("\nAfter initialization:\n");
    for (int i = 0; i < num_students; i++) {
        printf("Student %d: id=%d, name='%s', score=%.1f\n", 
               i, students[i].id, students[i].name, students[i].score);
    }
    
    free(students);
    students = NULL;
}

void benchmark_malloc_vs_calloc() {
    printf("\n--- Benchmark: malloc vs. calloc ---\n");
    
    const size_t sizes[] = {1024, 10240, 102400, 1024000};
    const int iterations = 1000;
    
    for (int s = 0; s < sizeof(sizes)/sizeof(sizes[0]); s++) {
        size_t size = sizes[s];
        
        // Benchmark malloc + memset
        clock_t start = clock();
        for (int i = 0; i < iterations; i++) {
            char *data = (char*)malloc(size);
            if (data) {
                memset(data, 0, size);
                free(data);
            }
        }
        clock_t end = clock();
        double malloc_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        
        // Benchmark calloc
        start = clock();
        for (int i = 0; i < iterations; i++) {
            char *data = (char*)calloc(size, 1);
            if (data) {
                free(data);
            }
        }
        end = clock();
        double calloc_time = ((double)(end - start)) / CLOCKS_PER_SEC;
        
        printf("Size: %zu bytes - malloc+memset: %.4f sec, calloc: %.4f sec\n", 
               size, malloc_time, calloc_time);
    }
}

int main() {
    printf("==== DYNAMIC MEMORY ALLOCATION EXAMPLES ====\n\n");
    
    malloc_example();
    calloc_example();
    realloc_examples();
    calloc_struct_example();
    benchmark_malloc_vs_calloc();
    
    return 0;
}