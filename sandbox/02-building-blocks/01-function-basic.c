#include <stdio.h>
#include <math.h>

// Function declaration/prototype (Array Decay)
float calculate_average(int *nums, int length);

// Function parameters (pass by value)
void swap(int *a, int *b);

int main(int argc, char *argv[]) {
    printf("\n---------------- Function declaration/definition ----------------\n\n");

    int nums[] = {2, 4, 6, 8, 10};
    const float result = calculate_average(nums, sizeof(nums) / sizeof(nums[0]));
    printf("The average result is %.2f\n", result);

    printf("\n---------------- Function pass by 'reference' ----------------\n\n");

    int a = 99, b = -1;
    printf("Original value of a=%d, b=%d\n", a, b);
    
    swap(&a, &b);

    printf("After swap value of a=%d, b=%d\n", a, b);

    printf("\n---------------- Standard library: math ----------------\n\n");
    // Basic math functions
    printf("Square root of 16: %.2f\n", sqrt(16.0));
    printf("2 raised to power 3: %.2f\n", pow(2.0, 3.0));
    printf("Absolute value of -10: %.2f\n", fabs(-10.0));
    
    // Rounding
    printf("Ceiling of 4.3: %.2f\n", ceil(4.3));  // 5.0
    printf("Floor of 4.8: %.2f\n", floor(4.8));   // 4.0
    printf("Round 4.5: %.2f\n", round(4.5));      // 5.0
    
    // Trigonometry (angles in radians)
    printf("Sin(0): %.2f\n", sin(0));  // 0.00
    printf("Cos(0): %.2f\n", cos(0));  // 1.00

    printf("\n---------------- Standard library: ctype ----------------\n\n");

    char c = 'A';
    
    // Character classification
    printf("Is '%c' a letter? %s\n", c, isalpha(c) ? "Yes" : "No");
    printf("Is '%c' a digit? %s\n", c, isdigit(c) ? "Yes" : "No");
    printf("Is '%c' uppercase? %s\n", c, isupper(c) ? "Yes" : "No");
    printf("Is '%c' lowercase? %s\n", c, islower(c) ? "Yes" : "No");
    
    // Character conversion
    printf("'%c' to lowercase: '%c'\n", c, tolower(c));
    printf("'%c' to uppercase: '%c'\n", 'a', toupper('a'));

    return 0;
}

// Function definition
float calculate_average(int *nums, int length) {
    int sum = 0;
    for (int i = 0; i < length; i++) {
        sum += nums[i];
    }
    
    return sum / length;
}

void swap(int *a, int *b) {
    int temp = *b;
    *b = *a;
    *a = temp;
}