#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
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
    printf("strcmp(name, \"Bibibubu Lu\") = %d\n", 
        strcmp(name, "Bibibubu Lu"));

    printf("strcmp(name, \"bibibubu lu\") = %d\n", 
        strcmp(name, "bibibubu lu"));

    if (strcmp(name, "Bibibubu Lu") == 0) {
        printf("Confirmed: name equals \"Bibibubu Lu\"\n");
    }

    // Demonstrate modifiability
    char str5[] = "Hello";    // Size determined by compiler (6 bytes including '\0')
                              // String literal copied to stack memory
    char *str6 = "Hello";     // Pointer to string literal in read-only memory segment
                              // No copy is made; pointer just references the literal

    str5[0] = 'J';  // OK: modifying array element
    printf("After str5[0] = 'J': str5 = \"%s\"\n", str5);

        
    // In modern C, attempting to modify string literals is undefined behavior
    // str6[0] = 'J';  // DANGEROUS: Runtime error - modifying read-only memory


 printf("\n-------------- String Null Termination --------------\n\n");

    // String with proper null termination
    char proper[] = { 'C', 'o', 'd', 'e', '\0' };
    printf("Proper string: \"%s\"\n", proper);
    printf("Proper string length: %zu characters\n", strlen(proper));

    // WARNING: String without null termination - DANGEROUS!
    char improper[] = { 'C', 'o', 'd', 'e' };
    printf("Improper string: \"%s\"\n", improper);

    // This may print "Code" followed by random characters until
    // it randomly encounters a 0 byte in memory

    // Safer demonstration
    for (int i = 0; i < 10; i++) {
        // Print each character to show the potential overflow
        char c = improper[i];
        // Printable ASCII range
        if (c >= 32 && c <= 126) {
            printf("%c", c);
        } else {
            // Show non-printable bytes as hex
            printf("[0x%02x]", c);
        }
        
        // Break if we encounter a null byte (by chance)
        if (c == '\0') {
            printf(" (encountered random null byte)\n");
            break;
        }
        
        // Safety limit to prevent excessive output
        if (i == 9) {
            printf("... (continues into undefined memory)\n");
        }
    }

    // DANGER: Accessing improper string as if it were null-terminated
    printf("\nDANGER: strlen(improper) = %zu (UNRELIABLE - reading beyond array!)\n", strlen(improper)); // Undefined behavior!
    
    /**
     * proper[]:    | 'C' | 'o' | 'd' | 'e' | '\0' |  <-- Safe, well-defined string
     * improper[]:  | 'C' | 'o' | 'd' | 'e' | ???  | ??? | ??? |  <-- Undefined boundary
     */
    return 0;
}