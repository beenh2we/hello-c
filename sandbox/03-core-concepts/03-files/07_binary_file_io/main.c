#include <stdio.h>

int main(int argc, char *argv[]) {
    FILE *fp;
    int numbers[5] = {1, 2, 3, 4, 5};
    int read_numbers[5];

    fp = fopen("binary_data.bin", "wb");  // Note the "b" for binary mode
    if (fp == NULL) {
        perror("Error opening file for writing ");
        return 1;
    }

    size_t written = fwrite(numbers, sizeof(int), 5, fp);
    printf("Elements written: %zu\n", written);

    fclose(fp);

    fp = fopen("binary_data.bin", "rb");
    if (fp == NULL) {
        perror("Error opening file for reading ");
        return 1;
    }

    size_t read = fread(read_numbers, sizeof(int), 5, fp);
    printf("Elements read: %zu\n", read);

    for (int i = 0; i < 5; i++) {
        printf("Element %d: %d\n", i, read_numbers[i]);
    }

    fclose(fp);
    return 0;
}