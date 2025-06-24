#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 30
#define DATA_FILE "students.dat"

typedef struct
{
    int id;
    char name[MAX_NAME_LEN];
    float score;
} Student;

void add_student(void);
void display_all_students(void);
void search_student_by_id(void);
void update_student_score(void);
bool save_to_file(Student *student);
int load_from_file(Student *student, int max_size);

int main(int argc, char *argv[])
{
    int choice;

    do
    {
        printf("\n----- Student Management System -----\n");
        printf("1. Add New Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student by ID\n");
        printf("4. Update Student Score\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 0:
            printf("Exiting program.\n");
            break;
        case 1:
            add_student();
            break;
        case 2:
            display_all_students();
            break;
        case 3:
            search_student_by_id();
            break;
        case 4:
            update_student_score();
            break;
        default:
            break;
        }
    } while (choice != 0);

    return 0;
}

void add_student(void)
{
    Student new_student;
    printf("Enter student ID: ");
    scanf("%d", &new_student.id);

    printf("Enter student name: ");
    scanf("%s", new_student.name);

    printf("Enter student score: ");
    scanf("%f", &new_student.score);

    if (save_to_file(&new_student))
    {
        printf("Student added successfully!\n");
    }
    else
    {
        printf("Error saving student data.\n");
    }
}

bool save_to_file(Student *student)
{
    FILE *fp;
    fp = fopen(DATA_FILE, "ab");
    if (fp == NULL)
    {
        perror("Error opening file ");
        return false;
    }

    if (fwrite(student, sizeof(Student), 1, fp) != 1)
    {
        fclose(fp);
        return false;
    }

    fclose(fp);
    return true;
}

void display_all_students(void)
{
    Student students[100];
    int count, i;

    count = load_from_file(students, 100);

    if (count > 0)
    {
        printf("\n--- Student Records ---\n");
        printf("ID\tName\t\tScore\n");
        printf("-----------------------------\n");

        for (i = 0; i < count; i++)
        {
            printf("%d\t%-15s\t%.2f\n", students[i].id, students[i].name, students[i].score);
        }
    }
    else
    {
        printf("No student records found.\n");
    }
}

int load_from_file(Student *students, int max_size)
{
    FILE *fp;
    int count = 0;

    fp = fopen(DATA_FILE, "rb");
    if (fp == NULL)
    {
        return 0;
    }

    while (count < max_size && fread(&students[count], sizeof(Student), 1, fp) == 1)
    {
        count++;
    }
    fclose(fp);
    return count;
}

void search_student_by_id(void)
{
    Student students[100];
    int count, i, search_id;
    bool found = false;

    printf("Enter student ID to search: ");
    scanf("%d", &search_id);

    for (i = 0; i < count; i++)
    {
        if (students[i].id == search_id)
        {
            printf("\nStudent found!\n");
            printf("ID: %d\n", students[i].id);
            printf("Name: %s\n", students[i].name);
            printf("Score: %.2f\n", students[i].score);
            found = true;
        }
    }

    if (!found)
    {
        printf("Student with ID %d not found.\n", search_id);
    }
}

void update_student_score(void)
{
    Student students[100];
    int count, i, update_id;
    float new_score;
    bool found = false;
    FILE *fp;

    printf("Enter student ID to update: ");
    scanf("%d", &update_id);

    printf("Enter new score: ");
    scanf("%f", &new_score);

    count = load_from_file(students, 100);

    for (i = 0; i < count; i++)
    {
        if (students[i].id == update_id)
        {
            students[i].score = new_score;
            found = true;
            break;
        }
    }

    if (found)
    {
        fp = fopen(DATA_FILE, "wb");
        if (fp == NULL)
        {
            perror("Error opening file ");
            return;
        }

        if (fwrite(students, sizeof(Student), count, fp) != count)
        {
            printf("Error writing to file.\n");
        }
        else
        {
            printf("Student score updated successfully!\n");
        }

        fclose(fp);
    }
    else
    {
        printf("Student with ID %d not found.\n", update_id);
    }
}
