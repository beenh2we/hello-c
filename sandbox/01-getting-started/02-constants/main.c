#include <stdio.h>

#define PI 3.14159265358979323846

int main(int argc, char *argv[]) {
    float radius;

    printf("Please enter the radius of the circle: ");
    if (scanf("%f", &radius) != 1) {
        printf("Invalid input, please enter a number. \n");
        return 1;
    }

    if (radius < 0) {
        printf("Error: Radius must be a positive number.\n");
        return 1;
    }

    const float area = PI * radius * radius;
    printf("- Radius: %.2f units\n", radius);
    printf("- Area (PI * radius * radius): %.2f square units\n", area);
    printf("- Circumference (PI * 2 * radius): %.2f units\n", 2 * PI * radius);

    return 0;
}