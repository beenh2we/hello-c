#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    printf("\n--- String Operations in C ---\n\n");

    // 1. STRING LENGTH
    printf("1. STRING LENGTH\n");
    char sample[50] = "Hello";

    printf("strlen(\"%s\"):     %lu (characters without null)\n", sample, strlen(sample));
    printf("sizeof(sample):  %lu (total bytes allocated)\n\n", sizeof(sample));

    // 2. STRING COPYING
    printf("2. STRING COPYING\n");
    char src[] = "Hello, C";
    char dest1[20];
    int i;

    // Manual character-by-character copying
    for (i = 0; src[i] != '\0'; i++)
    {
        dest1[i] = src[i];
    }
    dest1[i] = '\0';  // Manually add null terminator
    printf("Manual copy:     \"%s\"\n", dest1);

    // Standard library equivalent
    char dest2[20];
    strcpy(dest2, src);
    printf("strcpy():        \"%s\"\n", dest2);

    // Safer standard library alternative
    char dest3[20];
    strncpy(dest3, src, sizeof(dest3) - 1);
    dest3[sizeof(dest3) - 1] = '\0';  // Ensure null termination
    printf("strncpy():       \"%s\" (safer, length-limited)\n\n", dest3);

    // 3. STRING COMPARISON
    printf("3. STRING COMPARISON\n");
    char str1[] = "Apple";
    char str2[] = "Apple";
    char str3[] = "Orange";

    // Manual comparison implementation
    i = 0;
    while (str1[i] != '\0' && str2[i] != '\0' && str1[i] == str2[i])
    {
        i++;
    }
    int result = str1[i] - str2[i];

    // Standard library equivalent
    int cmp1 = strcmp(str1, str2);
    int cmp2 = strcmp(str1, str3);

    printf("Manual compare:  %d (str1 & str2)\n", result);
    printf("strcmp(equal):   %d (str1 & str2)\n", cmp1);
    printf("strcmp(< 0):     %d (str1 < str3)\n", cmp2);
    printf("strcmp(> 0):     %d (\"World\" > \"Hello\")\n\n", strcmp("World", "Hello"));

    // 4. STRING CONCATENATION
    printf("4. STRING CONCATENATION\n");
    char base1[30] = "Hello, ";
    char add[] = "World!";

    // Manual concatenation
    for (i = 0; base1[i] != '\0'; i++);
    int j = 0;
    for (j = 0; add[j] != '\0'; j++)
    {
        base1[i + j] = add[j];
    }
    base1[i + j] = '\0';
    printf("Manual concat:   \"%s\"\n", base1);

    // Standard library equivalent
    char base2[30] = "Hello, ";
    strcat(base2, add);
    printf("strcat():        \"%s\"\n", base2);

    // Safer standard library alternative
    char base3[30] = "Hello, ";
    strncat(base3, add, sizeof(base3) - strlen(base3) - 1);
    printf("strncat():       \"%s\" (safer, length-limited)\n\n", base3);

    // 5. STRING SEARCHING
    printf("5. STRING SEARCHING\n");
    char text[] = "Find the first 'e' character";
    char *found = strchr(text, 'e');
    if (found != NULL)
    {
        printf("strchr():        'e' found at position %ld\n", found - text);
    }

    char haystack[] = "Finding a needle in a haystack";
    char *needle = strstr(haystack, "needle");
    if (needle != NULL)
    {
        printf("strstr():        \"needle\" found at position %ld\n\n", needle - haystack);
    }

    // 6. SAFETY ISSUES & SOLUTIONS
    printf("6. SAFETY ISSUES & SOLUTIONS\n");

    printf("Common problems:\n");
    printf("- Buffer overflows when copying without size checks\n");
    printf("- Missing null termination with strncpy()\n");
    printf("- Using uninitialized memory\n\n");

    printf("Solutions:\n");

    // Buffer size checking example
    char buffer[10];
    const char *long_str = "A long string that won't fit";

    // Safe approach 1: Check first
    if (strlen(long_str) < sizeof(buffer))
    {
        strcpy(buffer, long_str);
    }
    else
    {
        strncpy(buffer, long_str, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';
    }
    printf("1. Size check:   \"%s\"\n", buffer);

    // Safe approach 2: Use snprintf
    snprintf(buffer, sizeof(buffer), "%s", long_str);
    printf("2. snprintf:     \"%s\"\n", buffer);

    // Dynamic memory allocation
    char *dyn_str;
    const char *dyn_src = "Dynamic memory for strings";

    // Allocate exactly enough memory (+1 for null terminator)
    dyn_str = (char *) malloc(strlen(dyn_src) + 1);

    if (dyn_str != NULL)
    {
        strcpy(dyn_str, dyn_src);
        printf("3. Dynamic mem:  \"%s\"\n", dyn_str);
        free(dyn_str);  // Free the memory when done
    }

    return 0;
}
