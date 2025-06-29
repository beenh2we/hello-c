#include <stdio.h>
#include <string.h>

// Define some structs for demonstration
typedef struct
{
    char name[50];
    int age;
    float height;
    float weight;
} Person;

typedef struct
{
    int year;
    int month;
    int day;
} Date;

typedef struct
{
    char title[100];
    char author[50];
    int year;
    float rating;
    int pages;
    char genre[20];
    char publisher[50];
    char language[20];
} Book;

typedef struct
{
    Person owner;
    Date purchase_date;
    char model[30];
    char manufacturer[30];
    int year;
    float price;
} Car;

// Basic struct designated initializers
void basic_struct_initializers()
{
    printf("=== Basic Struct Designated Initializers ===\n");

    // Traditional initialization (positional)
    Person person1 = {"John Smith", 30, 1.75, 70.5};
    printf("Traditional initialization:\n");
    printf("  Name: %s, Age: %d, Height: %.2f m, Weight: %.1f kg\n",
           person1.name,
           person1.age,
           person1.height,
           person1.weight);

    // Using designated initializers (order doesn't matter)
    Person person2
        = {.age = 25, .name = "Jane Doe", .weight = 58.5, .height = 1.68};

    printf("\nUsing designated initializers:\n");
    printf("  Name: %s, Age: %d, Height: %.2f m, Weight: %.1f kg\n",
           person2.name,
           person2.age,
           person2.height,
           person2.weight);

    // Mix of designated and positional initializers
    Person person3 = {.name = "Robert", .age = 40, 1.82, 85.0};

    printf("\nMixing designated and positional initializers:\n");
    printf("  Name: %s, Age: %d, Height: %.2f m, Weight: %.1f kg\n",
           person3.name,
           person3.age,
           person3.height,
           person3.weight);
}

// Array designated initializers
void array_designated_initializers()
{
    printf("\n=== Array Designated Initializers ===\n");

    // Traditional initialization
    int scores1[5] = {85, 92, 78, 90, 88};

    printf("Traditional array initialization:\n  ");
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", scores1[i]);
    }
    printf("\n");

    // Using designated initializers
    int scores2[10] = {[0] = 85, [2] = 78, [5] = 92, [7] = 90};

    printf("\nArray with designated initializers:\n  ");
    for (int i = 0; i < 10; i++)
    {
        printf("%d ", scores2[i]);
    }
    printf("\n");

    // Range initialization
    int scores3[20] = {[0 ... 9] = 50, [10 ... 19] = 100};

    printf("\nArray with range initialization (GNU extension):\n  ");
    for (int i = 0; i < 20; i++)
    {
        printf("%d ", scores3[i]);
    }
    printf("\n");

    // Character array (string) with designated initializer
    char message[20] = {[0] = 'H', [1] = 'e', [2] = 'l', [3] = 'l', [4] = 'o'};

    printf("\nCharacter array with designated initializers:\n  %s\n", message);
}

// Nested designated initializers
void nested_designated_initializers()
{
    printf("\n=== Nested Designated Initializers ===\n");

    // Car with nested structures
    Car my_car
        = {.owner
           = {.name = "John Smith", .age = 35, .height = 1.80, .weight = 75.0},
           .purchase_date = {.year = 2022, .month = 6, .day = 15},
           .model = "Civic",
           .manufacturer = "Honda",
           .year = 2021,
           .price = 25000.00};

    printf("Car details using nested designated initializers:\n");
    printf(
        "  Car: %s %s (%d)\n", my_car.manufacturer, my_car.model, my_car.year);
    printf("  Owner: %s, %d years old\n", my_car.owner.name, my_car.owner.age);
    printf("  Purchased on: %d-%02d-%02d\n",
           my_car.purchase_date.year,
           my_car.purchase_date.month,
           my_car.purchase_date.day);
    printf("  Price: $%.2f\n", my_car.price);
}

