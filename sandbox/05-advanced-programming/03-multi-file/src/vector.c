#include "vector.h"
#include <math.h>  /* for sqrt */

/* Private constant - only visible in this file */
static const float EPSILON = 0.00001f;

/* Private helper function - not exposed in header */
static bool is_zero(float value) {
    return fabsf(value) < EPSILON;
}

/* Implementation of functions declared in vector.h */
Vector3 vector_create(float x, float y, float z) {
    Vector3 v = {x, y, z};
    return v;
}

Vector3 vector_add(Vector3 v1, Vector3 v2) {
    return vector_create(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3 vector_subtract(Vector3 v1, Vector3 v2) {
    return vector_create(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vector3 vector_scale(Vector3 v, float scalar) {
    return vector_create(v.x * scalar, v.y * scalar, v.z * scalar);
}

float vector_dot(Vector3 v1, Vector3 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3 vector_cross(Vector3 v1, Vector3 v2) {
    return vector_create(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}

float vector_magnitude(Vector3 v) {
    return sqrtf(vector_dot(v, v));
}

Vector3 vector_normalize(Vector3 v) {
    float mag = vector_magnitude(v);
    if (is_zero(mag)) return vector_create(0, 0, 0);
    
    return vector_scale(v, 1.0f / mag);
}

bool vector_equals(Vector3 v1, Vector3 v2, float epsilon) {
    if (epsilon <= 0) epsilon = EPSILON;
    
    return fabsf(v1.x - v2.x) < epsilon &&
           fabsf(v1.y - v2.y) < epsilon &&
           fabsf(v1.z - v2.z) < epsilon;
}