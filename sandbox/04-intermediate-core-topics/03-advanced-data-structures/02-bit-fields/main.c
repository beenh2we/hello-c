#include <stdio.h>

// Basic bit field example
struct PackedDate {
    unsigned int day   : 5;  // 5 bits for day (0-31)
    unsigned int month : 4;  // 4 bits for month (0-15)
    unsigned int year  : 12; // 12 bits for year (0-4095), can represent years 0-4095
};

// RGB color with bit fields
struct RGBColor {
    unsigned int blue  : 8;  // 8 bits for blue (0-255)
    unsigned int green : 8;  // 8 bits for green (0-255)
    unsigned int red   : 8;  // 8 bits for red (0-255)
    unsigned int alpha : 8;  // 8 bits for alpha (0-255)
};

// Bit fields for flags (booleans)
struct FilePermissions {
    unsigned int read    : 1;  // 1 bit for read permission
    unsigned int write   : 1;  // 1 bit for write permission
    unsigned int execute : 1;  // 1 bit for execute permission
    unsigned int system  : 1;  // 1 bit for system flag
    unsigned int hidden  : 1;  // 1 bit for hidden flag
};

// Hardware register emulation
struct ControlRegister {
    unsigned int enable      : 1;  // Enable bit
    unsigned int direction   : 1;  // 0 = input, 1 = output
    unsigned int mode        : 2;  // Mode (0-3)
    unsigned int interrupt   : 1;  // Interrupt enable
    unsigned int reserved    : 3;  // Reserved bits
};

// Example with signed bit fields
struct SignedBits {
    int positive : 4;  // Range: -8 to 7
    int negative : 4;  // Range: -8 to 7
};

// Example of bit field alignment issues
struct AlignmentTest {
    unsigned int flag1 : 1;
    unsigned int       : 0;  // Force alignment to next word boundary
    unsigned int flag2 : 1;
};

int main() {
    printf("==== BIT FIELDS EXAMPLES ====\n\n");

    // 1. Basic bit field example
    printf("--- Date Bit Field Example ---\n");
    
    struct PackedDate date = {15, 6, 2023}; // June 15, 2023
    
    printf("Date: %u/%u/%u\n", date.month, date.day, date.year);
    printf("Size of struct PackedDate: %zu bytes\n", sizeof(struct PackedDate));
    printf("Size if using regular ints: %zu bytes\n\n", 3 * sizeof(int));
    
    // Test bit field validation
    date.month = 15; // Outside valid month range (1-12)
    printf("Invalid month (15): %u (fits in 4 bits but not a valid month)\n", date.month);
    
    // 2. RGB Color Bit Field Example
    printf("\n--- RGB Color Bit Field Example ---\n");
    
    struct RGBColor color = {255, 128, 64, 255}; // RGBA: (64, 128, 255, 255)
    
    printf("Color (R,G,B,A): (%u, %u, %u, %u)\n", color.red, color.green, color.blue, color.alpha);
    printf("Size of struct RGBColor: %zu bytes\n", sizeof(struct RGBColor));
    
    // 3. Flags Bit Field Example
    printf("\n--- File Permissions Bit Field Example ---\n");
    
    struct FilePermissions perm = {1, 1, 0, 0, 0}; // read & write only
    
    printf("File permissions:\n");
    printf("- Read:    %s\n", perm.read    ? "Yes" : "No");
    printf("- Write:   %s\n", perm.write   ? "Yes" : "No");
    printf("- Execute: %s\n", perm.execute ? "Yes" : "No");
    printf("- System:  %s\n", perm.system  ? "Yes" : "No");
    printf("- Hidden:  %s\n", perm.hidden  ? "Yes" : "No");
    
    printf("Size of struct FilePermissions: %zu bytes\n", sizeof(struct FilePermissions));
    printf("Size compared to using 5 ints: %zu bytes\n\n", 5 * sizeof(int));
    
    // Toggle execution permission
    perm.execute = 1;
    printf("After setting execute permission:\n");
    printf("- Execute: %s\n\n", perm.execute ? "Yes" : "No");
    
    // 4. Hardware Register Example
    printf("--- Hardware Register Bit Field Example ---\n");
    
    struct ControlRegister reg = {1, 1, 2, 0, 0}; // Enabled, Output, Mode 2
    
    printf("Control Register State:\n");
    printf("- Enabled:    %s\n", reg.enable    ? "Yes" : "No");
    printf("- Direction:  %s\n", reg.direction ? "Output" : "Input");
    printf("- Mode:       %u\n", reg.mode);
    printf("- Interrupt:  %s\n", reg.interrupt ? "Enabled" : "Disabled");
    
    printf("\nRaw byte value: 0x%02X\n\n", 
           (reg.enable | (reg.direction << 1) | (reg.mode << 2) | (reg.interrupt << 4)));
    
    // 5. Signed Bit Fields Example
    printf("--- Signed Bit Fields Example ---\n");
    
    struct SignedBits sb = {7, -8}; // Max and min values for 4-bit signed integers
    
    printf("Positive field (7): %d\n", sb.positive);
    printf("Negative field (-8): %d\n\n", sb.negative);
    
    // Test overflow behavior
    sb.positive = 8; // This exceeds the range
    printf("Positive field after overflow (8 in 4 bits): %d\n", sb.positive);
    
    // 6. Alignment Example
    printf("\n--- Bit Field Alignment Example ---\n");
    
    struct AlignmentTest at;
    printf("Size of AlignmentTest: %zu bytes\n", sizeof(struct AlignmentTest));
    
    printf("\nNote: Bit field behavior can vary between compilers and platforms.\n");
    printf("Some aspects of bit fields are implementation-defined.\n");
    
    return 0;
}