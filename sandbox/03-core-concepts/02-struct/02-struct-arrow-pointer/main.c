#include <stdio.h>
#include <string.h>

typedef struct {
    char name[50];
    int id;
    float score;
} Student;

int main() {
    Student student1;
    Student *ptr = &student1;
    
    // Using dot operator with structure
    strcpy(student1.name, "Alice Johnson");
    student1.id = 12345;
    student1.score = 92.5;
    
    printf("Using dot operator:\n");
    printf("Name: %s, ID: %d, Score: %.1f\n", 
           student1.name, student1.id, student1.score);
    
    // Using arrow operator with pointer to structure
    strcpy(ptr->name, "Bob Smith");  // Same as (*ptr).name
    ptr->id = 67890;                 // Same as (*ptr).id
    ptr->score = 85.0;               // Same as (*ptr).score
    
    printf("\nUsing arrow operator:\n");
    printf("Name: %s, ID: %d, Score: %.1f\n",
           ptr->name, ptr->id, ptr->score);
    
    return 0;
}