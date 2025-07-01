#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Function to make a file descriptor non-blocking
int set_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
    {
        perror("fcntl F_GETFL");
        return -1;
    }

    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        perror("fcntl F_SETFL O_NONBLOCK");
        return -1;
    }

    return 0;
}

// Function to demonstrate non-blocking read
void demo_nonblocking_read()
{
    printf("\n=== Non-blocking Read Demo ===\n");

    // Create a pipe for demonstration
    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return;
    }

    // Make the read end non-blocking
    if (set_nonblocking(pipefd[0]) == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return;
    }

    printf("Reading from a pipe in non-blocking mode.\n");

    // Try to read before writing anything (should fail with EAGAIN)
    char buffer[256];
    ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);

    if (bytes_read < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            printf("No data available yet (EAGAIN/EWOULDBLOCK)\n");
        }
        else
        {
            perror("read");
        }
    }

    // Now write some data to the pipe
    const char *message = "Hello, non-blocking I/O!";
    write(pipefd[1], message, strlen(message));

    // Try reading again - should succeed now
    bytes_read = read(pipefd[0], buffer, sizeof(buffer) - 1);

    if (bytes_read > 0)
    {
        buffer[bytes_read] = '\0';  // Null-terminate
        printf("Read %zd bytes: '%s'\n", bytes_read, buffer);
    }
    else if (bytes_read == 0)
    {
        printf("End of file reached\n");
    }
    else
    {
        perror("read");
    }

    // Clean up
    close(pipefd[0]);
    close(pipefd[1]);
}

// Function to demonstrate select() for I/O multiplexing
void demo_select()
{
    printf("\n=== Select() Demo ===\n");

    // Create two pipes for demonstration
    int pipe1[2], pipe2[2];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
    {
        perror("pipe");
        return;
    }

    printf("Monitoring two pipes with select()\n");

    // Fork a child process to write to the pipes
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);
        return;
    }

    if (pid == 0)
    {
        // Child process - will write to pipes
        close(pipe1[0]);  // Close unused read ends
        close(pipe2[0]);

        sleep(1);  // Wait 1 second before first write

        // Write to pipe1
        const char *msg1 = "Message for pipe 1";
        write(pipe1[1], msg1, strlen(msg1));

        sleep(2);  // Wait 2 seconds before second write

        // Write to pipe2
        const char *msg2 = "Message for pipe 2";
        write(pipe2[1], msg2, strlen(msg2));

        // Clean up and exit
        close(pipe1[1]);
        close(pipe2[1]);
        exit(0);
    }
    else
    {
        // Parent process - will read from pipes
        close(pipe1[1]);  // Close unused write ends
        close(pipe2[1]);

        // Variables for select()
        fd_set readfds;
        struct timeval timeout;
        int max_fd = (pipe1[0] > pipe2[0]) ? pipe1[0] : pipe2[0];
        char buffer[256];

        // Monitor pipes for 5 seconds or until both pipes are read
        bool pipe1_read = false, pipe2_read = false;

        while (!pipe1_read || !pipe2_read)
        {
            // Initialize fd_set
            FD_ZERO(&readfds);
            if (!pipe1_read) FD_SET(pipe1[0], &readfds);
            if (!pipe2_read) FD_SET(pipe2[0], &readfds);

            // Set timeout (2 seconds)
            timeout.tv_sec = 2;
            timeout.tv_usec = 0;

            // Wait for activity
            int activity = select(max_fd + 1, &readfds, NULL, NULL, &timeout);

            if (activity == -1)
            {
                perror("select");
                break;
            }
            else if (activity == 0)
            {
                printf("Timeout reached, no data available\n");
            }
            else
            {
                // Check which pipes are ready
                if (FD_ISSET(pipe1[0], &readfds))
                {
                    ssize_t bytes = read(pipe1[0], buffer, sizeof(buffer) - 1);
                    if (bytes > 0)
                    {
                        buffer[bytes] = '\0';
                        printf("Data from pipe 1: %s\n", buffer);
                        pipe1_read = true;
                    }
                }

                if (FD_ISSET(pipe2[0], &readfds))
                {
                    ssize_t bytes = read(pipe2[0], buffer, sizeof(buffer) - 1);
                    if (bytes > 0)
                    {
                        buffer[bytes] = '\0';
                        printf("Data from pipe 2: %s\n", buffer);
                        pipe2_read = true;
                    }
                }
            }
        }

        // Clean up
        close(pipe1[0]);
        close(pipe2[0]);

        // Wait for child to finish
        wait(NULL);
    }
}

// Function to demonstrate poll() for I/O multiplexing
void demo_poll()
{
    printf("\n=== Poll() Demo ===\n");

    // Create two pipes for demonstration
    int pipe1[2], pipe2[2];

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
    {
        perror("pipe");
        return;
    }

    printf("Monitoring two pipes with poll()\n");

    // Fork a child process to write to the pipes
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        // Child process - will write to pipes
        close(pipe1[0]);  // Close unused read ends
        close(pipe2[0]);

        sleep(1);  // Wait 1 second before first write

        // Write to pipe1
        const char *msg1 = "Poll message for pipe 1";
        write(pipe1[1], msg1, strlen(msg1));

        sleep(2);  // Wait 2 seconds before second write

        // Write to pipe2
        const char *msg2 = "Poll message for pipe 2";
        write(pipe2[1], msg2, strlen(msg2));

        // Clean up and exit
        close(pipe1[1]);
        close(pipe2[1]);
        exit(0);
    }
    else
    {
        // Parent process - will read from pipes
        close(pipe1[1]);  // Close unused write ends
        close(pipe2[1]);

        // Set up pollfd structures
        struct pollfd fds[2];
        fds[0].fd = pipe1[0];
        fds[0].events = POLLIN;
        fds[1].fd = pipe2[0];
        fds[1].events = POLLIN;

        char buffer[256];
        bool pipe1_read = false, pipe2_read = false;

        while (!pipe1_read || !pipe2_read)
        {
            // Wait for activity with a 2-second timeout
            int activity = poll(fds, 2, 2000);

            if (activity == -1)
            {
                perror("poll");
                break;
            }
            else if (activity == 0)
            {
                printf("Poll timeout reached, no data available\n");
            }
            else
            {
                // Check for data on pipe1
                if (!pipe1_read && (fds[0].revents & POLLIN))
                {
                    ssize_t bytes = read(pipe1[0], buffer, sizeof(buffer) - 1);
                    if (bytes > 0)
                    {
                        buffer[bytes] = '\0';
                        printf("Poll data from pipe 1: %s\n", buffer);
                        pipe1_read = true;
                    }
                }

                // Check for data on pipe2
                if (!pipe2_read && (fds[1].revents & POLLIN))
                {
                    ssize_t bytes = read(pipe2[0], buffer, sizeof(buffer) - 1);
                    if (bytes > 0)
                    {
                        buffer[bytes] = '\0';
                        printf("Poll data from pipe 2: %s\n", buffer);
                        pipe2_read = true;
                    }
                }
            }
        }

        // Clean up
        close(pipe1[0]);
        close(pipe2[0]);

        // Wait for child to finish
        wait(NULL);
    }
}

int main()
{
    printf("=== Non-blocking I/O Demo ===\n");

    // Run demos
    demo_nonblocking_read();
    demo_select();
    demo_poll();

    return 0;
}
