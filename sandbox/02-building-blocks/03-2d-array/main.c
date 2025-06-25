#include <stdio.h>

#define ROW    3
#define COLUMN 4

void printMatrix(char *name, int martrix[2][3])
{
    printf("%s:\n", name);
    for (int i = 0; i < 2; i++)
    {
        printf("[ ");
        for (int j = 0; j < 3; j++)
        {
            printf("%3d", martrix[i][j]);
            if (j < 3 - 1) printf(", ");
        }
        printf(" ]\n");
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    // 2D Array Initialization and Access
    // Method 1: Declaration with individual element assignments
    int matrix[ROW][COLUMN] = {};  // Initialize all elements to zero
    matrix[0][0] = 10;
    matrix[2][3] = 99;

    printf("matrix[%d][%d]:\n", ROW, COLUMN);
    for (int i = 0; i < ROW; i++)
    {
        printf("[ ");
        for (int j = 0; j < COLUMN; j++)
        {
            printf("%3d", matrix[i][j]);
            if (j < COLUMN - 1) printf(", ");
        }
        printf(" ]\n");
    }

    // Method 2: Full initialization with nested braces
    int grid[4][3] = {
        {32, 46, 83}, {21, 30, -12}, {-13, 412, 90}, {43, 21, 30}};

    printf("grid[4][3]:\n");
    for (int i = 0; i < 4; i++)
    {
        printf("[ ");
        for (int j = 0; j < 3; j++)
        {
            printf("%4d", grid[i][j]);
            if (j < 3 - 1) printf(", ");
        }
        printf(" ]\n");
    }

    // Method 3: Partial initialization (remaining elements are zero)
    int partial[][3] = {
        {2, 3, 4},
        {-3},     // Only first element specified, others are zero
        {-2, 93}  // First two elements specified, last is zero
    };

    printf("partial[3][3] (zeros for unspecified elements):\n");
    for (int i = 0; i < 3; i++)
    {
        printf("[ ");
        for (int j = 0; j < 3; j++)
        {
            printf("%4d", partial[i][j]);
            if (j < 3 - 1) printf(", ");
        }
        printf(" ]\n");
    }

    // Method 4: Flat initialization (row-major order)
    int flat[3][2] = {1, 2, 3, 4, 5, 6};  // Equivalent to {{1,2}, {3,4}, {5,6}}

    printf("flat[3][2]:\n");
    for (int i = 0; i < 3; i++)
    {
        printf("[ ");
        for (int j = 0; j < 2; j++)
        {
            printf("%2d", flat[i][j]);
            if (j < 2 - 1) printf(", ");
        }
        printf(" ]\n");
    }

    // 2D Array Manipulation
    int A[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int B[2][3] = {{7, 8, 9}, {10, 11, 12}};
    int C[2][3];

    // Matrix addition
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }

    printMatrix("Matrix A: ", A);
    printMatrix("Matrix B: ", B);

    printf("Result of matrix addition: C[2][3]\n");
    for (int i = 0; i < 2; i++)
    {
        printf("[ ");
        for (int j = 0; j < 3; j++)
        {
            printf("%3d", C[i][j]);
            if (j < 3 - 1) printf(", ");
        }
        printf(" ]\n");
    }

    return 0;
}
