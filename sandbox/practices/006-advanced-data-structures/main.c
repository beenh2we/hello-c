#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// === TYPEDEFS AND ENUMERATIONS ===

// Define a status code enumeration
typedef enum {
    STATUS_SUCCESS = 0,
    STATUS_ERROR_FILE_NOT_FOUND = -1,
    STATUS_ERROR_OUT_OF_MEMORY = -2,
    STATUS_ERROR_INVALID_INPUT = -3
} StatusCode;

// Define a color type using bit fields
typedef struct {
    unsigned int blue  : 8;
    unsigned int green : 8;
    unsigned int red   : 8;
    unsigned int alpha : 8;
} Color;

// Define a point in 2D space
typedef struct {
    float x;
    float y;
} Point;

// Define different shape types
typedef enum {
    SHAPE_CIRCLE,
    SHAPE_RECTANGLE,
    SHAPE_TRIANGLE
} ShapeType;

// Define a generic shape using a union
typedef struct {
    ShapeType type;
    Color color;
    union {
        struct {
            Point center;
            float radius;
        } circle;
        
        struct {
            Point top_left;
            Point bottom_right;
        } rectangle;
        
        struct {
            Point points[3];
        } triangle;
    } data;
} Shape;

// Function pointer typedefs
typedef float (*AreaCalculator)(const Shape*);
typedef void (*ShapeRenderer)(const Shape*);

// === FUNCTION PROTOTYPES ===

// Create different shapes
Shape* create_circle(float x, float y, float radius, Color color);
Shape* create_rectangle(float x1, float y1, float x2, float y2, Color color);
Shape* create_triangle(Point points[3], Color color);

// Calculate areas
float calculate_circle_area(const Shape *shape);
float calculate_rectangle_area(const Shape *shape);
float calculate_triangle_area(const Shape *shape);
float calculate_shape_area(const Shape *shape);

// Render shapes (just prints description in this example)
void render_circle(const Shape *shape);
void render_rectangle(const Shape *shape);
void render_triangle(const Shape *shape);
void render_shape(const Shape *shape);

// Utility functions
Color create_color(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha);
void print_status(StatusCode status);
const char* get_shape_name(ShapeType type);

// === FUNCTION IMPLEMENTATIONS ===

Shape* create_circle(float x, float y, float radius, Color color) {
    Shape *shape = (Shape*)malloc(sizeof(Shape));
    if (shape == NULL) {
        return NULL;
    }
    
    shape->type = SHAPE_CIRCLE;
    shape->color = color;
    shape->data.circle.center.x = x;
    shape->data.circle.center.y = y;
    shape->data.circle.radius = radius;
    
    return shape;
}

Shape* create_rectangle(float x1, float y1, float x2, float y2, Color color) {
    Shape *shape = (Shape*)malloc(sizeof(Shape));
    if (shape == NULL) {
        return NULL;
    }
    
    shape->type = SHAPE_RECTANGLE;
    shape->color = color;
    shape->data.rectangle.top_left.x = x1;
    shape->data.rectangle.top_left.y = y1;
    shape->data.rectangle.bottom_right.x = x2;
    shape->data.rectangle.bottom_right.y = y2;
    
    return shape;
}

Shape* create_triangle(Point points[3], Color color) {
    Shape *shape = (Shape*)malloc(sizeof(Shape));
    if (shape == NULL) {
        return NULL;
    }
    
    shape->type = SHAPE_TRIANGLE;
    shape->color = color;
    
    for (int i = 0; i < 3; i++) {
        shape->data.triangle.points[i] = points[i];
    }
    
    return shape;
}

float calculate_circle_area(const Shape *shape) {
    if (shape->type != SHAPE_CIRCLE) return 0.0f;
    
    float radius = shape->data.circle.radius;
    return 3.14159f * radius * radius;
}

float calculate_rectangle_area(const Shape *shape) {
    if (shape->type != SHAPE_RECTANGLE) return 0.0f;
    
    float width = shape->data.rectangle.bottom_right.x - shape->data.rectangle.top_left.x;
    float height = shape->data.rectangle.bottom_right.y - shape->data.rectangle.top_left.y;
    
    return width * height;
}

float calculate_triangle_area(const Shape *shape) {
    if (shape->type != SHAPE_TRIANGLE) return 0.0f;
    
    // Using Heron's formula
    Point *p = shape->data.triangle.points;
    
    // Calculate side lengths
    float a = sqrtf((p[1].x - p[0].x) * (p[1].x - p[0].x) + 
                    (p[1].y - p[0].y) * (p[1].y - p[0].y));
    float b = sqrtf((p[2].x - p[1].x) * (p[2].x - p[1].x) + 
                    (p[2].y - p[1].y) * (p[2].y - p[1].y));
    float c = sqrtf((p[0].x - p[2].x) * (p[0].x - p[2].x) + 
                    (p[0].y - p[2].y) * (p[0].y - p[2].y));
    
    // Semi-perimeter
    float s = (a + b + c) / 2.0f;
    
    // Area
    return sqrtf(s * (s - a) * (s - b) * (s - c));
}

float calculate_shape_area(const Shape *shape) {
    switch(shape->type) {
        case SHAPE_CIRCLE:
            return calculate_circle_area(shape);
        case SHAPE_RECTANGLE:
            return calculate_rectangle_area(shape);
        case SHAPE_TRIANGLE:
            return calculate_triangle_area(shape);
        default:
            return 0.0f;
    }
}

