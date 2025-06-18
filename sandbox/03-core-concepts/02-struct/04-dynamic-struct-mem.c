#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[50];
    int id;
    float score;
} Student;

// Function to display student information (pass by value)
void displayStudent(Student s) {
    printf("Student: %s (ID: %d)\n", s.name, s.id);
    printf("Score: %.2f\n", s.score);
}

// Function to update student score (pass by reference)
void updateScore(Student *s, float newScore) {
    s->score = newScore;
}

// Function to create a new student dynamically
Student* createStudent(char *name, int id, float score) {
    Student *newStudent = (Student*)malloc(sizeof(Student));
    if (newStudent != NULL) {
        strcpy(newStudent->name, name);
        newStudent->id = id;
        newStudent->score = score;
    }
    return newStudent;
}

int main() {
    // Local structure
    Student student1 = {"Emily Brown", 67890, 85.5};
    
    // Display using pass-by-value
    displayStudent(student1);
    
    // Update using pass-by-reference
    updateScore(&student1, 92.5);
    printf("\nAfter update:\n");
    displayStudent(student1);
    
    // Create dynamic structure
    Student *student2 = createStudent("Mark Wilson", 54321, 78.1);
    if (student2 != NULL) {
        printf("\nDynamically created student:\n");
        displayStudent(*student2);
        free(student2);
    }
    
    return 0;
}