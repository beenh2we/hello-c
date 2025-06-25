#include <stdio.h>
#include <stdlib.h>

void print_matrix(char *name, int matrix[][3], int rows)
{
    printf("%s[%d][3]: \n", name, rows);
    for (int i = 0; i < rows; i++)
    {
        printf("[");
        for (int j = 0; j < 3; j++)
        {
            printf("%3d", matrix[i][j]);
            if (j < 3 - 1) printf(", ");
        }
        printf("]\n");
    }
    printf("\n");
}

void print_matrix_alt(char *name, int (*matrix)[3], int rows)
{
    printf("%s using pointer to array notation: \n", name);

    // for (int i = 0; i < rows; i++)
    // {
    //     printf("[");
    //     for (int j = 0; j < 3; j++)
    //     {
    //         printf("%3d", (*(matrix + i))[j]);
    //         if (j < 3 - 1) printf(", ");
    //     }
    //     printf("]\n");
    // }

    // Easier way
    for (int i = 0; i < rows; i++)
    {
        printf("[%3d, %3d, %3d]\n", matrix[i][0], matrix[i][1], matrix[i][2]);
    }
    printf("\n");
}

void print_matrix_ptr(char *name, int *matrix, int rows, int cols)
{
    printf("%s using flat pointer arithmetic: \n", name);
    for (int i = 0; i < rows; i++)
    {
        printf("[");
        for (int j = 0; j < cols; j++)
        {
            // Access elements using pointer arithmetic
            printf("%3d", *(matrix + (i * cols) + j));
            if (j < cols - 1) printf(", ");
        }
        printf("]\n");
    }
    printf("\n");
}

int main(int argc, char const *argv[])
{
    int matrix_A[2][3] = {};
    matrix_A[0][0] = 99;
    matrix_A[1][2] = 66;
    print_matrix("matrix_A", matrix_A, 2);

    int matrix_B[][3] = {{99}, {-1, 66}};
    print_matrix("matrix_B", matrix_B, 2);

    print_matrix_alt("matrix_A", matrix_A, 2);
    print_matrix_ptr("matrix_B", (int *) matrix_B, 2, 3);

    // Designated initializers (C99)
    int matrix_C[3][3] = {[0][0] = 108, [0][2] = -99, [2][2] = 67};
    print_matrix("matrix_C", matrix_C, 3);

    // Array of pointers (different from 2D array)
    int *matrix_D[2];
    matrix_D[0] = (int[]) {11, 22, 33};  // Compound literals (C99)
    matrix_D[1] = (int[]) {44, 55, 66};

    // Can't use print_matrix with matrix_D since it's not a true 2D array
    printf("matrix_D (array of pointers):\n");
    for (int i = 0; i < 2; i++)
    {
        printf("[%3d, %3d, %3d]\n",
               matrix_D[i][0],
               matrix_D[i][1],
               matrix_D[i][2]);
    }
    printf("\n");

    // Dynamically allocated 2D array
    int rows = 2, cols = 3;
    int **dynamic_matrix = (int **) malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
    {
        dynamic_matrix[i] = (int *) malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++)
        {
            dynamic_matrix[i][j] = i * 10 + j;
        }
    }

    printf("dynamic_matrix:\n");
    for (int i = 0; i < rows; i++)
    {
        printf("[%3d, %3d, %3d]\n",
               dynamic_matrix[i][0],
               dynamic_matrix[i][1],
               dynamic_matrix[i][2]);
    }

    // Free allocated memory
    for (int i = 0; i < rows; i++)
    {
        free(dynamic_matrix[i]);
    }

    free(dynamic_matrix);

    return 0;
}
