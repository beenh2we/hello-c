/* ipc_mechanisms.c */
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

// Flag for graceful termination
volatile sig_atomic_t keep_running = 1;

// Signal handler
void handle_signal(int sig)
{
    keep_running = 0;
}

// Define message structure for message queues
struct msg_buffer
{
    long msg_type;
    char msg_text[100];
};

// Union for semctl
union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

// Shared memory structure
typedef struct
{
    int value;
    char message[100];
    time_t timestamp;
} SharedData;

// Function to demonstrate unnamed pipes
void unnamed_pipe_demo()
{
    printf("\n=== Unnamed Pipes Demonstration ===\n");

    int pipefd[2];
    pid_t pid;
    char buffer[100];

    // Create pipe
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return;
    }

    // Create child process
    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        // Child process - will write to pipe

        // Close unused read end
        close(pipefd[0]);

        // Prepare message
        char message[50];
        snprintf(message,
                 sizeof(message),
                 "Hello from child process (PID: %d)",
                 getpid());

        printf("[Child] Writing message to pipe...\n");

        // Write to pipe
        write(pipefd[1], message, strlen(message) + 1);

        // Close write end
        close(pipefd[1]);

        exit(0);
    }
    else
    {
        // Parent process - will read from pipe

        // Close unused write end
        close(pipefd[1]);

        // Read from pipe
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer));

        if (bytes_read > 0)
        {
            printf("[Parent] Received message from pipe: %s\n", buffer);
        }
        else
        {
            perror("read");
        }

        // Close read end
        close(pipefd[0]);

        // Wait for child to finish
        wait(NULL);
    }
}

// Function to demonstrate bidirectional pipe communication
void bidirectional_pipe_demo()
{
    printf("\n=== Bidirectional Pipe Communication ===\n");

    int pipe_parent_to_child[2];
    int pipe_child_to_parent[2];
    pid_t pid;
    char buffer[100];

    // Create pipes
    if (pipe(pipe_parent_to_child) == -1 || pipe(pipe_child_to_parent) == -1)
    {
        perror("pipe");
        return;
    }

    // Create child process
    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        // Child process

        // Close unused pipe ends
        close(
            pipe_parent_to_child[1]);  // Close write end of parent->child pipe
        close(pipe_child_to_parent[0]);  // Close read end of child->parent pipe

        // Read message from parent
        ssize_t bytes_read
            = read(pipe_parent_to_child[0], buffer, sizeof(buffer));

        if (bytes_read > 0)
        {
            printf("[Child] Received from parent: %s\n", buffer);

            // Prepare response
            char response[100];
            snprintf(response,
                     sizeof(response),
                     "Hello parent (PID: %d), I got your message!",
                     getpid());

            // Send response to parent
            write(pipe_child_to_parent[1], response, strlen(response) + 1);
        }

        // Close remaining pipe ends
        close(pipe_parent_to_child[0]);
        close(pipe_child_to_parent[1]);

        exit(0);
    }
    else
    {
        // Parent process

        // Close unused pipe ends
        close(pipe_parent_to_child[0]);  // Close read end of parent->child pipe
        close(
            pipe_child_to_parent[1]);  // Close write end of child->parent pipe

        // Prepare message for child
        char message[100];
        snprintf(message,
                 sizeof(message),
                 "Hello child (PID: %d) from parent!",
                 pid);

        // Send message to child
        write(pipe_parent_to_child[1], message, strlen(message) + 1);

        // Read response from child
        ssize_t bytes_read
            = read(pipe_child_to_parent[0], buffer, sizeof(buffer));

        if (bytes_read > 0)
        {
            printf("[Parent] Received from child: %s\n", buffer);
        }

        // Close remaining pipe ends
        close(pipe_parent_to_child[1]);
        close(pipe_child_to_parent[0]);

        // Wait for child to finish
        wait(NULL);
    }
}

// Function to demonstrate named pipes (FIFOs)
void named_pipe_demo()
{
    printf("\n=== Named Pipes (FIFOs) Demonstration ===\n");

    const char *fifo_path = "/tmp/my_fifo";
    pid_t pid;

    // Create FIFO (will fail if it already exists, which is fine)
    if (mkfifo(fifo_path, 0666) < 0 && errno != EEXIST)
    {
        perror("mkfifo");
        return;
    }

    // Create child process
    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        // Child process - writer

        // Open FIFO for writing
        int fifo_fd = open(fifo_path, O_WRONLY);
        if (fifo_fd < 0)
        {
            perror("open");
            exit(1);
        }

        // Prepare and send message
        char message[100];
        snprintf(message,
                 sizeof(message),
                 "Hello from child process via FIFO (PID: %d)",
                 getpid());

        printf("[Child] Writing message to FIFO...\n");
        write(fifo_fd, message, strlen(message) + 1);

        // Close FIFO
        close(fifo_fd);
        exit(0);
    }
    else
    {
        // Parent process - reader

        // Give child time to open FIFO for writing
        usleep(100000);  // 100ms

        // Open FIFO for reading
        int fifo_fd = open(fifo_path, O_RDONLY);
        if (fifo_fd < 0)
        {
            perror("open");
            return;
        }

        // Read message
        char buffer[100];
        ssize_t bytes_read = read(fifo_fd, buffer, sizeof(buffer));

        if (bytes_read > 0)
        {
            printf("[Parent] Received message from FIFO: %s\n", buffer);
        }
        else
        {
            perror("read");
        }

        // Close FIFO
        close(fifo_fd);

        // Wait for child to finish
        wait(NULL);

        // Remove FIFO
        unlink(fifo_path);
    }
}

