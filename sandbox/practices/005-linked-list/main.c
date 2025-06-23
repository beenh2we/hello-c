/* generic_linked_list.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Generic linked list node structure
typedef struct Node {
    void *data;            // Pointer to the actual data
    size_t data_size;      // Size of the data in bytes
    struct Node *next;     // Pointer to the next node
} Node;

// Linked list structure
typedef struct {
    Node *head;            // Pointer to the first node
    int size;              // Number of elements in the list
    
    // Function pointer for printing data
    void (*print_data)(void *data);
    
    // Function pointer for comparing data (returns 0 if equal)
    int (*compare_data)(void *a, void *b);
} LinkedList;

// Function to create a new linked list
LinkedList* create_list(void (*print_func)(void *), 
                        int (*compare_func)(void *, void *)) {
    LinkedList *list = (LinkedList*)malloc(sizeof(LinkedList));
    
    if (list == NULL) {
        printf("Failed to allocate memory for list\n");
        return NULL;
    }
    
    list->head = NULL;
    list->size = 0;
    list->print_data = print_func;
    list->compare_data = compare_func;
    
    return list;
}

// Function to create a new node with given data
Node* create_node(void *data, size_t data_size) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    
    if (new_node == NULL) {
        printf("Failed to allocate memory for node\n");
        return NULL;
    }
    
    // Allocate memory for the data and copy it
    new_node->data = malloc(data_size);
    
    if (new_node->data == NULL) {
        printf("Failed to allocate memory for node data\n");
        free(new_node);
        return NULL;
    }
    
    // Copy the data
    memcpy(new_node->data, data, data_size);
    new_node->data_size = data_size;
    new_node->next = NULL;
    
    return new_node;
}

// Function to add an element at the beginning of the list
void prepend(LinkedList *list, void *data, size_t data_size) {
    if (list == NULL) return;
    
    Node *new_node = create_node(data, data_size);
    
    if (new_node == NULL) return;
    
    new_node->next = list->head;
    list->head = new_node;
    list->size++;
}

// Function to add an element at the end of the list
void append(LinkedList *list, void *data, size_t data_size) {
    if (list == NULL) return;
    
    Node *new_node = create_node(data, data_size);
    
    if (new_node == NULL) return;
    
    if (list->head == NULL) {
        list->head = new_node;
    } else {
        Node *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    
    list->size++;
}

// Function to find an element in the list
Node* find(LinkedList *list, void *data) {
    if (list == NULL || list->head == NULL || list->compare_data == NULL) return NULL;
    
    Node *current = list->head;
    
    while (current != NULL) {
        if (list->compare_data(current->data, data) == 0) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;  // Not found
}

// Function to remove an element from the list
int remove_element(LinkedList *list, void *data) {
    if (list == NULL || list->head == NULL || list->compare_data == NULL) return 0;
    
    Node *current = list->head;
    Node *prev = NULL;
    
    // Check if the head node matches
    if (list->compare_data(current->data, data) == 0) {
        list->head = current->next;
        free(current->data);
        free(current);
        list->size--;
        return 1;
    }
    
    // Check the rest of the list
    prev = current;
    current = current->next;
    
    while (current != NULL) {
        if (list->compare_data(current->data, data) == 0) {
            prev->next = current->next;
            free(current->data);
            free(current);
            list->size--;
            return 1;
        }
        prev = current;
        current = current->next;
    }
    
    return 0;  // Element not found
}

// Function to print the list
void print_list(LinkedList *list) {
    if (list == NULL || list->head == NULL) {
        printf("List is empty\n");
        return;
    }
    
    Node *current = list->head;
    printf("List (size %d): ", list->size);
    
    while (current != NULL) {
        if (list->print_data != NULL) {
            list->print_data(current->data);
        } else {
            printf("[data]");
        }
        
        if (current->next != NULL) {
            printf(" -> ");
        }
        
        current = current->next;
    }
    
    printf("\n");
}

// Function to free the entire list
void free_list(LinkedList *list) {
    if (list == NULL) return;
    
    Node *current = list->head;
    Node *next;
    
    while (current != NULL) {
        next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    
    free(list);
}

// Print function for integers
void print_int(void *data) {
    printf("%d", *(int*)data);
}

// Compare function for integers
int compare_int(void *a, void *b) {
    return *(int*)a - *(int*)b;
}

// Print function for strings
void print_string(void *data) {
    printf("\"%s\"", (char*)data);
}

// Compare function for strings
int compare_string(void *a, void *b) {
    return strcmp((char*)a, (char*)b);
}

int main() {
    printf("=== Generic Linked List Example ===\n\n");
    
    // 1. Create a list of integers
    printf("--- Integer List Example ---\n");
    LinkedList *int_list = create_list(print_int, compare_int);
    
    int values[] = {10, 20, 30, 40, 50};
    
    for (int i = 0; i < 5; i++) {
        append(int_list, &values[i], sizeof(int));
    }
    
    print_list(int_list);
    
    // Find an element
    int search_value = 30;
    Node *found = find(int_list, &search_value);
    
    if (found != NULL) {
        printf("Found value %d in the list\n", *(int*)found->data);
    } else {
        printf("Value %d not found in the list\n", search_value);
    }
    
    // Remove an element
    int remove_value = 20;
    
    if (remove_element(int_list, &remove_value)) {
        printf("Removed %d from the list\n", remove_value);
    } else {
        printf("Value %d not found in the list\n", remove_value);
    }
    
    print_list(int_list);
    free_list(int_list);
    
    // 2. Create a list of strings
    printf("\n--- String List Example ---\n");
    LinkedList *string_list = create_list(print_string, compare_string);
    
    char *fruits[] = {"Apple", "Banana", "Cherry", "Date", "Elderberry"};
    
    for (int i = 0; i < 5; i++) {
        // Note: +1 for null terminator
        append(string_list, fruits[i], strlen(fruits[i]) + 1);
    }
    
    print_list(string_list);
    
    // Find a string
    char *search_string = "Cherry";
    found = find(string_list, search_string);
    
    if (found != NULL) {
        printf("Found string \"%s\" in the list\n", (char*)found->data);
    } else {
        printf("String \"%s\" not found in the list\n", search_string);
    }
    
    // Remove a string
    char *remove_string = "Banana";
    
    if (remove_element(string_list, remove_string)) {
        printf("Removed \"%s\" from the list\n", remove_string);
    } else {
        printf("String \"%s\" not found in the list\n", remove_string);
    }
    
    print_list(string_list);
    free_list(string_list);
    
    return 0;
}