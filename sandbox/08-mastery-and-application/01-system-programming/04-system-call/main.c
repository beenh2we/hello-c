#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

// Function to demonstrate file-related system calls
void demonstrate_file_syscalls(const char *filename)
{
    printf("\n=== File I/O System Calls ===\n");

    int fd;
    ssize_t bytes_read, bytes_written;
    off_t offset;
    char buffer[100];
    struct stat file_info;

    // open() system call - creates or opens files
    printf("Opening file '%s' for writing...\n", filename);
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        // Error handling pattern: check return value and use errno
        perror("open failed");
        return;
    }
    printf("File opened successfully with descriptor: %d\n", fd);

    // write() system call - writes data to file descriptor
    const char *message = "Hello, this is a system call demonstration!\n";
    bytes_written = write(fd, message, strlen(message));
    if (bytes_written == -1)
    {
        perror("write failed");
        close(fd);  // Always clean up resources even on error
        return;
    }
    printf("Wrote %zd bytes to the file\n", bytes_written);

    // close() system call - closes a file descriptor
    if (close(fd) == -1)
    {
        perror("close failed");
        return;
    }
    printf("File closed successfully\n");

    // open the file again for reading
    printf("\nRe-opening file for reading...\n");
    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("open for reading failed");
        return;
    }

    // read() system call - reads data from file descriptor
    bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1)
    {
        perror("read failed");
        close(fd);
        return;
    }
    buffer[bytes_read] = '\0';  // Null-terminate the string
    printf("Read %zd bytes: %s", bytes_read, buffer);

    // lseek() system call - repositions read/write offset
    printf("\nUsing lseek() to move to the beginning of the file...\n");
    offset = lseek(fd, 0, SEEK_SET);
    if (offset == -1)
    {
        perror("lseek failed");
        close(fd);
        return;
    }

    // Read from the new position
    bytes_read = read(fd, buffer, 5);  // Read just 5 bytes
    if (bytes_read == -1)
    {
        perror("read failed");
        close(fd);
        return;
    }
    buffer[bytes_read] = '\0';
    printf("Read %zd bytes after seeking: %s\n", bytes_read, buffer);

    // stat() system call - gets file status
    printf("\nGetting file information with stat()...\n");
    if (stat(filename, &file_info) == -1)
    {
        perror("stat failed");
        close(fd);
        return;
    }

    printf("File size: %lld bytes\n", (long long) file_info.st_size);
    printf("File permissions (octal): %o\n", file_info.st_mode & 0777);
    printf("Last access time: %s", ctime(&file_info.st_atime));

    close(fd);
}

// Function to demonstrate process-related system calls
void demonstrate_process_syscalls()
{
    printf("\n=== Process Control System Calls ===\n");

    pid_t pid;
    int status;

    // fork() system call - creates a new process
    printf("Calling fork() to create a child process...\n");
    pid = fork();

    if (pid == -1)
    {
        perror("fork failed");
        return;
    }

    if (pid == 0)
    {
        // Child process
        printf("Child process: PID = %d, Parent PID = %d\n", getpid(), getppid());

        // exec() family - replaces the current process image
        // Uncomment to see exec in action:
        /*
        printf("Child will now execute 'ls -l'...\n");
        execl("/bin/ls", "ls", "-l", NULL);
        perror("execl failed");  // Only reached if exec fails
        exit(EXIT_FAILURE);
        */

        // Just sleep and exit for demonstration
        sleep(1);
        printf("Child process exiting with status 42\n");
        exit(42);
    }
    else
    {
        // Parent process
        printf("Parent process: PID = %d, Child PID = %d\n", getpid(), pid);

        // wait() system call - waits for child to terminate
        printf("Parent waiting for child to terminate...\n");
        if (wait(&status) == -1)
        {
            perror("wait failed");
            return;
        }

        if (WIFEXITED(status))
        {
            printf("Child exited with status: %d\n", WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("Child terminated by signal: %d\n", WTERMSIG(status));
        }
    }
}

// Function to demonstrate time-related system calls
void demonstrate_time_syscalls()
{
    printf("\n=== Time-Related System Calls ===\n");

    time_t raw_time;
    struct tm *time_info;
    struct timeval tv;

    // time() system call - seconds since the epoch
    raw_time = time(NULL);
    if (raw_time == (time_t) -1)
    {
        perror("time failed");
        return;
    }

    printf("Current time (seconds since epoch): %ld\n", (long) raw_time);

    // Convert to human-readable format (not a system call, but commonly used)
    time_info = localtime(&raw_time);
    if (time_info == NULL)
    {
        perror("localtime failed");
        return;
    }

    char time_str[50];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", time_info);
    printf("Local time: %s\n", time_str);

    // gettimeofday() system call - microsecond precision
    if (gettimeofday(&tv, NULL) == -1)
    {
        perror("gettimeofday failed");
        return;
    }

    printf("Time with microsecond precision:\n");
    printf("  Seconds: %ld\n", (long) tv.tv_sec);
    printf("  Microseconds: %ld\n", (long) tv.tv_usec);
}

// Error handling with resource cleanup example
void demonstrate_error_handling()
{
    printf("\n=== Error Handling for System Calls ===\n");

    int fd = -1;
    char *buffer = NULL;

    // Try to allocate memory
    buffer = malloc(1024);
    if (buffer == NULL)
    {
        perror("malloc failed");
        goto cleanup;  // Use goto for consistent cleanup
    }

    // Try to open a file in a restricted location (should fail)
    fd = open("/root/restricted_file.txt", O_RDONLY);
    if (fd == -1)
    {
        // Check specific error codes
        if (errno == EACCES)
        {
            printf("Permission denied: cannot open restricted file\n");
        }
        else if (errno == ENOENT)
        {
            printf("File does not exist\n");
        }
        else
        {
            printf("Error opening file: %s (errno = %d)\n", strerror(errno), errno);
        }
        goto cleanup;
    }

cleanup:
    printf("\nCleaning up resources...\n");

    if (fd != -1)
    {
        close(fd);
        printf("Closed file descriptor\n");
    }

    if (buffer != NULL)
    {
        free(buffer);
        printf("Freed memory buffer\n");
    }

    printf("Cleanup complete\n");
}

// Main function
int main()
{
    printf("===== System Call Demonstration Program =====\n");

    // Run all demonstrations
    demonstrate_file_syscalls("syscall_test.txt");
    demonstrate_process_syscalls();
    demonstrate_time_syscalls();
    demonstrate_error_handling();

    printf("\nAll demonstrations completed!\n");

    // Clean up the test file
    if (unlink("syscall_test.txt") == -1)
    {
        perror("unlink failed");
    }
    else
    {
        printf("\nTest file removed\n");
    }

    return 0;
}
