#include <ctype.h>
#include <math.h>
#include <stdio.h>

float add(float num1, float num2);
float subtract(float num1, float num2);
float multiply(float num1, float num2);
float divide(float num1, float num2);

int main(int argc, char *argv[])
{
    printf("\n---------------- Functions declaration/definition ----------------\n\n");
    float result, num1 = 98, num2 = 132;

    result = add(num1, num2);
    printf("The result of add(): %.2f\n", result);

    result = subtract(num1, num2);
    printf("The result of subtract(): %.2f\n", result);

    result = multiply(num1, num2);
    printf("The result of multiply(): %.2f\n", result);

    result = divide(num1, num2);
    printf("The result of divide(): %.2f\n", result);

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

float add(float num1, float num2)
{
    return num1 + num2;
}

float subtract(float num1, float num2)
{
    return num1 - num2;
}

float multiply(float num1, float num2)
{
    return num1 * num2;
}

float divide(float num1, float num2)
{
    if (num2 == 0)
    {
        printf("\nError: Cannot divide by zero.\n");
    }
    else
    {
        return num1 / num2;
    }
}
