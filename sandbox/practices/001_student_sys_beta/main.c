#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STUDENTS 50
#define MAX_NAME_LEN 30

void input_student_data(char names[][MAX_NAME_LEN], float scores[], int n);
void display_student_data(char names[][MAX_NAME_LEN], float scores[], int n);
float calculate_average(float scores[], int n);
float find_highest_score(float scores[], int n);
float find_lowest_score(float scores[], int n);
void sort_by_score(char names[][MAX_NAME_LEN], float scores[], int n);

int main(int argc, char *argv[]) {

    int choice;
    int student_count;
    char student_names[MAX_STUDENTS][MAX_NAME_LEN];
    float student_scores[MAX_STUDENTS];
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
                "Please enter the students.. How many students? (Should less "
                "then "
                "%d): ",
                MAX_STUDENTS
            );
            scanf("%d", &student_count);

            if (student_count > 0 && student_count < MAX_STUDENTS) {
                input_student_data(
                    student_names,
                    student_scores,
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
                    student_names,
                    student_scores,
                    student_count
                );
            } else {
                printf("Please input students data first.\n");
            }
            break;
        case 3:
            if (data_entered) {
                float average =
                    calculate_average(student_scores, student_count);
                printf("The average score in your class is %.2f\n", average);
            } else {
                printf("Please input students data first.\n");
            }
            break;
        case 4:
            if (data_entered) {
                float highest =
                    find_highest_score(student_scores, student_count);
                float lowest = find_lowest_score(student_scores, student_count);
                printf(
                    "The highest score in your class is %.2f, lowest is: "
                    "%.2f\n",
                    highest,
                    lowest
                );
            } else {
                printf("Please input students data first.\n");
            }
            break;
        case 5:
            if (data_entered) {
                sort_by_score(student_names, student_scores, student_count);
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

void input_student_data(char names[][MAX_NAME_LEN], float scores[], int n) {
    printf("\nPlease enter student's information: \n");
    for (int i = 0; i < n; i++) {
        printf("Student %d Name: ", i + 1);
        scanf("%s", names[i]);

        printf("Student %d Score: ", i + 1);
        scanf("%f", scores + i);
    }
}

void display_student_data(char names[][MAX_NAME_LEN], float scores[], int n) {
    for (int i = 0; i < n; i++) {
        printf("Student %s, scores: %.2f\n", names[i], scores[i]);
    }
}

float calculate_average(float scores[], int n) {
    float sum = 0;
    for (int i = 0; i < n; i++) {
        sum += scores[i];
    }
    return sum / n;
}

float find_highest_score(float socres[], int n) {
    float highest = -1;
    for (int i = 0; i < n; i++) {
        if (highest < socres[i]) {
            highest = socres[i];
        }
    }
    return highest;
}

float find_lowest_score(float scores[], int n) {
    float lowest = 99;
    for (int i = 0; i < n; i++) {
        if (lowest > scores[i]) {
            lowest = scores[i];
        }
    }
    return lowest;
}

void sort_by_score(char names[][MAX_NAME_LEN], float scores[], int n) {
    // bubble sort
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (scores[j] < scores[j + 1]) {
                // switch score
                float temp_score = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = temp_score;

                // switch name
                char temp_name[MAX_NAME_LEN];
                strcpy(temp_name, names[j]);
                strcpy(names[j], names[j + 1]);
                strcpy(names[j + 1], temp_name);
            }
        }
    }
}
