#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure used for demonstration
typedef struct {
    char name[50];
    int age;
    float salary;
} Employee;

void basic_malloc_example() {
    printf("--- Basic malloc/free Example ---\n");
    
    // Allocate memory for a single integer
    int *p_int = (int*)malloc(sizeof(int));
    
    if (p_int == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    // Assign a value
    *p_int = 42;
    printf("Allocated memory for int: %d\n", *p_int);
    
    // Free the allocated memory
    free(p_int);
    p_int = NULL;  // Good practice to prevent dangling pointer
    
    // Allocate memory for an array of integers
    int size = 5;
    int *int_array = (int*)malloc(size * sizeof(int));
    
    if (int_array == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    // Initialize array values
    for (int i = 0; i < size; i++) {
        int_array[i] = i * 10;
    }
    
    // Display array values
    printf("\nAllocated array values: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", int_array[i]);
    }
    printf("\n");
    
    // Free the allocated memory
    free(int_array);
    int_array = NULL;
}

void calloc_vs_malloc() {
    printf("\n--- calloc vs malloc ---\n");
    
    int size = 5;
    
    // Allocate with malloc (memory not initialized)
    int *p_malloc = (int*)malloc(size * sizeof(int));
    
    // Allocate with calloc (memory initialized to zero)
    int *p_calloc = (int*)calloc(size, sizeof(int));
    
    if (p_malloc == NULL || p_calloc == NULL) {
        printf("Memory allocation failed\n");
        if (p_malloc) free(p_malloc);
        if (p_calloc) free(p_calloc);
        return;
    }
    
    // Display values (malloc memory will contain garbage values)
    printf("First 5 values from malloc memory: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", p_malloc[i]);
    }
    printf("\n");
    
    printf("First 5 values from calloc memory: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", p_calloc[i]);
    }
    printf("\n");
    
    free(p_malloc);
    free(p_calloc);
    p_malloc = NULL;
    p_calloc = NULL;
}

void realloc_example() {
    printf("\n--- realloc Example ---\n");
    
    // Initial allocation
    int initial_size = 3;
    int *array = (int*)malloc(initial_size * sizeof(int));
    
    if (array == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    // Initialize array
    for (int i = 0; i < initial_size; i++) {
        array[i] = (i + 1) * 100;
    }
    
    printf("Initial array values: ");
    for (int i = 0; i < initial_size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    // Resize array to a larger size
    int new_size = 5;
    int *new_array = (int*)realloc(array, new_size * sizeof(int));
    
    if (new_array == NULL) {
        printf("Memory reallocation failed\n");
        free(array);
        return;
    }
    
    // Original array pointer is no longer valid, use the new one
    array = new_array;
    
    // Initialize new elements
    for (int i = initial_size; i < new_size; i++) {
        array[i] = (i + 1) * 100;
    }
    
    printf("Array after realloc: ");
    for (int i = 0; i < new_size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    // Free the memory
    free(array);
    array = NULL;
}

void heap_vs_stack_example() {
    printf("\n--- Heap vs Stack Memory ---\n");
    
    // Stack allocation (automatic storage)
    int stack_array[5] = {1, 2, 3, 4, 5};
    
    // Heap allocation (dynamic storage)
    int *heap_array = (int*)malloc(5 * sizeof(int));
    if (heap_array == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    for (int i = 0; i < 5; i++) {
        heap_array[i] = i + 1;
    }
    
    printf("Stack array address: %p\n", (void*)stack_array);
    printf("Heap array address: %p\n", (void*)heap_array);
    
    // Memory comparison
    printf("\nComparison of memory allocation:\n");
    printf("Stack: Fixed size, fast allocation, automatically managed\n");
    printf("Heap: Dynamic size, slower allocation, manually managed\n");
    
    // Free heap memory
    free(heap_array);
    heap_array = NULL;
}

void struct_allocation_example() {
    printf("\n--- Struct Allocation Example ---\n");
    
    // Allocate memory for a single structure
    Employee *emp = (Employee*)malloc(sizeof(Employee));
    
    if (emp == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    // Initialize structure fields
    strcpy(emp->name, "John Doe");
    emp->age = 35;
    emp->salary = 75000.0f;
    
    printf("Employee details:\n");
    printf("Name: %s\n", emp->name);
    printf("Age: %d\n", emp->age);
    printf("Salary: $%.2f\n", emp->salary);
    
    // Free memory
    free(emp);
    emp = NULL;
    
    // Allocate an array of structures
    int num_employees = 3;
    Employee *team = (Employee*)malloc(num_employees * sizeof(Employee));
    
    if (team == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    // Initialize array of structures
    strcpy(team[0].name, "Alice Smith");
    team[0].age = 28;
    team[0].salary = 65000.0f;
    
    strcpy(team[1].name, "Bob Johnson");
    team[1].age = 42;
    team[1].salary = 85000.0f;
    
    strcpy(team[2].name, "Carol Williams");
    team[2].age = 33;
    team[2].salary = 72000.0f;
    
    printf("\nTeam details:\n");
    for (int i = 0; i < num_employees; i++) {
        printf("Employee %d:\n", i + 1);
        printf("  Name: %s\n", team[i].name);
        printf("  Age: %d\n", team[i].age);
        printf("  Salary: $%.2f\n", team[i].salary);
    }
    
    // Free memory
    free(team);
    team = NULL;
}

int main() {
    // Run the examples
    basic_malloc_example();
    calloc_vs_malloc();
    realloc_example();
    heap_vs_stack_example();
    struct_allocation_example();
    
    return 0;
}