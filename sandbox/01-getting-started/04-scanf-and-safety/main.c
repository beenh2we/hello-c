#include <stdio.h>

int main(int argc, char *argv[])
{
    char name[20];
    int age;

    // Case 1: Buffer overflow
    // Case 2: White Space
    printf("Hi, what is your name: ");
    // scanf("%s", name);
    // Case 3: With length (Remaining buffer still read by next scanf)
    int input_nums = scanf("%19s", name);

    printf("How old are you: ");
    scanf("%d", &age);

    printf("Your name is: %s, you are %d years old.\n", name, age);

    printf("Input Nums: %d\n", input_nums);

    // char c;
    // while ((c = getchar()) != '\n' && c != EOF);

    return 0;
}
