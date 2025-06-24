#include <stdio.h>

// Structure Padding and Memory Alignment
struct Padded
{
    char a;  // 1 byte
    // 3 bytes padding might be inserted here
    int b;    // 4 bytes
    short c;  // 2 bytes
    // 2 bytes padding might be inserted here
};

// Self-Referential Structures
typedef struct Node
{
    int data;
    struct Node *next;  // Pointer to another Node
} Node;

int main()
{
    printf("Size of char: %zu bytes\n", sizeof(char));
    printf("Size of int: %zu bytes\n", sizeof(int));
    printf("Size of short: %zu bytes\n", sizeof(short));

    printf("Size of Padded: %zu bytes\n", sizeof(struct Padded));
    // Might be 12 bytes instead of 7 due to padding

    return 0;
}
