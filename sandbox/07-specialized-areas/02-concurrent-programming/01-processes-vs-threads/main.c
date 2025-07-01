#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

// Global variables
int global_var = 0;

// Thread function
void* thread_function(void* arg)
{
    int thread_id = *(int*) arg;
    int local_var = 0;

    // Modify both local and global variables
    local_var += 100 + thread_id;
    global_var += 100 + thread_id;

    printf("[Thread %d] Address of local_var: %p, Value: %d\n",
           thread_id,
           &local_var,
           local_var);
    printf("[Thread %d] Address of global_var: %p, Value: %d\n",
           thread_id,
           &global_var,
           global_var);

    sleep(1);  // Sleep to demonstrate concurrency

    // Read again to show that global_var might have changed
    printf(
        "[Thread %d] After sleeping, global_var: %d\n", thread_id, global_var);

    return NULL;
}

// Process demonstration
void process_demo()
{
    printf("\n=== PROCESS DEMONSTRATION ===\n");
    printf("Parent process ID: %d\n", getpid());

    global_var = 50;  // Set initial value
    printf("Initial global_var: %d at address %p\n", global_var, &global_var);

    // Create a child process
    pid_t pid = fork();

    if (pid < 0)
    {
        // Error
        fprintf(stderr, "Fork failed\n");
        exit(1);
    }
    else if (pid == 0)
    {
        // Child process
        printf("[Child] Process ID: %d, Parent ID: %d\n", getpid(), getppid());
        printf("[Child] Initial global_var: %d at address %p\n",
               global_var,
               &global_var);

        // Modify the global variable
        global_var += 100;
        printf("[Child] Modified global_var: %d at address %p\n",
               global_var,
               &global_var);

        sleep(2);  // Sleep to demonstrate independence

        printf("[Child] After sleeping, global_var: %d at address %p\n",
               global_var,
               &global_var);
        exit(0);
    }
    else
    {
        // Parent process
        printf("[Parent] Child process ID: %d\n", pid);

        sleep(1);  // Sleep to let child run first

        // Modify the global variable in parent
        global_var += 200;
        printf("[Parent] Modified global_var: %d at address %p\n",
               global_var,
               &global_var);

        // Wait for child to finish
        int status;
        waitpid(pid, &status, 0);
        printf("[Parent] Child process exited with status %d\n",
               WEXITSTATUS(status));
        printf("[Parent] Final global_var: %d\n", global_var);
    }
}

// Thread demonstration
void thread_demo()
{
    printf("\n=== THREAD DEMONSTRATION ===\n");
    printf("Process ID: %d\n", getpid());

    global_var = 50;  // Reset initial value
    printf("Initial global_var: %d at address %p\n", global_var, &global_var);

    // Create two threads
    pthread_t threads[2];
    int thread_ids[2] = {1, 2};

    for (int i = 0; i < 2; i++)
    {
        int result = pthread_create(
            &threads[i], NULL, thread_function, &thread_ids[i]);
        if (result != 0)
        {
            fprintf(
                stderr, "pthread_create failed with error code %d\n", result);
            exit(1);
        }
        printf("Thread %d created\n", i + 1);
    }

    // Main thread can also modify the global variable
    sleep(1);
    global_var += 300;
    printf("[Main thread] Modified global_var: %d at address %p\n",
           global_var,
           &global_var);

    // Wait for threads to finish
    for (int i = 0; i < 2; i++)
    {
        pthread_join(threads[i], NULL);
        printf("Thread %d joined\n", i + 1);
    }

    printf("[Main thread] Final global_var: %d\n", global_var);
}

// Compare processes and threads creation time
void measure_performance()
{
    printf("\n=== PERFORMANCE COMPARISON ===\n");

    const int NUM_CHILDREN = 5;

    printf("Creating %d processes...\n", NUM_CHILDREN);

    clock_t start_time = clock();

    // Create processes
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            fprintf(stderr, "Fork failed\n");
            exit(1);
        }
        else if (pid == 0)
        {
            // Child process
            sleep(2);
            exit(0);
        }
    }

    // Wait for all children
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        wait(NULL);
    }

    clock_t end_time = clock();
    double process_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time taken to create and join %d processes: %.6f seconds\n",
           NUM_CHILDREN,
           process_time);

    // Now create threads
    pthread_t threads[NUM_CHILDREN];
    int thread_ids[NUM_CHILDREN];

    printf("\nCreating %d threads...\n", NUM_CHILDREN);

    start_time = clock();

    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    // Wait for all threads
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        pthread_join(threads[i], NULL);
    }

    end_time = clock();
    double thread_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time taken to create and join %d threads: %.6f seconds\n",
           NUM_CHILDREN,
           thread_time);
}

int main()
{
    printf("==== PROCESSES VS THREADS DEMONSTRATION ====\n");

    // Demonstrate process concurrency
    process_demo();

    // Demonstrate thread concurrency
    thread_demo();

    // Measure and compare creation time
    measure_performance();

    return 0;
}
