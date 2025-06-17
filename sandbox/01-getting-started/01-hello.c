#include <stdio.h>

#define PI 3.14159265358979323846

/**
 * Basic C Program Structure
 *
 * This program demonstrates:
 * - Including header files
 * - Defining constants
 * - Variable declaration and initialization
 * - Using scanf for basic user input
 * - Using printf for formatted output
 * - Simple mathematical calculations
 */
int main(void) { // void indicates that it does not accept command line arguments
    /* Variable declaration */
    char name[20];
    int age;
    float radius;
    float area;

    /* User interaction and input */
    printf("Hello, what is your name: ");
    scanf("%s", name);

    printf("How old are you: ");
    scanf("%d", &age);

    printf("Hello %s, you are %d years old!\n", name, age);

    /* Circle area calculation */
    printf("Please enter the radius of the circle: ");
    scanf("%f", &radius);

    area = PI * radius * radius;

    printf("The area of the circle (radius=%.2f) is %.2f\n", radius, area);

    return 0;
}