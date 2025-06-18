#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    printf("\n------------- String Input/Output Methods -------------\n\n");
    
    /*** STRING INPUT METHODS ***/
    printf("===== STRING INPUT METHODS =====\n");
    
    // Method 1: fgets (safe, reads entire line)
    char name[20];
    printf("Enter your first name: ");
    fgets(name, sizeof(name), stdin);
    
    // fgets keeps the newline character, so we need to remove it manually
    for (int i = 0; name[i] != '\0'; i++) {
        if (name[i] == '\n') {
            name[i] = '\0';  // Manually replace newline with null terminator
            break;
        }
    }
    
    // Method 2: scanf with field width limit (safe for single words)
    char lastname[20];
    printf("Enter your last name: ");
    scanf("%19s", lastname);  // The %19s prevents buffer overflow
    
    printf("Full name: %s %s\n\n", name, lastname);
    
    /*** STRING OUTPUT METHODS ***/
    printf("===== STRING OUTPUT METHODS =====\n");
    char message[] = "Learning C programming";
    
    // Method 1: printf (flexible formatting)
    printf("Method 1 - printf(): %s\n", message);
    
    // Method 2: puts (simpler, adds newline automatically)
    printf("Method 2 - puts(): ");
    puts(message);
    
    // Method 3: putchar (character by character)
    printf("Method 3 - putchar(): ");
    for (int i = 0; message[i] != '\0'; i++) {
        putchar(message[i]);  // Loop continues until null terminator
    }
    putchar('\n');

    return 0;
}