void render_circle(const Shape *shape) {
    printf("Circle at (%.1f, %.1f) with radius %.1f\n", 
           shape->data.circle.center.x,
           shape->data.circle.center.y,
           shape->data.circle.radius);
    printf("  Color: RGBA(%u, %u, %u, %u)\n",
           shape->color.red, shape->color.green,
           shape->color.blue, shape->color.alpha);
}

void render_rectangle(const Shape *shape) {
    printf("Rectangle from (%.1f, %.1f) to (%.1f, %.1f)\n", 
           shape->data.rectangle.top_left.x,
           shape->data.rectangle.top_left.y,
           shape->data.rectangle.bottom_right.x,
           shape->data.rectangle.bottom_right.y);
    printf("  Color: RGBA(%u, %u, %u, %u)\n",
           shape->color.red, shape->color.green,
           shape->color.blue, shape->color.alpha);
}

void render_triangle(const Shape *shape) {
    printf("Triangle with points:\n");
    for (int i = 0; i < 3; i++) {
        printf("  Point %d: (%.1f, %.1f)\n", i + 1,
               shape->data.triangle.points[i].x,
               shape->data.triangle.points[i].y);
    }
    printf("  Color: RGBA(%u, %u, %u, %u)\n",
           shape->color.red, shape->color.green,
           shape->color.blue, shape->color.alpha);
}

void render_shape(const Shape *shape) {
    if (shape == NULL) return;
    
    printf("Rendering %s...\n", get_shape_name(shape->type));
    
    switch(shape->type) {
        case SHAPE_CIRCLE:
            render_circle(shape);
            break;
        case SHAPE_RECTANGLE:
            render_rectangle(shape);
            break;
        case SHAPE_TRIANGLE:
            render_triangle(shape);
            break;
        default:
            printf("Unknown shape type\n");
    }
    
    printf("  Area: %.2f square units\n\n", calculate_shape_area(shape));
}

Color create_color(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha) {
    Color color;
    color.red = red;
    color.green = green;
    color.blue = blue;
    color.alpha = alpha;
    return color;
}

void print_status(StatusCode status) {
    switch(status) {
        case STATUS_SUCCESS:
            printf("Status: Success\n");
            break;
        case STATUS_ERROR_FILE_NOT_FOUND:
            printf("Status: Error - File not found\n");
            break;
        case STATUS_ERROR_OUT_OF_MEMORY:
            printf("Status: Error - Out of memory\n");
            break;
        case STATUS_ERROR_INVALID_INPUT:
            printf("Status: Error - Invalid input\n");
            break;
        default:
            printf("Status: Unknown error code %d\n", status);
    }
}

const char* get_shape_name(ShapeType type) {
    switch(type) {
        case SHAPE_CIRCLE: return "Circle";
        case SHAPE_RECTANGLE: return "Rectangle";
        case SHAPE_TRIANGLE: return "Triangle";
        default: return "Unknown Shape";
    }
}

int main() {
    printf("==== ADVANCED DATA STRUCTURES DEMO ====\n\n");
    
    // Create some colors using bit fields
    Color red = create_color(255, 0, 0, 255);
    Color green = create_color(0, 255, 0, 255);
    Color blue = create_color(0, 0, 255, 255);
    
    // Create an array of shapes
    Shape *shapes[3];
    StatusCode status = STATUS_SUCCESS;
    
    // Create a circle
    shapes[0] = create_circle(0.0f, 0.0f, 5.0f, red);
    if (shapes[0] == NULL) {
        status = STATUS_ERROR_OUT_OF_MEMORY;
        goto cleanup;
    }
    
    // Create a rectangle
    shapes[1] = create_rectangle(0.0f, 0.0f, 10.0f, 5.0f, green);
    if (shapes[1] == NULL) {
        status = STATUS_ERROR_OUT_OF_MEMORY;
        goto cleanup;
    }
    
    // Create a triangle
    Point triangle_points[3] = {
        {0.0f, 0.0f},
        {5.0f, 10.0f},
        {10.0f, 0.0f}
    };
    shapes[2] = create_triangle(triangle_points, blue);
    if (shapes[2] == NULL) {
        status = STATUS_ERROR_OUT_OF_MEMORY;
        goto cleanup;
    }
    
    // Create an array of function pointers for area calculation
    AreaCalculator area_calculators[3] = {
        calculate_circle_area,
        calculate_rectangle_area,
        calculate_triangle_area
    };
    
    // Render all shapes
    for (int i = 0; i < 3; i++) {
        render_shape(shapes[i]);
    }
    
    // Use function pointers to calculate areas
    printf("Using function pointers for area calculations:\n");
    for (int i = 0; i < 3; i++) {
        float area = area_calculators[i](shapes[i]);
        printf("%s area: %.2f square units\n", get_shape_name(shapes[i]->type), area);
    }
    
cleanup:
    // Display status
    print_status(status);
    
    // Free resources
    for (int i = 0; i < 3; i++) {
        if (shapes[i] != NULL) {
            free(shapes[i]);
            shapes[i] = NULL;
        }
    }
    
    // Print memory usage information
    printf("\nMemory Usage Information:\n");
    printf("Size of Color (bit fields): %zu bytes\n", sizeof(Color));
    printf("Size of Point: %zu bytes\n", sizeof(Point));
    printf("Size of Shape: %zu bytes\n", sizeof(Shape));
    
    printf("\n=== Size of Each Shape Union Member ===\n");
    printf("Circle struct: %zu bytes\n", sizeof(((Shape*)0)->data.circle));
    printf("Rectangle struct: %zu bytes\n", sizeof(((Shape*)0)->data.rectangle));
    printf("Triangle struct: %zu bytes\n", sizeof(((Shape*)0)->data.triangle));
    
    return (int)status;
}