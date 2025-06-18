#include <stdio.h>
#include <string.h>

// First structure definition
typedef struct {
    char street[50];
    char city[30];
    char state[20];
    char zip[10];
} Address;

// Second structure with nested structures
typedef struct {
    char name[50];
    int age;
    Address homeAddress;
    Address workAddress;
} Person;

int main() {
    Person person;
    
    // Initialize the main structure
    strcpy(person.name, "David Johnson");
    person.age = 32;
    
    // Initialize the first nested structure
    strcpy(person.homeAddress.street, "123 Main St");
    strcpy(person.homeAddress.city, "Boston");
    strcpy(person.homeAddress.state, "MA");
    strcpy(person.homeAddress.zip, "02115");
    
    // Initialize the second nested structure
    strcpy(person.workAddress.street, "456 Corporate Pkwy");
    strcpy(person.workAddress.city, "Boston");
    strcpy(person.workAddress.state, "MA");
    strcpy(person.workAddress.zip, "02210");
    
    // Display person information
    printf("Name: %s\n", person.name);
    printf("Age: %d\n", person.age);
    
    printf("\nHome Address:\n");
    printf("%s\n%s, %s %s\n", 
           person.homeAddress.street,
           person.homeAddress.city,
           person.homeAddress.state,
           person.homeAddress.zip);
    
    printf("\nWork Address:\n");
    printf("%s\n%s, %s %s\n", 
           person.workAddress.street,
           person.workAddress.city,
           person.workAddress.state,
           person.workAddress.zip);
    
    return 0;
}