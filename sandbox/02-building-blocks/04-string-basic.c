#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    printf("\n-------------- String Representation in C --------------\n\n");

    // Method 1: Character-by-character initialization
    printf("Method 1: Character-by-character initialization\n");
    char str1[6] = {'H', 'e', 'l', 'l', 'o', '\0'};  // Explicit null terminator
    char str2[] = {'H', 'e', 'l', 'l', 'o', '\0'};   // Size determined by compiler

    printf("str1: \"%s\" (size: %zu bytes)\n", str1, sizeof(str1));
    printf("str2: \"%s\" (size: %zu bytes)\n\n", str2, sizeof(str2));


    // Method 2: String literal initialization
    printf("Method 2: String literal initialization\n");
    char str3[10] = "Hello";  // Fixed size array, string literal copied to stack
    char str4[] = "Hello";    // Size determined by compiler (6 bytes including '\0')
                              // String literal copied to stack memory
    char *str5 = "Hello";     // Pointer to string literal in read-only memory segment
                              // No copy is made; pointer just references the literal
    
    printf("str3: \"%s\" (size: %zu bytes, stored on stack)\n", str3, sizeof(str3));
    printf("str4: \"%s\" (size: %zu bytes, stored on stack)\n", str4, sizeof(str4));
    printf("str5: \"%s\" (pointer to read-only memory, sizeof pointer: %zu)\n\n", str5, sizeof(str5));


    // Method 3: String assignment after declaration
    printf("Method 3: String assignment after declaration\n");
    char str6[10];
    // str6 = "Hello";  // WRONG! Can't assign to arrays after declaration
    strcpy(str6, "Hello");    // Correct way to assign strings after declaration
    
    printf("str6: \"%s\" (after strcpy)\n\n", str6);

    // Demonstrate modifiability
    str4[0] = 'J';  // OK: modifying array element
    printf("After str4[0] = 'J': str4 = \"%s\"\n", str4);

        
    // In modern C, attempting to modify string literals is undefined behavior
    // str5[0] = 'J';  // DANGEROUS: Runtime error - modifying read-only memory

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
    printf("\n------------------------------------------------------\n");

    return 0;
}