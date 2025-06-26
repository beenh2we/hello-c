#include <stdio.h>
#include <string.h>

// Structure definition using struct keyword
struct Subject
{
    char name[20];
};

// Structure definition using typedef
typedef struct
{
    char title[100];
    char author[50];
    int year;
    float price;
} Book;

int main()
{
    // Initialize at declaration
    struct Subject math = {"Math"};

    // Initialize after declaration
    struct Subject english;
    strcpy(english.name, "English");

    printf("Subject 1: %s\n", math.name);
    printf("Subject 2: %s\n", english.name);

    // Using the typedef version
    Book myBook = {"C Programming", "Dennis Ritchie", 1978, 29.99};

    printf("Book: \"%s\" by %s, $%.2f\n",
           myBook.title,
           myBook.author,
           myBook.price);

    return 0;
}
