#include <stdio.h>
#include <string.h>

// Basic union example
union Data {
    int i;
    float f;
    char str[20];
};

// Tagged union example (type-safe union)
enum DataType {
    INTEGER_TYPE,
    FLOAT_TYPE,
    STRING_TYPE
};

typedef struct {
    enum DataType type;
    union {
        int i;
        float f;
        char str[20];
    } value;
} TaggedData;

// Example of union for data conversion (type punning)
union Converter {
    int i;
    float f;
    unsigned char bytes[4]; // Used to access individual bytes
};

// Union for memory conservation
struct Student {
    char name[50];
    int id;
    char grade;
    // Only one of these fields will be used at a time
    union {
        struct {
            char course_name[30];
            float gpa;
        } college;
        struct {
            char school_name[30];
            int class_rank;
        } highschool;
    } education;
};

int main() {
    printf("==== UNION EXAMPLES ====\n\n");
    
    // 1. Basic Union Example
    printf("--- Basic Union Example ---\n");
    
    union Data data;
    
    // Demonstrating shared memory space
    data.i = 42;
    printf("data.i: %d\n", data.i);
    
    data.f = 3.14f;
    printf("data.f: %f\n", data.f);
    // The integer value is now corrupted because f overwrote that memory
    printf("data.i is now: %d (corrupted because memory is shared)\n", data.i);
    
    strcpy(data.str, "Hello");
    printf("data.str: %s\n", data.str);
    // Now both i and f are corrupted
    printf("After string assignment:\n");
    printf("data.i: %d\n", data.i);
    printf("data.f: %f\n\n", data.f);
    
    // Size of union is the size of largest member
    printf("Size of union Data: %zu bytes\n", sizeof(union Data));
    printf("Size of int: %zu bytes\n", sizeof(int));
    printf("Size of float: %zu bytes\n", sizeof(float));
    printf("Size of char[20]: %zu bytes\n\n", sizeof(char[20]));
    
    // 2. Tagged Union Example
    printf("--- Tagged Union Example ---\n");
    
    TaggedData safe_data;
    
    // Using as integer
    safe_data.type = INTEGER_TYPE;
    safe_data.value.i = 100;
    
    // Safe access using tag
    if (safe_data.type == INTEGER_TYPE) {
        printf("Integer value: %d\n", safe_data.value.i);
    }
    
    // Change to float
    safe_data.type = FLOAT_TYPE;
    safe_data.value.f = 99.99f;
    
    // Safe access with proper type checking
    switch (safe_data.type) {
        case INTEGER_TYPE:
            printf("Integer: %d\n", safe_data.value.i);
            break;
        case FLOAT_TYPE:
            printf("Float: %f\n", safe_data.value.f);
            break;
        case STRING_TYPE:
            printf("String: %s\n", safe_data.value.str);
            break;
    }
    printf("\n");
    
    // 3. Type Punning Example
    printf("--- Type Punning Example ---\n");
    
    union Converter conv;
    conv.f = 3.14159f;
    
    // Access float as integer and bytes
    printf("As float: %f\n", conv.f);
    printf("As integer: %d\n", conv.i);
    printf("Individual bytes (hex): ");
    for (int i = 0; i < 4; i++) {
        printf("%02X ", conv.bytes[i]);
    }
    printf("\n\n");
    
    // 4. Memory Conservation Example
    printf("--- Memory Conservation Example ---\n");
    
    struct Student student1;
    strcpy(student1.name, "John Smith");
    student1.id = 12345;
    student1.grade = 'A';
    
    // Using college field
    strcpy(student1.education.college.course_name, "Computer Science");
    student1.education.college.gpa = 3.8f;
    
    printf("Student: %s (ID: %d, Grade: %c)\n", student1.name, student1.id, student1.grade);
    printf("College: %s, GPA: %.1f\n", student1.education.college.course_name, 
           student1.education.college.gpa);
    
    // Overwriting with high school info
    strcpy(student1.education.highschool.school_name, "Lincoln High");
    student1.education.highschool.class_rank = 5;
    
    printf("\nAfter change:\n");
    printf("Student: %s (ID: %d, Grade: %c)\n", student1.name, student1.id, student1.grade);
    printf("High School: %s, Class Rank: %d\n", 
           student1.education.highschool.school_name, 
           student1.education.highschool.class_rank);
    
    // Note: college data is now corrupted since we're using the high school fields
    printf("College course name is now corrupted: %s\n", 
           student1.education.college.course_name);
    
    return 0;
}