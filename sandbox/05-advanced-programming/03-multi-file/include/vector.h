#ifndef VECTOR_H
#define VECTOR_H

/* Only include what's necessary for the declarations */
#include <stdbool.h> /* For bool type */

/**
 * 3D vector structure
 */
typedef struct
{
    float x;
    float y;
    float z;
} Vector3;

/**
 * Create a new 3D vector with given components
 */
Vector3 vector_create(float x, float y, float z);

/**
 * Add two vectors
 * @return Result of v1 + v2
 */
Vector3 vector_add(Vector3 v1, Vector3 v2);

/**
 * Subtract v2 from v1
 * @return Result of v1 - v2
 */
Vector3 vector_subtract(Vector3 v1, Vector3 v2);

/**
 * Multiply a vector by a scalar
 * @return Vector with each component multiplied by scalar
 */
Vector3 vector_scale(Vector3 v, float scalar);

/**
 * Calculate dot product of two vectors
 * @return The dot product v1·v2
 */
float vector_dot(Vector3 v1, Vector3 v2);

/**
 * Calculate cross product of two vectors
 * @return The cross product v1×v2
 */
Vector3 vector_cross(Vector3 v1, Vector3 v2);

/**
 * Calculate the magnitude (length) of a vector
 * @return The magnitude of the vector
 */
float vector_magnitude(Vector3 v);

/**
 * Normalize a vector to unit length
 * @return Normalized vector with same direction but magnitude 1
 */
Vector3 vector_normalize(Vector3 v);

/**
 * Check if two vectors are equal (within a small epsilon)
 * @return true if vectors are approximately equal
 */
bool vector_equals(Vector3 v1, Vector3 v2, float epsilon);

#endif /* VECTOR_H */