// Function to demonstrate System V message queues
void message_queue_demo()
{
    printf("\n=== System V Message Queue Demonstration ===\n");

    key_t key = ftok("/tmp", 'A');
    if (key == -1)
    {
        perror("ftok");
        return;
    }

    // Create message queue
    int msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1)
    {
        perror("msgget");
        return;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        // Child process - sender

        // Prepare message
        struct msg_buffer msg;
        msg.msg_type = 1;  // Message type
        snprintf(msg.msg_text,
                 sizeof(msg.msg_text),
                 "Hello from child process via message queue (PID: %d)",
                 getpid());

        // Send message
        printf("[Child] Sending message to queue...\n");
        if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1)
        {
            perror("msgsnd");
            exit(1);
        }

        // Wait and send another message with different type
        sleep(1);
        msg.msg_type = 2;  // Different message type
        snprintf(msg.msg_text,
                 sizeof(msg.msg_text),
                 "This is a priority message (type 2)!");

        if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1)
        {
            perror("msgsnd");
            exit(1);
        }

        exit(0);
    }
    else
    {
        // Parent process - receiver

        // Wait for child to send messages
        sleep(2);

        // Receive messages in specific order (type 2 first, then type 1)
        struct msg_buffer msg;

        // Receive type 2 message first (priority)
        printf("[Parent] Receiving messages from queue...\n");
        if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 2, 0) == -1)
        {
            perror("msgrcv");
        }
        else
        {
            printf("[Parent] Received message type 2: %s\n", msg.msg_text);
        }

        // Receive type 1 message
        if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0) == -1)
        {
            perror("msgrcv");
        }
        else
        {
            printf("[Parent] Received message type 1: %s\n", msg.msg_text);
        }

        // Wait for child to finish
        wait(NULL);

        // Remove message queue
        if (msgctl(msgid, IPC_RMID, NULL) == -1)
        {
            perror("msgctl");
        }
    }
}

// Function to demonstrate System V shared memory
void shared_memory_demo()
{
    printf("\n=== System V Shared Memory Demonstration ===\n");

    key_t key = ftok("/tmp", 'B');
    if (key == -1)
    {
        perror("ftok");
        return;
    }

    // Create shared memory segment
    int shmid = shmget(key, sizeof(SharedData), IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("shmget");
        return;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        // Child process - writer

        // Attach shared memory
        SharedData *shared_data = (SharedData *) shmat(shmid, NULL, 0);
        if (shared_data == (void *) -1)
        {
            perror("shmat");
            exit(1);
        }

        // Write to shared memory
        shared_data->value = 42;
        snprintf(shared_data->message,
                 sizeof(shared_data->message),
                 "Hello from child process via shared memory (PID: %d)",
                 getpid());
        shared_data->timestamp = time(NULL);

        printf("[Child] Updated shared memory\n");

        // Detach shared memory
        if (shmdt(shared_data) == -1)
        {
            perror("shmdt");
        }

        exit(0);
    }
    else
    {
        // Parent process - reader

        // Wait for child to update shared memory
        sleep(1);

        // Attach shared memory
        SharedData *shared_data = (SharedData *) shmat(shmid, NULL, 0);
        if (shared_data == (void *) -1)
        {
            perror("shmat");
            return;
        }

        // Read from shared memory
        printf("[Parent] Reading from shared memory:\n");
        printf("  Value: %d\n", shared_data->value);
        printf("  Message: %s\n", shared_data->message);
        printf("  Timestamp: %s", ctime(&shared_data->timestamp));

        // Detach shared memory
        if (shmdt(shared_data) == -1)
        {
            perror("shmdt");
        }

        // Wait for child to finish
        wait(NULL);

        // Remove shared memory segment
        if (shmctl(shmid, IPC_RMID, NULL) == -1)
        {
            perror("shmctl");
        }
    }
}

