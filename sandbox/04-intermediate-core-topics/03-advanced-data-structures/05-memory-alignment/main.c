#include <stdio.h>
#include <stddef.h>
#include <stdalign.h>  // C11 standard

// Basic structure with potential padding
struct BasicStruct {
    char a;       // 1 byte
    int b;        // 4 bytes
    char c;       // 1 byte
    double d;     // 8 bytes
};

// Reordered structure to minimize padding
struct ReorderedStruct {
    double d;     // 8 bytes
    int b;        // 4 bytes
    char a;       // 1 byte
    char c;       // 1 byte
    // 2 bytes of padding likely here
};

// Structure with array member
struct ArrayStruct {
    int id;
    double values[3];
};

// Structure with explicit padding
struct ExplicitPadding {
    char a;
    char padding1[3];  // Explicit padding to align b on 4-byte boundary
    int b;
    char c;
    char padding2[7];  // Explicit padding to align d on 8-byte boundary
    double d;
};

// Packed structure (might have different alignment requirements)
#pragma pack(push, 1)
struct PackedStruct {
    char a;
    int b;
    char c;
    double d;
};
#pragma pack(pop)

// Structure with alignment specifier (C11)
struct alignas(16) AlignedStruct {  // Force 16-byte alignment
    char a;
    int b;
    double c;
};

// Function to print address and offset information
void print_member_info(const char *struct_name, const char *member_name, 
                      void *struct_addr, void *member_addr, size_t member_size) {
    printf("  %s.%s:\n", struct_name, member_name);
    printf("    Address: %p\n", member_addr);
    printf("    Offset: %td bytes\n", (char*)member_addr - (char*)struct_addr);
    printf("    Size: %zu bytes\n", member_size);
}

// Function to show the memory layout of BasicStruct
void inspect_basic_struct() {
    struct BasicStruct s;
    
    printf("struct BasicStruct:\n");
    printf("  Total size: %zu bytes\n", sizeof(struct BasicStruct));
    
    print_member_info("BasicStruct", "a", &s, &s.a, sizeof(s.a));
    print_member_info("BasicStruct", "b", &s, &s.b, sizeof(s.b));
    print_member_info("BasicStruct", "c", &s, &s.c, sizeof(s.c));
    print_member_info("BasicStruct", "d", &s, &s.d, sizeof(s.d));
    
    // Calculate padding
    size_t expected_size = sizeof(s.a) + sizeof(s.b) + sizeof(s.c) + sizeof(s.d);
    size_t actual_size = sizeof(struct BasicStruct);
    printf("  Sum of member sizes: %zu bytes\n", expected_size);
    printf("  Padding bytes: %zu bytes\n", actual_size - expected_size);
}

// Function to show the memory layout of ReorderedStruct
void inspect_reordered_struct() {
    struct ReorderedStruct s;
    
    printf("\nstruct ReorderedStruct:\n");
    printf("  Total size: %zu bytes\n", sizeof(struct ReorderedStruct));
    
    print_member_info("ReorderedStruct", "d", &s, &s.d, sizeof(s.d));
    print_member_info("ReorderedStruct", "b", &s, &s.b, sizeof(s.b));
    print_member_info("ReorderedStruct", "a", &s, &s.a, sizeof(s.a));
    print_member_info("ReorderedStruct", "c", &s, &s.c, sizeof(s.c));
    
    // Calculate padding
    size_t expected_size = sizeof(s.a) + sizeof(s.b) + sizeof(s.c) + sizeof(s.d);
    size_t actual_size = sizeof(struct ReorderedStruct);
    printf("  Sum of member sizes: %zu bytes\n", expected_size);
    printf("  Padding bytes: %zu bytes\n", actual_size - expected_size);
}

