#include <stdio.h>

#include "math_utils.h"
#include "vector.h"

int main()
{
    /* Using math utils */
    double value = 3.0;
    printf("Square of %.1f = %.1f\n", value, square(value));
    printf("Cube of %.1f = %.1f\n", value, cube(value));
    printf("%.1f to the power of 4 = %.1f\n", value, power(value, 4));

    /* Using vector library */
    Vector3 v1 = vector_create(1.0, 2.0, 3.0);
    Vector3 v2 = vector_create(4.0, 5.0, 6.0);

    Vector3 sum = vector_add(v1, v2);
    printf("v1 + v2 = (%.1f, %.1f, %.1f)\n", sum.x, sum.y, sum.z);

    float dot = vector_dot(v1, v2);
    printf("Dot product = %.1f\n", dot);

    Vector3 cross = vector_cross(v1, v2);
    printf("Cross product = (%.1f, %.1f, %.1f)\n", cross.x, cross.y, cross.z);

    float mag = vector_magnitude(v1);
    printf("Magnitude of v1 = %.2f\n", mag);

    return 0;
}
