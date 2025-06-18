#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    printf("\n--------------- String Operations: Manual vs Standard ---------------\n\n");

    // 1. STRING COPYING
    printf("1. STRING COPYING\n");
    char destination[20];  // Destination buffer with fixed capacity
    char source[] = "Hello, C";
    int i;

    // Manual character-by-character copying
    for (i = 0; source[i] != '\0'; i++) {
        destination[i] = source[i];
    }
    destination[i] = '\0';  // Manually add null terminator
    printf("Manual copy:     \"%s\"\n", destination);

    // Standard library equivalent
    char dest_strcpy[20];
    strcpy(dest_strcpy, source);
    printf("strcpy():        \"%s\"\n", dest_strcpy);
    
    // Safer standard library alternative
    char dest_strncpy[20];
    strncpy(dest_strncpy, source, sizeof(dest_strncpy) - 1);
    dest_strncpy[sizeof(dest_strncpy) - 1] = '\0'; // Ensure null termination
    printf("strncpy():       \"%s\" (safer, length-limited)\n\n", dest_strncpy);

    // 2. STRING COMPARISON
    printf("2. STRING COMPARISON\n");
    char str1[] = "Apple";
    char str2[] = "Apple";
    char str3[] = "Orange";

    // Manual comparison implementation
    i = 0;
    while (str1[i] != '\0' && str2[i] != '\0' && str1[i] == str2[i]) {
        i++;
    }
    int result1 = str1[i] - str2[i];
    
    // Standard library equivalent
    int strcmp_result1 = strcmp(str1, str2);
    int strcmp_result2 = strcmp(str1, str3);
    
    printf("Manual comparison str1 & str2: %d\n", result1);
    printf("strcmp(str1, str2):           %d\n", strcmp_result1);
    printf("strcmp(str1, str3):           %d (negative: str1 < str3)\n\n", strcmp_result2);
    
    // 3. STRING CONCATENATION
    printf("3. STRING CONCATENATION\n");
    char result_manual[30] = "Hello, ";
    char additional[] = "World!";

    // Manual concatenation
    for (i = 0; result_manual[i] != '\0'; i++);
    int j = 0;
    for (j = 0; additional[j] != '\0'; j++) {
        result_manual[i + j] = additional[j];
    }
    result_manual[i + j] = '\0';
    printf("Manual concatenation: \"%s\"\n", result_manual);

    // Standard library equivalent
    char result_strcat[30] = "Hello, ";
    strcat(result_strcat, additional);
    printf("strcat():            \"%s\"\n", result_strcat);
    
    // Safer standard library alternative
    char result_strncat[30] = "Hello, ";
    strncat(result_strncat, additional, sizeof(result_strncat) - strlen(result_strncat) - 1);
    printf("strncat():           \"%s\" (safer, length-limited)\n\n", result_strncat);

    return 0;
}