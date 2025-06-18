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

    printf("\n------------------- Character Array, String -------------------\n\n");

    // Method 1: Explicitly sized array with character-by-character initialization
    char str1[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
    
    // Method 2: Auto-sized array with character-by-character initialization
    char str2[] = {'H', 'e', 'l', 'l', 'o', '\0'};
    
    // Method 3: Explicitly sized array with string literal
    char str3[20] = "Hello";
    
    // Method 4: Auto-sized array with string literal (most common)
    char str4[] = "Hello";

    // Display results with additional information
    printf("Method 1 - char str1[6] = {'H',...,'\\0'}:\n");
    printf("  Value: \"%s\"\n", str1);
    printf("  Length: %zu chars\n", strlen(str1));
    printf("  Size: %zu bytes\n\n", sizeof(str1));

    printf("Method 2 - char str2[] = {'H',...,'\\0'}:\n");
    printf("  Value: \"%s\"\n", str2);
    printf("  Length: %zu chars\n", strlen(str2));
    printf("  Size: %zu bytes\n\n", sizeof(str2));

    printf("Method 3 - char str3[20] = \"Hello\":\n");
    printf("  Value: \"%s\"\n", str3);
    printf("  Length: %zu chars\n", strlen(str3));
    printf("  Size: %zu bytes\n\n", sizeof(str3));

    printf("Method 4 - char str4[] = \"Hello\":\n");
    printf("  Value: \"%s\"\n", str4);
    printf("  Length: %zu chars\n", strlen(str4));
    printf("  Size: %zu bytes\n\n", sizeof(str4));

    printf("Note: Length is character count (excluding '\\0')\n");
    printf("      Size is total memory allocation in bytes\n");

    printf("\n------------------- String Manipulation -------------------\n\n");
    
    // String initialization
    char name[20] = "Bibibubu Lu";
    printf("Original string: \"%s\"\n", name);
    
    // String length measurement
    size_t length = strlen(name);
    printf("Length of string: %zu characters\n", length);
    
    // String copying demonstration
    char destination[20];
    strcpy(destination, name);
    printf("After strcpy():   \"%s\"\n", destination);
    
    // String concatenation
    strcat(destination, " Lulup");
    printf("After strcat():   \"%s\"\n", destination);
    
    // String comparison
    printf("\nString comparison results:\n");
    printf("strcmp(name, \"Bibibubu Lu\") = %d\n", strcmp(name, "Bibibubu Lu"));
    printf("strcmp(name, \"bibibubu lu\") = %d\n", strcmp(name, "bibibubu lu"));
    
    if (strcmp(name, "Bibibubu Lu") == 0) {
        printf("Confirmed: name equals \"Bibibubu Lu\"\n");
    }

    return 0;
}