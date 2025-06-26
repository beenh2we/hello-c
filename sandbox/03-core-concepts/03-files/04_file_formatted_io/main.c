#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    char name[50];
    int age;
    float height;

    fp = fopen("formatted_data.txt", "w");
    if (fp == NULL)
    {
        perror("Error opening file for writing ");
        return 1;
    }

    fprintf(fp, "Name: %s, Age: %d, Height: %.2f\n", "John", 25, 1.75);
    fprintf(fp, "Name: %s, Age: %d, Height: %.2f\n", "Alice", 22, 1.85);

    fclose(fp);

    fp = fopen("formatted_data.txt", "r");
    if (fp == NULL)
    {
        perror("Error opening file for reading ");
        return 1;
    }

    int result;
    while (
        (result = fscanf(
             fp, "Name: %[^,], Age: %d, Height: %f\n", name, &age, &height)) ==
        3)
    {
        printf("Read: %s is %d years old and %.2f meters tall.\n",
               name,
               age,
               height);
    }

    fclose(fp);
    return 0;
}
