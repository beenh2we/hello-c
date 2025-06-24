#include <stdio.h>
#include <string.h>

typedef struct
{
    char name[50];
    int id;
    float scores[3];  // Array within a structure
} Student;

int main()
{
    // Array of structures
    Student students[2];

    // Initialize first student
    strcpy(students[0].name, "Emily");
    students[0].id = 1001;
    students[0].scores[0] = 92.5;
    students[0].scores[1] = 88.0;
    students[0].scores[2] = 95.5;

    // Initialize second student using a temporary structure
    Student john = {"John", 1002, {85.0, 90.5, 82.0}};
    students[1] = john;

    // Display student information
    for (int i = 0; i < 2; i++)
    {
        printf("Student %d: %s (ID: %d)\n", i + 1, students[i].name, students[i].id);

        printf("Scores: ");
        for (int j = 0; j < 3; j++)
        {
            printf("%.1f ", students[i].scores[j]);
        }
        printf("\n\n");
    }

    return 0;
}
