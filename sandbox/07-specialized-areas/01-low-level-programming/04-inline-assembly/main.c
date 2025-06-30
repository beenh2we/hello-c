#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Note: This code uses GCC inline assembly syntax.
// Different compilers use different syntax for inline assembly.

// Get the CPU timestamp counter using inline assembly
uint64_t get_cpu_ticks()
{
    uint32_t low, high;

    // rdtsc instruction reads the CPU timestamp counter
    // into EDX:EAX (high:low)
    __asm__ volatile("rdtsc" : "=a"(low), "=d"(high));

    return ((uint64_t) high << 32) | low;
}

// Calculate square root using x86 assembly instruction
float fast_sqrt(float number)
{
    float result;

    __asm__ volatile("sqrtss %1, %0" : "=x"(result) : "x"(number));

    return result;
}

// Simple assembly function with no inputs/outputs
void basic_assembly_demo()
{
    printf("=== Basic Inline Assembly ===\n");

    // Push value to stack, pop it back, no actual effect
    __asm__(
        "pushl $42 \n\t"
        "popl %eax \n\t");

    printf("Basic assembly block executed.\n");
}

// Addition using inline assembly
int add_with_assembly(int a, int b)
{
    int result;

    // Basic inline assembly syntax:
    // __asm__(assembly_code : output_operands : input_operands : clobbered_registers);

    __asm__(
        "movl %1, %0 \n\t"  // Move a to result
        "addl %2, %0"       // Add b to result
        : "=r"(result)      // Output operands
        : "r"(a), "r"(b)    // Input operands
    );

    return result;
}

// Assembly with C expressions
int multiply_and_add(int a, int b, int c)
{
    int result;

    __asm__(
        "imull %2, %1 \n\t"       // a * b
        "addl %3, %1 \n\t"        // (a * b) + c
        "movl %1, %0"             // Store in result
        : "=r"(result)            // Output
        : "r"(a), "r"(b), "r"(c)  // Inputs
        : "cc"                    // Clobbered (condition codes)
    );

    return result;
}

// Demonstrate accessing CPU flags
int check_carry_flag(int a, int b)
{
    int carry;

    __asm__(
        "addl %2, %1 \n\t"  // Add b to a
        "setc %0"           // Set carry to 1 if carry flag is set, 0 otherwise
        : "=r"(carry)       // Output
        : "r"(a), "r"(b)    // Inputs
        : "cc"              // Clobbered
    );

    return carry;
}

// Simple memory barrier
void memory_barrier()
{
    __asm__ volatile("" : : : "memory");
    // This serves as a compiler barrier, preventing reordering of
    // memory accesses across this point
}

// Full memory barrier
void full_memory_barrier()
{
    __asm__ volatile("mfence" : : : "memory");
    // Ensures all memory operations before this instruction
    // complete before any memory operations after it
}

// Measure timing difference between C and assembly implementation of a function
void benchmark_sqrt()
{
    printf("\n=== Benchmark: Standard sqrt vs Assembly sqrtss ===\n");

    float test_value = 12345.6789f;
    int iterations = 10000000;

    // Warm up the cache
    float dummy = fast_sqrt(test_value);
    dummy = sqrtf(test_value);

    // Test assembly version
    uint64_t start_asm = get_cpu_ticks();
    float asm_result = 0;
    for (int i = 0; i < iterations; i++)
    {
        asm_result = fast_sqrt(test_value + i * 0.01f);
    }
    uint64_t end_asm = get_cpu_ticks();

    // Test C library version
    uint64_t start_c = get_cpu_ticks();
    float c_result = 0;
    for (int i = 0; i < iterations; i++)
    {
        c_result = sqrtf(test_value + i * 0.01f);
    }
    uint64_t end_c = get_cpu_ticks();

    printf("Assembly sqrtss result: %f in %llu cycles\n",
           asm_result,
           (end_asm - start_asm));
    printf("C library sqrtf result: %f in %llu cycles\n",
           c_result,
           (end_c - start_c));
    printf(
        "Assembly version was %.2f%% %s\n",
        100.0
            * abs((long long) end_asm - start_asm - (long long) end_c + start_c)
            / (double) (end_c - start_c),
        (end_asm - start_asm < end_c - start_c) ? "faster" : "slower");
}

// Demonstrate various inline assembly use cases
void demo_various_assembly()
{
    printf("\n=== Various Assembly Examples ===\n");

    // Get CPU ID information
    uint32_t eax, ebx, ecx, edx;
    eax = 1;  // Set CPUID function

    __asm__ volatile("cpuid"
                     : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                     : "a"(eax)
                     : "cc");

    printf("CPU ID: EAX=%08X EBX=%08X ECX=%08X EDX=%08X\n", eax, ebx, ecx, edx);

    // Example: Byte swap using BSWAP instruction
    uint32_t value = 0x12345678;
    uint32_t swapped;

    __asm__ volatile(
        "bswap %1 \n\t"
        "movl %1, %0"
        : "=r"(swapped)
        : "r"(value));

    printf("Original: 0x%08X, Byte-swapped: 0x%08X\n", value, swapped);

    // Example: Count leading zeros using BSR instruction
    uint32_t x = 0x00000080;  // Bit 7 is set
    uint32_t position;

    __asm__ volatile("bsrl %1, %0 \n\t" : "=r"(position) : "r"(x));

    printf("Most significant bit position in 0x%08X: %u\n", x, position);
}

int main()
{
    printf("==== INLINE ASSEMBLY DEMONSTRATION ====\n\n");

    // Basic inline assembly
    basic_assembly_demo();

    // Simple addition
    int a = 10, b = 20;
    int sum = add_with_assembly(a, b);
    printf("\nAssembly addition: %d + %d = %d\n", a, b, sum);

    // More complex operation
    int c = 5;
    int result = multiply_and_add(a, b, c);
    printf("Assembly multiply and add: %d * %d + %d = %d\n", a, b, c, result);

    // Check for carry flag
    int x = 0xFFFFFFFF, y = 1;  // Will overflow 32-bit integer
    int carry = check_carry_flag(x, y);
    printf("Adding %u + %u: carry flag = %d\n", x, y, carry);

    // Demonstrate CPU timestamp counter
    uint64_t start_ticks = get_cpu_ticks();
    // Do some work
    for (int i = 0; i < 1000000; i++)
    {
        // Simple computation to prevent optimization
        result += i % 10;
    }
    uint64_t end_ticks = get_cpu_ticks();

    printf("\nCPU ticks elapsed: %llu\n", (end_ticks - start_ticks));

    // Square root using assembly
    float num = 2.0f;
    float sqrt_result = fast_sqrt(num);
    printf("\nSquare root of %.1f using assembly: %.6f\n", num, sqrt_result);

    // Run benchmark
    benchmark_sqrt();

    // Show various assembly examples
    demo_various_assembly();

    return 0;
}