// Function to demonstrate System V semaphores
void semaphore_demo()
{
    printf("\n=== System V Semaphores Demonstration ===\n");

    key_t key = ftok("/tmp", 'C');
    if (key == -1)
    {
        perror("ftok");
        return;
    }

    // Create semaphore set with 1 semaphore
    int semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid == -1)
    {
        perror("semget");
        return;
    }

    // Initialize semaphore value to 1 (binary semaphore, initially unlocked)
    union semun arg;
    arg.val = 1;
    if (semctl(semid, 0, SETVAL, arg) == -1)
    {
        perror("semctl");
        return;
    }

    // Create shared memory for demonstration
    key_t shm_key = ftok("/tmp", 'D');
    int shmid = shmget(shm_key, sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("shmget");
        return;
    }

    // Attach shared memory
    int *shared_counter = (int *) shmat(shmid, NULL, 0);
    if (shared_counter == (void *) -1)
    {
        perror("shmat");
        return;
    }

    // Initialize counter
    *shared_counter = 0;

    printf("Starting semaphore demonstration with shared counter\n");

    // Create two child processes
    for (int i = 0; i < 2; i++)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork");
            return;
        }

        if (pid == 0)
        {
            // Child process
            int child_id = i + 1;

            // Semaphore operations
            struct sembuf sem_lock = {0, -1, 0};   // Decrement (lock)
            struct sembuf sem_unlock = {0, 1, 0};  // Increment (unlock)

            // Perform several operations on the shared counter
            for (int j = 0; j < 5; j++)
            {
                // Lock semaphore
                printf("[Child %d] Waiting for semaphore...\n", child_id);
                if (semop(semid, &sem_lock, 1) == -1)
                {
                    perror("semop (lock)");
                    exit(1);
                }

                // Critical section - accessing shared counter
                printf("[Child %d] Got semaphore, counter = %d\n",
                       child_id,
                       *shared_counter);
                (*shared_counter)++;
                printf("[Child %d] Incremented counter to %d\n",
                       child_id,
                       *shared_counter);

                // Simulate some work
                sleep(1);

                // Unlock semaphore
                if (semop(semid, &sem_unlock, 1) == -1)
                {
                    perror("semop (unlock)");
                    exit(1);
                }

                printf("[Child %d] Released semaphore\n", child_id);

                // Wait a bit before next operation
                usleep(500000);  // 500ms
            }

            exit(0);
        }
    }

    // Parent waits for children to finish
    for (int i = 0; i < 2; i++)
    {
        wait(NULL);
    }

    printf("[Parent] Final counter value: %d\n", *shared_counter);

    // Clean up

    // Detach shared memory
    if (shmdt(shared_counter) == -1)
    {
        perror("shmdt");
    }

    // Remove shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("shmctl");
    }

    // Remove semaphore
    if (semctl(semid, 0, IPC_RMID) == -1)
    {
        perror("semctl");
    }
}

// Function to demonstrate POSIX shared memory
void posix_shm_demo()
{
    printf("\n=== POSIX Shared Memory Demonstration ===\n");

    const char *shm_name = "/my_posix_shm";

    // Create shared memory object
    int fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (fd == -1)
    {
        perror("shm_open");
        return;
    }

    // Set size
    if (ftruncate(fd, sizeof(SharedData)) == -1)
    {
        perror("ftruncate");
        close(fd);
        shm_unlink(shm_name);
        return;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        close(fd);
        shm_unlink(shm_name);
        return;
    }

    if (pid == 0)
    {
        // Child process - writer

        // Map shared memory
        SharedData *shared_data = (SharedData *) mmap(NULL,
                                                      sizeof(SharedData),
                                                      PROT_READ | PROT_WRITE,
                                                      MAP_SHARED,
                                                      fd,
                                                      0);
        if (shared_data == MAP_FAILED)
        {
            perror("mmap");
            exit(1);
        }

        // Write to shared memory
        shared_data->value = 100;
        snprintf(shared_data->message,
                 sizeof(shared_data->message),
                 "Hello from child via POSIX shared memory (PID: %d)",
                 getpid());
        shared_data->timestamp = time(NULL);

        printf("[Child] Updated POSIX shared memory\n");

        // Unmap shared memory
        munmap(shared_data, sizeof(SharedData));
        close(fd);

        exit(0);
    }
    else
    {
        // Parent process - reader

        // Wait for child to update shared memory
        sleep(1);

        // Map shared memory
        SharedData *shared_data = (SharedData *) mmap(NULL,
                                                      sizeof(SharedData),
                                                      PROT_READ | PROT_WRITE,
                                                      MAP_SHARED,
                                                      fd,
                                                      0);
        if (shared_data == MAP_FAILED)
        {
            perror("mmap");
            close(fd);
            shm_unlink(shm_name);
            return;
        }

        // Read from shared memory
        printf("[Parent] Reading from POSIX shared memory:\n");
        printf("  Value: %d\n", shared_data->value);
        printf("  Message: %s\n", shared_data->message);
        printf("  Timestamp: %s", ctime(&shared_data->timestamp));

        // Unmap shared memory
        munmap(shared_data, sizeof(SharedData));
        close(fd);

        // Wait for child to finish
        wait(NULL);

        // Remove shared memory object
        shm_unlink(shm_name);
    }
}

int main(int argc, char *argv[])
{
    printf("=== Interprocess Communication (IPC) Demonstration ===\n");

    // Register signal handler
    signal(SIGINT, handle_signal);

    // Run all demos
    unnamed_pipe_demo();
    bidirectional_pipe_demo();
    named_pipe_demo();
    message_queue_demo();
    shared_memory_demo();
    semaphore_demo();
    posix_shm_demo();

    return 0;
}
