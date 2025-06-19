#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
       
    printf("\n----------------- Array Initialization Methods -----------------\n\n");
    // Method 1: Declaration without initialization (contains garbage values)
    int nums_0[5];
    
    // Method 2: Zero initialization using empty braces
    int nums_1[5] = {};
    
    // Method 3: Initialization with explicit values (size inferred from initializer)
    int nums_2[] = { 1, 3, 7, 9, -23 };

    // Method 4: Using memset to initialize all bytes to zero
    int nums_3[5];
    memset(nums_3, 0, sizeof(nums_3));

    // Display results of different initialization methods
    printf("Method 1 - Uninitialized array:        nums_0[0] = %d, nums_0[1] = %d\n", 
           nums_0[0], nums_0[1]);
           
    printf("Method 2 - Empty braces initialization: nums_1[0] = %d, nums_1[1] = %d\n", 
           nums_1[0], nums_1[1]);
           
    printf("Method 3 - Explicit initialization:     nums_2[0] = %d, nums_2[1] = %d\n", 
           nums_2[0], nums_2[1]);
           
    printf("Method 4 - memset() initialization:     nums_3[0] = %d, nums_3[1] = %d\n", 
           nums_3[0], nums_3[1]);

    return 0;
}