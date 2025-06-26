#include <stddef.h>  // For ptrdiff_t
#include <stdio.h>

void basic_pointer_arithmetic()
{
    printf("--- Basic Pointer Arithmetic ---\n");

    int array[5] = {10, 20, 30, 40, 50};
    int *p = array;  // p points to the first element

    // Accessing array elements using pointer arithmetic
    printf("Array elements using pointer arithmetic:\n");
    printf("*p = %d\n", *p);
    printf("*(p+1) = %d\n", *(p + 1));
    printf("*(p+2) = %d\n", *(p + 2));
    printf("*(p+3) = %d\n", *(p + 3));
    printf("*(p+4) = %d\n", *(p + 4));

    // Incrementing pointers
    printf("\nIncrementing pointers:\n");
    printf("*p = %d\n", *p);
    p++;  // Move to next element
    printf("After p++, *p = %d\n", *p);
    p += 2;  // Skip ahead by 2 elements
    printf("After p += 2, *p = %d\n", *p);

    // Reset pointer
    p = array;

    // Relationship between array indexing and pointer arithmetic
    printf("\nArray indexing vs pointer arithmetic:\n");
    for (int i = 0; i < 5; i++)
    {
        printf("array[%d] = %d, *(p+%d) = %d\n", i, array[i], i, *(p + i));
    }
}

void type_scaling_demonstration()
{
    printf("\n--- Type Scaling in Pointer Arithmetic ---\n");

    // Allocate aligned memory to see address differences clearly
    int int_array[5] = {1, 2, 3, 4, 5};
    char char_array[5] = {'a', 'b', 'c', 'd', 'e'};
    double double_array[5] = {1.1, 2.2, 3.3, 4.4, 5.5};

    // Show address differences
    printf("int_array address scaling:\n");
    for (int i = 0; i < 5; i++)
    {
        printf("&int_array[%d] = %p (difference: %ld bytes)\n",
               i,
               (void *) &int_array[i],
               i == 0
                   ? 0
                   : (long) ((char *) &int_array[i] - (char *) &int_array[0]));
    }

    printf("\nchar_array address scaling:\n");
    for (int i = 0; i < 5; i++)
    {
        printf("&char_array[%d] = %p (difference: %ld bytes)\n",
               i,
               (void *) &char_array[i],
               i == 0 ? 0
                      : (long) ((char *) &char_array[i] -
                                (char *) &char_array[0]));
    }

    printf("\ndouble_array address scaling:\n");
    for (int i = 0; i < 5; i++)
    {
        printf("&double_array[%d] = %p (difference: %ld bytes)\n",
               i,
               (void *) &double_array[i],
               i == 0 ? 0
                      : (long) ((char *) &double_array[i] -
                                (char *) &double_array[0]));
    }

    // Demonstrate pointer scaling with different types
    printf("\nPointer arithmetic scaling by type:\n");
    int *int_ptr = int_array;
    char *char_ptr = char_array;
    double *double_ptr = double_array;

    printf("Size of int: %zu bytes\n", sizeof(int));
    printf("Size of char: %zu bytes\n", sizeof(char));
    printf("Size of double: %zu bytes\n", sizeof(double));

    printf("\nOriginal addresses:\n");
    printf("int_ptr = %p\n", (void *) int_ptr);
    printf("char_ptr = %p\n", (void *) char_ptr);
    printf("double_ptr = %p\n", (void *) double_ptr);

    printf("\nAfter adding 1 to each pointer:\n");
    printf("int_ptr + 1 = %p (added %zu bytes)\n",
           (void *) (int_ptr + 1),
           sizeof(int));
    printf("char_ptr + 1 = %p (added %zu bytes)\n",
           (void *) (char_ptr + 1),
           sizeof(char));
    printf("double_ptr + 1 = %p (added %zu bytes)\n",
           (void *) (double_ptr + 1),
           sizeof(double));
}

void pointer_difference_example()
{
    printf("\n--- Pointer Difference Example ---\n");

    int arr[10];
    int *ptr1 = &arr[2];
    int *ptr2 = &arr[7];

    // Calculate the difference between pointers
    ptrdiff_t diff1 = ptr2 - ptr1;
    ptrdiff_t diff2 = ptr1 - ptr2;

    printf("ptr1 = %p (points to arr[2])\n", (void *) ptr1);
    printf("ptr2 = %p (points to arr[7])\n", (void *) ptr2);
    printf("ptr2 - ptr1 = %td elements\n", diff1);
    printf("ptr1 - ptr2 = %td elements\n", diff2);

    // This gives us the number of elements between the pointers
    printf("\nBytes between pointers: %td\n", (char *) ptr2 - (char *) ptr1);
}

int main(int argc, char *argv[])
{
    basic_pointer_arithmetic();

    type_scaling_demonstration();

    pointer_difference_example();
}
