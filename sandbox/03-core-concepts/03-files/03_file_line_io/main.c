#include <stdio.h>

int main(int argc, char *argv[]) {
    FILE *fp;
    char buffer[100];

    fp = fopen("line_example.txt", "w");
    if (fp == NULL) {
        perror("Error opening file for writing ");
        return 1;
    }

    fputs("First line of text.\n", fp);
    fputs("Second line of text.\n", fp);
    fputs("Third line of text.\n", fp);

    fclose(fp);

    fp = fopen("line_example.txt", "r");
    if (fp == NULL) {
        perror("Error opening file for reading ");
        return 1;
    }

    printf("File contents: \n");
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }

    fclose(fp);
    
    return 0;
}