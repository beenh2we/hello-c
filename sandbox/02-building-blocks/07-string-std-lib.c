#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    printf("\n---------------------- Standard String Functions in C ----------------------\n\n");

    /*** 1. Basic String Length and Memory ***/
    printf("===== STRING LENGTH AND BASIC OPERATIONS =====\n");
    char str1[50] = "Hello";
    char str2[] = "World";
    char str3[50] = "Hello";
    
    printf("strlen(str1) = %lu (counts characters without null terminator)\n", strlen(str1));
    printf("sizeof(str1) = %lu (total memory allocation in bytes)\n\n", sizeof(str1));

    /*** 2. String Copying Functions ***/
    printf("===== STRING COPYING =====\n");
    strcpy(str1, "Bonjour");
    printf("strcpy(str1, \"Bonjour\")  → str1: \"%s\"\n", str1);
    
    char limited[10];
    strncpy(limited, "Very very long string.", 9); 
    limited[9] = '\0'; // IMPORTANT: strncpy doesn't guarantee null termination
    printf("strncpy with manual null: \"%s\"\n\n", limited);

    /*** 3. String Concatenation Functions ***/
    printf("===== STRING CONCATENATION =====\n");
    strcpy(str1, "Hello");
    strcat(str1, " ");
    strcat(str1, str2);
    printf("strcat results: \"%s\"\n", str1);
    
    char dest[10] = "ABC";
    strncat(dest, "DEFGHIJKLMNOPQ", 6);
    printf("strncat results: \"%s\" (length=%lu)\n\n", dest, strlen(dest));

    /*** 4. String Comparison Functions ***/
    printf("===== STRING COMPARISON =====\n");
    printf("strcmp(\"Hello\", \"World\"): %d (negative: first < second)\n", 
           strcmp("Hello", "World"));

    printf("strcmp(\"Hello\", \"Hello\"): %d (zero: strings equal)\n", 
           strcmp("Hello", "Hello"));

    printf("strcmp(\"World\", \"Hello\"): %d (positive: first > second)\n\n", 
           strcmp("World", "Hello"));

    /*** 5. String Searching Functions ***/
    printf("===== STRING SEARCHING =====\n");
    char text[] = "Find the first 'e' character";
    char *found = strchr(text, 'e');
    if (found != NULL) {
        printf("strchr found 'e' at position: %ld\n", found - text);
    }
    
    char haystack[] = "Finding a needle in a haystack";
    char *needle = strstr(haystack, "needle");
    if (needle != NULL) {
        printf("strstr found \"needle\" at position: %ld\n\n", needle - haystack);
    }

    /*** 6. String Manipulation Challenges and Safety Issues ***/
    /*
     * 1. Buffer Overflow:
     * When copying strings without checking size, you can write beyond 
     * the bounds of your destination buffer
     */
    char small[5]; // Only enough space for 4 chars + '\0'
    // strcpy(small, "This string is too long");  // DANGEROUS!
    // This would write past the end of small, corrupting adjacent memory
    
    /*
     * 2. Missing Null Terminator:
     * Using strncpy without ensuring null termination is a common error
     */
    char str[5];
    strncpy(str, "Hello", 5);  // Fills all 5 bytes with "Hello", no room for '\0'
    // printf("%s", str);      // DANGEROUS! Will read beyond array bounds
    
    /*
     * 3. Using Uninitialized Memory:
     * Using pointers before they point to valid memory
     */
    char *ptr;  // Uninitialized pointer
    // strcpy(ptr, "Hello");   // DANGEROUS! Writing to random memory location
    
    /*** 7. Safer String Handling ***/
    printf("\n===== SAFER STRING HANDLING TECHNIQUES =====\n");
    
    /*
     * 1. Always Check Buffer Sizes before copying
     */
    char buffer[10];
    const char *source = "A long string that won't fit";
    
    // Safe approach 1: Check first
    if (strlen(source) < sizeof(buffer)) {
        strcpy(buffer, source);
    } else {
        strncpy(buffer, source, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';  // Ensure null termination
    }
    printf("1. Buffer size checking: \"%s\"\n", buffer);
    
    // Safe approach 2: Use snprintf
    snprintf(buffer, sizeof(buffer), "%s", source);
    printf("2. Using snprintf: \"%s\"\n", buffer);
    
    /*
     * 2. Always Null-Terminate After strncpy
     * strncpy doesn't guarantee null termination if the source is too large
     */
    char dest2[10];
    strncpy(dest2, "0123456789ABCDEF", 9);  // Copy 9 chars from a 16-char string
    dest2[9] = '\0';  // Explicitly ensure null termination
    printf("3. Manual null termination: \"%s\"\n", dest2);
    
    /*
     * 3. Use Dynamically Allocated Memory for Variable-Length Strings
     * This allows exact sizing based on the source string
     */
    char *dynamic_str;
    const char *dynamic_source = "Dynamic memory allows flexible string sizes";
    
    // Allocate exactly enough memory (+1 for null terminator)
    dynamic_str = (char *)malloc(strlen(dynamic_source) + 1);
    
    if (dynamic_str != NULL) {
        strcpy(dynamic_str, dynamic_source);
        printf("4. Dynamic allocation: \"%s\"\n", dynamic_str);
        free(dynamic_str);  // Don't forget to free the memory when done!
    }

    return 0;
}