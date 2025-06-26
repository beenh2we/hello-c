#include <stdio.h>

// Global variable with internal linkage (only visible in this file)
static int internal_global = 100;

// Regular global variable (external linkage)
int regular_global = 200;

// Function with internal linkage (only visible in this file)
static void internal_function()
{
    printf("This function has internal linkage\n");
}

// Function prototype with register hint
void function_with_register(register int x);

// Demonstrate various storage classes
void demonstrate_storage_classes()
{
    // auto variable (default)
    // 'auto' keyword is rarely used as it's the default
    auto int auto_var = 10;
    printf("Auto variable: %d\n", auto_var);

    // register variable (hint to compiler)
    register int reg_var = 20;
    printf("Register variable: %d\n", reg_var);

    // static variable (persistent between function calls)
    static int static_var = 30;
    static_var++;
    printf("Static variable: %d\n", static_var);

    // Example showing address of auto variable
    int *ptr = &auto_var;
    printf("Address of auto_var: %p\n", (void *) ptr);

    // Cannot take the address of a register variable
    // ptr = &reg_var;  // This would cause a compilation error
}

// Function with register parameter
void function_with_register(register int x)
{
    printf("Register parameter value: %d\n", x);
    // Cannot take the address of register parameter
    // int *ptr = &x;  // This would cause a compilation error
}

int main()
{
    printf("-- Storage Classes Demonstration --\n\n");

    // Demonstrate storage classes
    printf("Storage Classes:\n");
    demonstrate_storage_classes();
    printf("\n");

    // Call again to show that static variable persists
    printf("Calling demonstrate_storage_classes again:\n");
    demonstrate_storage_classes();
    printf("\n");

    // Demonstrate register parameter
    printf("Register parameter:\n");
    function_with_register(50);
    printf("\n");

    // Access global variables
    printf("Global variables:\n");
    printf("Internal global variable: %d\n", internal_global);
    printf("Regular global variable: %d\n", regular_global);

    // Call internal function
    internal_function();

    return 0;
}
