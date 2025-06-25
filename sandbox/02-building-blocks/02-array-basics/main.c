#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // Array Initialization
    // Method 1: Declaration without initialization (contains garbage values)
    int nums_0[5];

    // Method 2: Zero initialization using empty braces
    int nums_1[5] = {};

    // Method 3: Initialization with explicit values (size inferred from
    // initializer)
    int nums_2[] = {1, 3, 7, 9, -23};

    // Method 4: Using memset to initialize all bytes to zero
    int nums_3[5];
    memset(nums_3, 0, sizeof(nums_3));

    // Method 5: Partial initialization
    // specified elements initialized, rest set to zero
    int nums_4[5] = {1, 2};

    // Method 6: Designated initializers (C99)
    // specify which indexes to initialize
    int nums_5[5] = {[0] = 99, [2] = 66};

    // Method 7: Compound Literals (C99)
    int *nums_6 = (int[]) {1, 2, 3, 4, 5};  // Creates an anonymous array

    // Method 8: Dynamic allocation with calloc (zeros all elements)
    int *nums_7 = (int *) calloc(5, sizeof(int));

    // Display results of different initialization methods
    printf("Method 1 - Uninitialized array:      nums_0[0]=%d, nums_0[1]=%d\n",
           nums_0[0],
           nums_0[1]);

    printf(
        "Method 2 - Empty braces initialization: nums_1[0]=%d, nums_1[1]=%d\n",
        nums_1[0],
        nums_1[1]);

    printf(
        "Method 3 - Explicit initialization:     nums_2[0]=%d, nums_2[1]=%d\n",
        nums_2[0],
        nums_2[1]);

    printf(
        "Method 4 - memset() initialization:     nums_3[0]=%d, nums_3[1]=%d\n",
        nums_3[0],
        nums_3[1]);

    printf(
        "Method 5 - Partial initialization:      nums_4[0]=%d, nums_4[2]=%d\n",
        nums_4[0],
        nums_4[2]);

    printf(
        "Method 6 - Designated initializers:     nums_5[0]=%d, nums_5[2]=%d, "
        "nums_5[3]=%d\n",
        nums_5[0],
        nums_5[2],
        nums_5[3]);

    printf(
        "Method 7 - Compound Literals:           nums_6[0]=%d, nums_6[2]=%d\n",
        nums_6[0],
        nums_6[2]);

    printf(
        "Method 8 - Dynamic allocation:          nums_7[0]=%d, nums_7[2]=%d\n",
        nums_7[0],
        nums_7[2]);
    return 0;
}
