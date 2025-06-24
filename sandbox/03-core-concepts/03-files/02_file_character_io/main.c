#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    int c;

    fp = fopen("char_example.txt", "w");
    if (fp == NULL)
    {
        perror("Error opening file for writing. ");
        return 1;
    }

    fputc('H', fp);
    fputc('e', fp);
    fputc('l', fp);
    fputc('l', fp);
    fputc('o', fp);
    fputc('\n', fp);

    fclose(fp);

    fp = fopen("char_example.txt", "r");
    if (fp == NULL)
    {
        perror("Error opening file for reading. ");
        return 1;
    }

    printf("File contents: ");
    while ((c = fgetc(fp)) != EOF)
    {
        putchar(c);  // print to console
    }

    fclose(fp);
    return 0;
}
