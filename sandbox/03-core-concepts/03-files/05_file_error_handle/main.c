#include <stdio.h>

int main()
{
    FILE *fp = fopen("nonexistent.txt", "r");

    if (fp == NULL)
    {
        perror("File opening failed");
        return 1;
    }

    int c = fgetc(fp);

    if (feof(fp))
    {
        printf("End of file reached.\n");
    }

    if (ferror(fp))
    {
        printf("An error occurred during reading.\n");
        clearerr(fp);
    }

    fclose(fp);
    return 0;
}
