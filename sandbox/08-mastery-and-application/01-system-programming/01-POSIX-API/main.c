#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// Demonstrate POSIX file operations
void posix_file_operations()
{
    // Create a file using open()
    int fd = open("posix_test.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("open");
        return;
    }

    // Write data using write()
    const char *text = "Hello, POSIX file API!\n";
    ssize_t bytes_written = write(fd, text, strlen(text));
    if (bytes_written == -1)
    {
        perror("write");
        close(fd);
        return;
    }

    // Close the file
    if (close(fd) == -1)
    {
        perror("close");
        return;
    }

    // Reopen the file for reading
    fd = open("posix_test.txt", O_RDONLY);
    if (fd == -1)
    {
        perror("open for reading");
        return;
    }

    // Get file information using fstat()
    struct stat file_info;
    if (fstat(fd, &file_info) == -1)
    {
        perror("fstat");
        close(fd);
        return;
    }

    printf("File size: %lld bytes\n", (long long) file_info.st_size);
    printf("File permissions: %o\n", file_info.st_mode & 0777);
    printf("Last modification: %s", ctime(&file_info.st_mtime));

    // Read data using read()
    char buffer[100];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1)
    {
        perror("read");
        close(fd);
        return;
    }

    // Null-terminate the string
    buffer[bytes_read] = '\0';
    printf("Read from file: %s", buffer);

    // Close the file
    close(fd);

    // Rename the file using rename()
    if (rename("posix_test.txt", "posix_test_renamed.txt") == -1)
    {
        perror("rename");
        return;
    }

    printf("File renamed successfully\n");

    // Delete the file using unlink()
    if (unlink("posix_test_renamed.txt") == -1)
    {
        perror("unlink");
        return;
    }

    printf("File deleted successfully\n");
}

// Demonstrate directory operations
void directory_operations()
{
    // Create a directory
    if (mkdir("posix_dir", 0755) == -1)
    {
        if (errno != EEXIST)
        {
            perror("mkdir");
            return;
        }
    }

    // Create a few files in the directory
    int fd;
    char filepath[100];

    for (int i = 1; i <= 3; i++)
    {
        snprintf(filepath, sizeof(filepath), "posix_dir/file%d.txt", i);
        fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
        {
            perror("open");
            continue;
        }

        char content[50];
        snprintf(content, sizeof(content), "This is file %d content\n", i);
        write(fd, content, strlen(content));
        close(fd);
    }

    // Open and read the directory
    DIR *dir = opendir("posix_dir");
    if (dir == NULL)
    {
        perror("opendir");
        return;
    }

    printf("\nListing directory contents:\n");

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        // Skip . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        // Get full path
        snprintf(filepath, sizeof(filepath), "posix_dir/%s", entry->d_name);

        // Get file information
        struct stat info;
        if (stat(filepath, &info) == -1)
        {
            perror("stat");
            continue;
        }

        // Print file information
        printf("%-20s %10lld bytes    %s", entry->d_name, (long long) info.st_size, ctime(&info.st_mtime));
    }

    // Close the directory
    if (closedir(dir) == -1)
    {
        perror("closedir");
        return;
    }

    // Change current working directory
    if (chdir("posix_dir") == -1)
    {
        perror("chdir");
        return;
    }

    // Get current working directory
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd");
        return;
    }

    printf("\nCurrent working directory: %s\n", cwd);

    // Return to parent directory
    if (chdir("..") == -1)
    {
        perror("chdir ..");
        return;
    }
}

// Demonstrate POSIX time functions
void time_functions()
{
    // Get current time using time()
    time_t current_time = time(NULL);
    if (current_time == (time_t) -1)
    {
        perror("time");
        return;
    }

    printf("\nCurrent time: %s", ctime(&current_time));

    // Use strftime() to format time
    struct tm *time_info = localtime(&current_time);
    if (time_info == NULL)
    {
        perror("localtime");
        return;
    }

    char time_str[100];
    if (strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", time_info) == 0)
    {
        fprintf(stderr, "strftime failed\n");
        return;
    }

    printf("Formatted time: %s\n", time_str);

    // Use clock_gettime() to get high-resolution time
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
    {
        perror("clock_gettime");
        return;
    }

    printf("High-resolution time: %ld.%09ld seconds since Epoch\n", ts.tv_sec, ts.tv_nsec);

    // Sleep for a brief period
    printf("Sleeping for 2 seconds...\n");
    sleep(2);

    // Get time again
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
    {
        perror("clock_gettime");
        return;
    }

    printf("After sleep: %ld.%09ld seconds since Epoch\n", ts.tv_sec, ts.tv_nsec);
}

int main()
{
    printf("=== POSIX API Demonstration ===\n\n");

    printf("--- POSIX File Operations ---\n");
    posix_file_operations();

    printf("\n--- Directory Operations ---\n");
    directory_operations();

    printf("\n--- POSIX Time Functions ---\n");
    time_functions();

    // Clean up the directory created for demonstration
    system("rm -rf posix_dir");

    return 0;
}
