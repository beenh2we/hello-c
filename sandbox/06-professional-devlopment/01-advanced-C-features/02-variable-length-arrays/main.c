#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function using VLAs
void process_data_with_vla(int size)
{
    printf("\n=== Using Variable-Length Array ===\n");

    // VLA declaration - size determined at runtime
    int data[size];

    // Initialize the array
    printf("Initializing array of size %d\n", size);
    for (int i = 0; i < size; i++)
    {
        data[i] = i * 10;
    }

    // Use the array
    printf("Array contents: ");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", data[i]);
    }
    printf("\n");

    // Stack-based array will be automatically deallocated when function returns
    printf("VLA will be automatically deallocated\n");
}

// Function using traditional dynamic allocation
void process_data_with_malloc(int size)
{
    printf("\n=== Using malloc for Dynamic Array ===\n");

    // Dynamic allocation on heap
    int *data = (int *) malloc(size * sizeof(int));
    if (!data)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    // Initialize the array
    printf("Initializing array of size %d\n", size);
    for (int i = 0; i < size; i++)
    {
        data[i] = i * 10;
    }

    // Use the array
    printf("Array contents: ");
    for (int i = 0; i < size; i++)
    {
        printf("%d ", data[i]);
    }
    printf("\n");

    // Explicit deallocation required
    printf("Explicitly freeing the allocated memory\n");
    free(data);
}

// Multi-dimensional VLAs
void demonstrate_multi_dim_vla(int rows, int cols)
{
    printf("\n=== Multi-dimensional VLA ===\n");

    // 2D VLA
    int matrix[rows][cols];

    // Initialize with values
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            matrix[i][j] = i * 100 + j;
        }
    }

    // Print the matrix
    printf("Matrix [%d][%d]:\n", rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Function that takes a VLA as parameter
void process_matrix(int rows, int cols, int matrix[rows][cols])
{
    printf("\n=== Processing VLA Parameter ===\n");

    // Calculate sum of elements
    int sum = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            sum += matrix[i][j];
        }
    }

    printf("Sum of all elements in the matrix: %d\n", sum);

    // Modify some elements
    printf("Doubling elements in first row...\n");
    for (int j = 0; j < cols; j++)
    {
        matrix[0][j] *= 2;
    }
}

// Function to benchmark VLA vs malloc
void benchmark_vla_vs_malloc()
{
    printf("\n=== Benchmark: VLA vs malloc ===\n");

    const int ITERATIONS = 1000000;
    const int ARRAY_SIZE = 100;

    // Benchmark VLA
    clock_t start_vla = clock();

    for (int i = 0; i < ITERATIONS; i++)
    {
        // Create and use a VLA
        int vla[ARRAY_SIZE];
        vla[0] = i;                        // Do a simple operation
        vla[ARRAY_SIZE - 1] = i + vla[0];  // Another operation
    }

    clock_t end_vla = clock();
    double time_vla = ((double) (end_vla - start_vla)) / CLOCKS_PER_SEC;

    // Benchmark malloc
    clock_t start_malloc = clock();

    for (int i = 0; i < ITERATIONS; i++)
    {
        // Create and use a malloc'd array
        int *arr = (int *) malloc(ARRAY_SIZE * sizeof(int));
        if (arr)
        {
            arr[0] = i;  // Same operation as with VLA
            arr[ARRAY_SIZE - 1] = i + arr[0];
            free(arr);
        }
    }

    clock_t end_malloc = clock();
    double time_malloc = ((double) (end_malloc - start_malloc)) / CLOCKS_PER_SEC;

    // Print results
    printf("Time with VLA: %.4f seconds\n", time_vla);
    printf("Time with malloc: %.4f seconds\n", time_malloc);
    printf("VLA is %.2f times faster for this test\n", time_malloc / time_vla);
}

// Explain VLA limitations and best practices
void explain_vla()
{
    printf("\n=== VLA Limitations and Best Practices ===\n");

    printf("Limitations of VLAs:\n");
    printf("1. Stack size constraints - can cause stack overflow\n");
    printf("2. Not available at file scope (only within function blocks)\n");
    printf("3. Cannot be returned from functions\n");
    printf("4. Cannot be used in struct/union definitions\n");
    printf("5. Made optional in C11 (not all compilers support them)\n");

    printf("\nBest Practices:\n");
    printf("1. Use for small-to-moderate sized arrays\n");
    printf("2. Always validate the size before creating a VLA\n");
    printf("3. Consider stack size limitations of your target platform\n");
    printf("4. Use malloc() for very large or long-lived arrays\n");
    printf("5. Check for compiler support if targeting C11\n");
}

int main()
{
    printf("==== VARIABLE-LENGTH ARRAYS ====\n");

    // Simple VLA demonstration
    int size = 5;
    process_data_with_vla(size);

    // Compare with traditional malloc
    process_data_with_malloc(size);

    // Multi-dimensional VLA
    demonstrate_multi_dim_vla(3, 4);

    // Create a matrix and process it
    int rows = 3, cols = 3;
    int my_matrix[rows][cols];

    // Initialize matrix
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            my_matrix[i][j] = i + j;
        }
    }

    // Process the matrix
    process_matrix(rows, cols, my_matrix);

    // Print the modified matrix
    printf("\nModified matrix:\n");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%d ", my_matrix[i][j]);
        }
        printf("\n");
    }

    // Run benchmark
    benchmark_vla_vs_malloc();

    // Explain limitations
    explain_vla();

    return 0;
}