// Function to show the memory layout of PackedStruct
void inspect_packed_struct() {
    struct PackedStruct s;
    
    printf("\nstruct PackedStruct (packed):\n");
    printf("  Total size: %zu bytes\n", sizeof(struct PackedStruct));
    
    print_member_info("PackedStruct", "a", &s, &s.a, sizeof(s.a));
    print_member_info("PackedStruct", "b", &s, &s.b, sizeof(s.b));
    print_member_info("PackedStruct", "c", &s, &s.c, sizeof(s.c));
    print_member_info("PackedStruct", "d", &s, &s.d, sizeof(s.d));
    
    // Calculate padding
    size_t expected_size = sizeof(s.a) + sizeof(s.b) + sizeof(s.c) + sizeof(s.d);
    size_t actual_size = sizeof(struct PackedStruct);
    printf("  Sum of member sizes: %zu bytes\n", expected_size);
    printf("  Padding bytes: %zu bytes\n", actual_size - expected_size);
}

// Function to demonstrate alignment requirements
void show_alignment_requirements() {
    printf("\n=== Alignment Requirements ===\n");
    
    printf("alignof(char): %zu bytes\n", alignof(char));
    printf("alignof(short): %zu bytes\n", alignof(short));
    printf("alignof(int): %zu bytes\n", alignof(int));
    printf("alignof(long): %zu bytes\n", alignof(long));
    printf("alignof(float): %zu bytes\n", alignof(float));
    printf("alignof(double): %zu bytes\n", alignof(double));
    printf("alignof(struct BasicStruct): %zu bytes\n", alignof(struct BasicStruct));
    printf("alignof(struct PackedStruct): %zu bytes\n", alignof(struct PackedStruct));
    printf("alignof(struct AlignedStruct): %zu bytes\n", alignof(struct AlignedStruct));
}

// Function to demonstrate proper alignment for arrays
void demonstrate_array_alignment() {
    printf("\n=== Array Element Alignment ===\n");
    
    struct ArrayStruct array_struct;
    
    printf("struct ArrayStruct:\n");
    printf("  Total size: %zu bytes\n", sizeof(struct ArrayStruct));
    
    print_member_info("ArrayStruct", "id", &array_struct, &array_struct.id, sizeof(array_struct.id));
    
    // Print info for each array element
    for (int i = 0; i < 3; i++) {
        char name[20];
        sprintf(name, "values[%d]", i);
        print_member_info("ArrayStruct", name, &array_struct, &array_struct.values[i], 
                         sizeof(array_struct.values[i]));
    }
}

// Function to explain alignment implications
void explain_alignment_implications() {
    printf("\n=== Alignment Implications ===\n");
    
    printf("1. Performance:\n");
    printf("   - Properly aligned data can be accessed faster\n");
    printf("   - Misaligned access might cause CPU penalties or exceptions\n\n");
    
    printf("2. Memory Usage:\n");
    printf("   - Alignment requirements can increase memory usage due to padding\n");
    printf("   - Careful structure design can reduce padding\n\n");
    
    printf("3. Hardware Compatibility:\n");
    printf("   - Some hardware requires specific alignment for certain operations\n");
    printf("   - Memory-mapped I/O needs strict alignment\n\n");
    
    printf("4. Structure Packing Tradeoffs:\n");
    printf("   - Packed structures save memory but may reduce performance\n");
    printf("   - Use packed structures only when necessary (e.g., file formats, network protocols)\n\n");
    
    printf("5. Cross-Platform Considerations:\n");
    printf("   - Different platforms might have different alignment requirements\n");
    printf("   - Explicit packing can help ensure consistent layout across platforms\n");
}

int main() {
    printf("==== MEMORY ALIGNMENT EXAMPLES ====\n\n");
    
    // Inspect the memory layouts of different structures
    inspect_basic_struct();
    inspect_reordered_struct();
    inspect_packed_struct();
    
    // Show alignment requirements
    show_alignment_requirements();
    
    // Demonstrate array alignment
    demonstrate_array_alignment();
    
    // Explain implications
    explain_alignment_implications();
    
    return 0;
}