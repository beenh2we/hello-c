#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Function to print buffer mode as string
const char *get_buffer_mode_str(int mode)
{
    switch (mode)
    {
    case _IONBF:
        return "Unbuffered";
    case _IOLBF:
        return "Line buffered";
    case _IOFBF:
        return "Fully buffered";
    default:
        return "Unknown";
    }
}

// Function to demonstrate different buffering modes
void demo_buffer_modes()
{
    printf("\n=== Buffer Modes Demonstration ===\n");

    // Create a file stream
    FILE *file = fopen("buffer_test.txt", "w");
    if (!file)
    {
        perror("Failed to open file");
        return;
    }

    // Test unbuffered mode
    printf("Setting file to unbuffered mode...\n");
    if (setvbuf(file, NULL, _IONBF, 0) != 0)
    {
        perror("setvbuf failed");
        fclose(file);
        return;
    }

    fprintf(file, "This is written in unbuffered mode.\n");
    printf("Data written in unbuffered mode (sent immediately to disk)\n");

    // Reset the file
    fclose(file);
    file = fopen("buffer_test.txt", "w");
    if (!file)
    {
        perror("Failed to reopen file");
        return;
    }

    // Test line buffered mode
    printf("\nSetting file to line buffered mode...\n");
    if (setvbuf(file, NULL, _IOLBF, 1024) != 0)
    {
        perror("setvbuf failed");
        fclose(file);
        return;
    }

    fprintf(file, "This is written in line buffered mode.\n");
    fprintf(file,
            "This line doesn't end with a newline, so it stays in buffer");
    printf("Data with newline sent to disk, data without newline buffered\n");

    // Add a newline to flush the buffer
    fprintf(file, "\n");

    // Reset the file
    fclose(file);
    file = fopen("buffer_test.txt", "w");
    if (!file)
    {
        perror("Failed to reopen file");
        return;
    }

    // Test fully buffered mode
    printf("\nSetting file to fully buffered mode...\n");
    if (setvbuf(file, NULL, _IOFBF, 4096) != 0)
    {
        perror("setvbuf failed");
        fclose(file);
        return;
    }

    fprintf(file, "This is written in fully buffered mode.\n");
    fprintf(file,
            "Even with newlines, this stays in buffer until full or flush.\n");
    printf("Data is buffered regardless of newlines\n");

    // Flush the buffer explicitly
    printf("\nFlushing buffer explicitly...\n");
    fflush(file);
    printf("Buffer flushed, data written to disk\n");

    // Clean up
    fclose(file);
}

// Function to demonstrate custom buffer with setvbuf
void demo_custom_buffer()
{
    printf("\n=== Custom Buffer Demonstration ===\n");

    // Create a file stream
    FILE *file = fopen("custom_buffer.txt", "w");
    if (!file)
    {
        perror("Failed to open file");
        return;
    }

    // Create a custom buffer
    const size_t buffer_size = 1024;
    char *custom_buffer = (char *) malloc(buffer_size);
    if (!custom_buffer)
    {
        perror("Failed to allocate buffer");
        fclose(file);
        return;
    }

    printf("Allocated custom buffer of size %zu bytes\n", buffer_size);

    // Set the custom buffer
    if (setvbuf(file, custom_buffer, _IOFBF, buffer_size) != 0)
    {
        perror("setvbuf failed");
        free(custom_buffer);
        fclose(file);
        return;
    }

    printf("File now using custom buffer\n");

    // Write data
    for (int i = 0; i < 10; i++)
    {
        fprintf(file,
                "Line %d: This data is being written to the custom buffer.\n",
                i + 1);
    }

    printf("Data written to buffer\n");

    // Flush half way
    printf("Flushing buffer...\n");
    fflush(file);

    // Write more data
    for (int i = 10; i < 20; i++)
    {
        fprintf(file, "Line %d: More data after flushing.\n", i + 1);
    }

    // Close the file (automatically flushes)
    fclose(file);

    // Free the buffer - IMPORTANT: Only after closing the file!
    free(custom_buffer);

    printf("Custom buffer demonstration completed\n");
}

// Function to demonstrate performance impact of different buffer sizes
void demo_buffer_performance()
{
    printf("\n=== Buffer Performance Demonstration ===\n");

    const int NUM_ITERATIONS = 100000;
    const size_t BUFFER_SIZES[] = {0, 64, 1024, 4096, 16384};
    const int NUM_BUFFER_SIZES = sizeof(BUFFER_SIZES) / sizeof(BUFFER_SIZES[0]);

    printf("Testing write performance with different buffer sizes...\n");
    printf("Writing %d lines for each buffer size\n", NUM_ITERATIONS);

    for (int i = 0; i < NUM_BUFFER_SIZES; i++)
    {
        size_t buffer_size = BUFFER_SIZES[i];
        char buffer_mode = (buffer_size == 0) ? _IONBF : _IOFBF;

        // Create a file with the specified buffer size
        char filename[64];
        snprintf(filename, sizeof(filename), "perf_test_%zu.txt", buffer_size);

        FILE *file = fopen(filename, "w");
        if (!file)
        {
            perror("Failed to open file");
            continue;
        }

        // Set buffer mode and size
        if (setvbuf(file, NULL, buffer_mode, buffer_size) != 0)
        {
            perror("setvbuf failed");
            fclose(file);
            continue;
        }

        // Measure writing performance
        clock_t start = clock();

        for (int j = 0; j < NUM_ITERATIONS; j++)
        {
            fprintf(file, "This is line %d in the performance test.\n", j);
        }

        clock_t end = clock();
        double time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;

        // Close the file
        fclose(file);

        // Get file size
        FILE *file_stat = fopen(filename, "r");
        if (file_stat)
        {
            fseek(file_stat, 0, SEEK_END);
            long file_size = ftell(file_stat);
            fclose(file_stat);

            printf(
                "Buffer size: %6zu bytes | Time: %7.3f sec | File size: %ld bytes\n",
                buffer_size,
                time_taken,
                file_size);
        }
        else
        {
            printf("Buffer size: %6zu bytes | Time: %7.3f sec\n",
                   buffer_size,
                   time_taken);
        }

        // Clean up
        unlink(filename);
    }
}

int main()
{
    printf("=== Stream Buffering Demo ===\n");

    // Run the demos
    demo_buffer_modes();
    demo_custom_buffer();
    demo_buffer_performance();

    // Clean up
    unlink("buffer_test.txt");
    unlink("custom_buffer.txt");

    return 0;
}
