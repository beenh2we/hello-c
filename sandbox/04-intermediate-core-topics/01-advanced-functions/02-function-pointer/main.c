#include <stdio.h>
#include <stdlib.h>

int add(int a, int b)
{
    return a + b;
}

int sub(int a, int b)
{
    return a - b;
}

// Higher-order function that takes a function pointer as an argument
int operate(int (*operation)(int, int), int a, int b)
{
    return operation(a, b);
}

// Callback function for array sorting
int compare_ints(const void *a, const void *b)
{
    return *(int *) a - *(int *) b;
}

// Function to print an array of integers
void print_array(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    int a = 10, b = 34;
    int (*op)(int, int);

    op = add;
    const int add_result = op(a, b);
    printf("add_result: %d\n", add_result);

    op = sub;
    const int sub_result = op(a, b);
    printf("sub_result: %d\n", sub_result);

    // Array of function pointers
    int (*operations[])(int, int) = {add, sub};
    char *op_names[] = {"Addition", "Subtraction"};

    printf("\nUsing array of function pointers:\n");
    for (int i = 0; i < 2; i++)
    {
        printf("%s: %d\n", op_names[i], operations[i](a, b));
    }

    op = add;
    const int higher_order = operate(op, 99, 1);
    printf("High order function demo result: %d\n", higher_order);

    // Using qsort with a comparison function pointer
    int numbers[] = {43, 22, 15, 87, 42, 31, 8, 56, 19, 62};
    size_t size = sizeof(numbers) / sizeof(numbers[0]);

    printf("\nArray before sorting: ");
    print_array(numbers, size);

    // Standard library qsort using our comparison function
    qsort(numbers, size, sizeof(int), compare_ints);

    printf("Array after sorting: ");
    print_array(numbers, size);

    return 0;
}