// Using designated initializers for sparse arrays
void sparse_arrays()
{
    printf("\n=== Designated Initializers for Sparse Arrays ===\n");

    // Create a sparse array with specific elements initialized
    int sparse[100] = {[0] = 10, [10] = 20, [20] = 30, [50] = 40, [99] = 50};

    printf("Sparse array with 100 elements, only 5 initialized:\n  ");

    // Print only the initialized elements for brevity
    printf("[0]=%d, [10]=%d, [20]=%d, [50]=%d, [99]=%d\n",
           sparse[0],
           sparse[10],
           sparse[20],
           sparse[50],
           sparse[99]);

    // Count non-zero elements to verify sparsity
    int nonzero_count = 0;
    for (int i = 0; i < 100; i++)
    {
        if (sparse[i] != 0) nonzero_count++;
    }

    printf("  Array has %d non-zero elements out of 100\n", nonzero_count);
}

// Partial initialization with defaults
void partial_initialization()
{
    printf("\n=== Partial Initialization with Defaults ===\n");

    // Creating a Book with only some fields specified
    Book book = {
        .title = "The C Programming Language",
        .author = "Brian W. Kernighan and Dennis M. Ritchie",
        .year = 1988,
        .rating = 4.9
        // Other fields will be set to 0 or empty string
    };

    printf("Partially initialized Book struct:\n");
    printf("  Title: %s\n", book.title);
    printf("  Author: %s\n", book.author);
    printf("  Year: %d\n", book.year);
    printf("  Rating: %.1f\n", book.rating);
    printf("  Pages: %d (default value)\n", book.pages);
    printf("  Genre: '%s' (default value)\n", book.genre);

    // Now let's set some of the uninitialized fields
    strcpy(book.genre, "Programming");
    book.pages = 272;
    strcpy(book.publisher, "Prentice Hall");
    strcpy(book.language, "English");

    printf("\nAfter setting the remaining fields:\n");
    printf("  Genre: %s\n", book.genre);
    printf("  Pages: %d\n", book.pages);
    printf("  Publisher: %s\n", book.publisher);
    printf("  Language: %s\n", book.language);
}

// Updating specific values
void updating_specific_values()
{
    printf("\n=== Updating Specific Values ===\n");

    // Initialize an array
    int values[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    printf("Original array:\n  ");
    for (int i = 0; i < 10; i++)
    {
        printf("%d ", values[i]);
    }
    printf("\n");

    // Update specific elements using compound literal with designated initializers
    int *ptr = (int[10]) {
        [0] = values[0],
        [1] = values[1],
        [2] = 30,  // Changed
        [3] = 40,  // Changed
        [4] = values[4],
        [5] = values[5],
        [6] = 70,  // Changed
        [7] = values[7],
        [8] = values[8],
        [9] = 100  // Changed
    };

    // Copy back to original array
    for (int i = 0; i < 10; i++)
    {
        values[i] = ptr[i];
    }

    printf("\nArray after updating specific elements:\n  ");
    for (int i = 0; i < 10; i++)
    {
        printf("%d ", values[i]);
    }
    printf("\n");
}

// Best practices for designated initializers
void designated_initializer_best_practices()
{
    printf("\n=== Best Practices for Designated Initializers ===\n");

    printf("1. Use designated initializers for struct clarity\n");
    printf("2. Consider designated initializers for sparse arrays\n");
    printf("3. Format complex initializers for readability\n");
    printf("4. Be consistent with your initialization style\n");
    printf("5. Use designated initializers to make code maintainable\n");
    printf(
        "6. Remember that unspecified elements get default values (0, NULL, etc.)\n");
}

int main()
{
    printf("==== DESIGNATED INITIALIZERS ====\n\n");

    basic_struct_initializers();
    array_designated_initializers();
    nested_designated_initializers();
    sparse_arrays();
    partial_initialization();
    updating_specific_values();
    designated_initializer_best_practices();

    return 0;
}
