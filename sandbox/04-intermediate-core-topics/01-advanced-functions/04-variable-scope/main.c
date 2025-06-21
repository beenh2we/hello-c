#include <stdio.h>

// Global variable - file scope, static storage duration
int global_var = 10;

// Function to demonstrate local variables
void demonstrate_local_variables() {
    // Local variable - block scope, automatic storage duration
    int local_var = 20;
    printf("Inside function - local_var: %d\n", local_var);
    
    // Block scope demonstration
    {
        int block_var = 30;
        printf("Inside block - block_var: %d\n", block_var);
        printf("Inside block - local_var: %d\n", local_var);
        
        // Variable shadowing
        int local_var = 25;  // This shadows the outer local_var
        printf("Inside block - shadowed local_var: %d\n", local_var);
    }
    
    // block_var is no longer accessible here
    // printf("Outside block - block_var: %d\n", block_var); // Compilation error
    
    printf("Outside block - local_var: %d\n", local_var); // Original local_var
}

// Function to demonstrate parameter scope
void demonstrate_parameters(int param_var) {
    printf("Parameter value: %d\n", param_var);
    param_var = 50;  // This only modifies the local copy
    printf("Modified parameter value: %d\n", param_var);
}

// Function with a static local variable
void demonstrate_static_local() {
    // Static local variable - block scope, static storage duration
    // Initialized only once and retains its value between function calls
    static int static_var = 0;
    static_var++;
    printf("Static variable value: %d\n", static_var);
}

int main() {
    printf("Global variable value: %d\n\n", global_var);
    
    // Local variable in main
    int main_var = 100;
    printf("Main variable value: %d\n\n", main_var);
    
    // Demonstrate local variables
    printf("-- Local variables demonstration --\n");
    demonstrate_local_variables();
    printf("\n");
    
    // Demonstrate parameters
    printf("-- Parameters demonstration --\n");
    int arg = 40;
    printf("Before function call: %d\n", arg);
    demonstrate_parameters(arg);
    printf("After function call: %d\n", arg);  // Value remains unchanged
    printf("\n");
    
    // Demonstrate static local variables
    printf("-- Static local variable demonstration --\n");
    for (int i = 0; i < 3; i++) {
        demonstrate_static_local();
    }
    printf("\n");
    
    // Demonstrate variable lifetime
    printf("-- Variable lifetime demonstration --\n");
    for (int i = 0; i < 3; i++) {
        int auto_var = 0;  // Automatic variable recreated each iteration
        auto_var++;
        printf("Automatic variable: %d\n", auto_var);  // Always prints 1
    }
    
    return 0;
}