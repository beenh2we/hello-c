#include <stdio.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    bool sys_running = true;
    int choice;
    float num1, num2, result;
    char buffer[100];
    char c;

    do {
        printf("\n┌─────────── Calculator ───────────┐\n");
        printf("│                                  │\n");
        printf("│  1. Addition    (a + b)          │\n");
        printf("│  2. Subtraction (a - b)          │\n");
        printf("│  3. Multiplication (a x b)       │\n");
        printf("│  4. Division    (a ÷ b)          │\n");
        printf("│  0. Exit                         │\n");
        printf("│                                  │\n");
        printf("└──────────────────────────────────┘\n");
        printf("\nChoose operation (0-4): ");

        if (scanf("%d", &choice) != 1) {
            printf("Error: Invalid input. Please enter a number.\n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        if (choice < 0 || choice > 4) {
            printf("Error: Please choose a valid option (0-4).\n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        if (choice == 0) {
            sys_running = false;
            continue;
        }

        printf("\nEnter first number: ");
        if (scanf("%f", &num1) != 1) {
            printf("Error: Invalid input. Please enter a valid number.\n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        
        printf("Enter second number: ");
        if (scanf("%f", &num2) != 1) {
            printf("Error: Invalid input. Please enter a valid number.\n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        switch (choice) {
            case 0:
                sys_running = false;
                break;
            case 1:
                result = num1 + num2;
                printf("%.1f + %.1f = %.1f\n", num1, num2, result);
                break;
            case 2:
                result = num1 - num2;
                printf("%.1f - %.1f = %.1f\n", num1, num2, result);
                break;
            case 3:
                result = num1 * num2;
                printf("%.1f * %.1f = %.1f\n", num1, num2, result);
                break;
            case 4:
                if (num2 == 0) {
                    printf("\nError: Cannot divide by zero.\n");
                } else {
                    result = num1 / num2;
                    printf("\nResult: %.2f ÷ %.2f = %.2f\n", num1, num2, result);
                }
                break;
        }

        while ((c = getchar()) != '\n' && c != EOF);
    } while (sys_running);

    printf("\nThank you for using the calculator. Goodbye!\n");
    return 0;
}