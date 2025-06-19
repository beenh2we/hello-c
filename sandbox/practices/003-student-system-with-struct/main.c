#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STUDENTS 50
#define MAX_NAME_LEN 30

typedef struct {
    char name[MAX_NAME_LEN];
    float score;
} Student;

void input_student_data(Student *students, int n);
void display_student_data(const Student *students, int n);
float calculate_average(const Student *students, int n);
const Student* find_highest_score(const Student *students, int n);
const Student* find_lowest_score(const Student *students, int n);
void sort_by_score(Student *students, int n);

int main(int argc, char *argv[]) {

    int choice;
    int student_count = 0;
    Student students[MAX_STUDENTS];
    bool data_entered = false;

    do {
        printf("\t\t\t ------ Menu ------ \t\t\t\n\n");
        printf("1. Input students data.\n");
        printf("2. Display all students data.\n");
        printf("3. Calculate the average score.\n");
        printf("4. Search highest and lowest scores.\n");
        printf("5. Sort by scores\n");
        printf("0. exit\n");

        printf(" -- Please select(1~5): ");
        scanf("%d", &choice);

        switch (choice) {
            case 0:
                printf("Bye, see you next time!\n");
                return 0;

            case 1:
                printf(
                    "Please enter the students.. How many students? (Should less then %d): ",
                    MAX_STUDENTS
                );
                scanf("%d", &student_count);

                if (student_count > 0 && student_count < MAX_STUDENTS) {
                    input_student_data(
                        students,
                        student_count
                    );
                    data_entered = true;
                } else {
                    printf("Invalid student numbers!\n");
                }

                break;
            case 2:
                if (data_entered) {
                    display_student_data(
                        students,
                        student_count
                    );
                } else {
                    printf("Please input students data first.\n");
                }
                break;
            case 3:
                if (data_entered) {
                    float average = calculate_average(students, student_count);
                    printf("The average score in your class is %.2f\n", average);
                } else {
                    printf("Please input students data first.\n");
                }
                break;
            case 4:
                if (data_entered) {
                    const Student *highest = find_highest_score(students, student_count);
                    const Student *lowest = find_lowest_score(students, student_count);
                    printf(
                        "The highest score in your class is %.2f, lowest is: %.2f\n",
                        highest -> score,
                        lowest -> score
                    );
                } else {
                    printf("Please input students data first.\n");
                }
                break;
            case 5:
                if (data_entered) {
                    sort_by_score(students, student_count);
                    printf("Sort completed, check the result via option 2\n");
                } else {
                    printf("Please input students data first.\n");
                }
                break;
            default:
                break;
        }
    } while (true);

    return 0;
}

void input_student_data(Student *students, int n) {
    printf("\nPlease enter student's information: \n");
    for (int i = 0; i < n; i++) {
        printf("Student %d Name: ", i + 1);
        scanf("%s", students[i].name);

        printf("Student %d Score: ", i + 1);
        scanf("%f", &students[i].score);
    }
}

void display_student_data(const Student *students, int n) {
    printf("\n----- Student Records -----\n");
    for (int i = 0; i < n; i++) {
        printf("Student %s, scores: %.2f\n", students[i].name, students[i].score);
    }
    printf("--------------------------\n");
}

float calculate_average(const Student *students, int n) {
    float sum = 0;
    for (int i = 0; i < n; i++) {
        sum += students[i].score;
    }
    return sum / n;
}

const Student* find_highest_score(const Student *students, int n) {
    const Student *highest = &students[0];
    for (int i = 0; i < n; i++) {
        if (highest -> score < students[i].score) {
            highest = &students[i];
        }
    }
    return highest;
}

const Student* find_lowest_score(const Student *students, int n) {
    const Student *lowest = &students[0];
    for (int i = 0; i < n; i++) {
        if (lowest -> score > students[i].score) {
            lowest = &students[i];
        }
    }
    return lowest;
}

void sort_by_score(Student *students, int n) {
    // bubble sort
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (students[j].score < students[j + 1].score) {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
}
