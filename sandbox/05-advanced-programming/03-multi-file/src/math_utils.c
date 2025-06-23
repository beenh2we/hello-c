#include "math_utils.h"

/* Implementation of functions declared in math_utils.h */
double square(double x) {
    return x * x;
}

double cube(double x) {
    return x * x * x;
}

double power(double x, int y) {
    if (y == 0) return 1.0;
    
    double result = 1.0;
    int abs_y = y < 0 ? -y : y;
    
    for (int i = 0; i < abs_y; i++) {
        result *= x;
    }
    
    return y < 0 ? 1.0 / result : result;
}