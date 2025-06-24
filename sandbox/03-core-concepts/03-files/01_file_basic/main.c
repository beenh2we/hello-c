#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    char buffer[100];

    fp = fopen("example.txt", "r");

    // Common file opening modes
    FILE *fp1 = fopen("file.txt", "r");   // Open for reading
    FILE *fp2 = fopen("file.txt", "w");   // Open for writing (creates new file/truncates existing)
    FILE *fp3 = fopen("file.txt", "a");   // Open for appending
    FILE *fp4 = fopen("file.txt", "r+");  // Open for reading and writing (file must exist)
    FILE *fp5 = fopen("file.txt", "w+");  // Open for reading and writing (creates/truncates)
    FILE *fp6 = fopen("file.txt", "a+");  // Open for reading and appending

    if (fp == NULL)
    {
        perror("Error opening file.\n");
        return 1;
    }

    if (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        printf("First line: %s\n", buffer);
    }

    fclose(fp);
    return 0;
}
