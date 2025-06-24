#include <stdio.h>

int main(int argc, char *argv[])
{
    char name[20];
    int age;

    printf("Hi, what is your name: ");
    scanf("%19s", name);

    char c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("How old are you: ");
    scanf("%d", &age);

    printf("Your name is: %s, you are %d years old.\n", name, age);

    return 0;
}
