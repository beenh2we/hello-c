#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global variables (stored in DATA or BSS segments)
int initialized_global = 100;    // DATA segment (initialized)
int uninitialized_global;        // BSS segment (uninitialized)

// Static variables
static int static_initialized = 200;    // DATA segment
static int static_uninitialized;        // BSS segment

// Constants
const int const_global = 300;           // Read-only DATA segment

// String literals
const char *string_literal = "Hello";   // String in read-only DATA, pointer in DATA

// Function to demonstrate stack memory
void stack_demo(int param) {
    // Local variables (stored on the stack)
    int local_var = 500;
    static int static_local = 600;  // DATA segment (persists between calls)
    
    printf("Stack demo:\n");
    printf("  Parameter: %d at %p\n", param, (void*)&param);
    printf("  Local variable: %d at %p\n", local_var, (void*)&local_var);
    printf("  Static local: %d at %p\n", static_local, (void*)&static_local);
    
    // Demonstrate stack growth
    if (param > 0) {
        printf("\nRecursing with param-1...\n");
        stack_demo(param - 1);
        printf("Back from recursion\n\n");
    }
}

// Function to demonstrate heap memory
void heap_demo() {
    // Dynamic memory (stored on the heap)
    int *heap_int = (int*)malloc(sizeof(int));
    char *heap_str = (char*)malloc(20);
    
    if (!heap_int || !heap_str) {
        printf("Memory allocation failed\n");
        if (heap_int) free(heap_int);
        if (heap_str) free(heap_str);
        return;
    }
    
    *heap_int = 700;
    strcpy(heap_str, "Dynamic string");
    
    printf("Heap demo:\n");
    printf("  Heap integer: %d at %p\n", *heap_int, (void*)heap_int);
    printf("  Heap string: %s at %p\n", heap_str, (void*)heap_str);
    
    free(heap_int);
    free(heap_str);
}

// Function that's not called (to demonstrate code segment)
void unused_function() {
    printf("This function is in the code segment but never executed\n");
}

// Function to print memory addresses
void print_memory_addresses() {
    printf("Memory segment addresses:\n");
    printf("  Code segment:\n");
    printf("    main() function: %p\n", (void*)main);
    printf("    unused_function(): %p\n", (void*)unused_function);
    
    printf("  Data segment:\n");
    printf("    initialized_global: %p\n", (void*)&initialized_global);
    printf("    static_initialized: %p\n", (void*)&static_initialized);
    printf("    const_global: %p\n", (void*)&const_global);
    printf("    string_literal pointer: %p\n", (void*)&string_literal);
    printf("    string literal content: %p\n", (void*)string_literal);
    
    printf("  BSS segment:\n");
    printf("    uninitialized_global: %p\n", (void*)&uninitialized_global);
    printf("    static_uninitialized: %p\n", (void*)&static_uninitialized);
}

void explain_memory_layout() {
    printf("\nC Program Memory Layout Explanation:\n");
    printf("------------------------------------\n");
    
    printf("1. Text/Code Segment:\n");
    printf("   - Contains executable instructions\n");
    printf("   - Read-only and shared among processes\n");
    printf("   - Includes functions like main() and other code\n\n");
    
    printf("2. Data Segment:\n");
    printf("   - Initialized global and static variables\n");
    printf("   - Read-write area loaded from executable file\n\n");
    
    printf("3. BSS (Block Started by Symbol) Segment:\n");
    printf("   - Uninitialized global and static variables\n");
    printf("   - Zero-initialized by the operating system\n");
    printf("   - Requires less space in executable file\n\n");
    
    printf("4. Heap:\n");
    printf("   - Dynamic memory allocation (malloc, calloc, realloc)\n");
    printf("   - Grows upward in memory (higher addresses)\n");
    printf("   - Managed manually by the programmer\n\n");
    
    printf("5. Stack:\n");
    printf("   - Local variables, function parameters, return addresses\n");
    printf("   - Grows downward in memory (lower addresses)\n");
    printf("   - Automatically managed (LIFO - Last In, First Out)\n\n");
    
    printf("Memory Ordering (typical):\n");
    printf("  Low address:  Text/Code\n");
    printf("                Data\n");
    printf("                BSS\n");
    printf("                Heap (grows upward ↑)\n");
    printf("                ...\n");
    printf("                Stack (grows downward ↓)\n");
    printf("  High address: Command-line arguments and environment variables\n");
}

// Visual representation of memory layout
void visual_memory_layout() {
    printf("\nVisual Memory Layout Representation:\n");
    printf("----------------------------------\n");
    printf("High Address\n");
    printf("┌───────────────────────────┐\n");
    printf("│ Environment Variables     │\n");
    printf("│ Command Line Arguments    │\n");
    printf("├───────────────────────────┤\n");
    printf("│                           │\n");
    printf("│ Stack                     │\n");
    printf("│ ▼ (grows downward)        │\n");
    printf("│                           │\n");
    printf("├ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─┤\n");
    printf("│                           │\n");
    printf("│ Free Memory               │\n");
    printf("│                           │\n");
    printf("├ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─┤\n");
    printf("│                           │\n");
    printf("│ Heap                      │\n");
    printf("│ ▲ (grows upward)          │\n");
    printf("│                           │\n");
    printf("├───────────────────────────┤\n");
    printf("│ BSS Segment               │\n");
    printf("│ (uninitialized globals)   │\n");
    printf("├───────────────────────────┤\n");
    printf("│ Data Segment              │\n");
    printf("│ (initialized globals)     │\n");
    printf("├───────────────────────────┤\n");
    printf("│ Text/Code Segment         │\n");
    printf("│ (program instructions)    │\n");
    printf("└───────────────────────────┘\n");
    printf("Low Address\n");
}

int main() {
    printf("==== MEMORY LAYOUT IN C PROGRAMS ====\n\n");
    
    // Print addresses of variables in different memory segments
    print_memory_addresses();
    
    printf("\n");
    
    // Demonstrate stack memory
    stack_demo(2);
    
    // Demonstrate heap memory
    heap_demo();
    
    // Explain memory layout
    explain_memory_layout();
    
    // Visual representation
    visual_memory_layout();
    
    return 0;
}