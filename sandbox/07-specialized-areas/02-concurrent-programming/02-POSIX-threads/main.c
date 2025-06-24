#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Simple thread function
void* hello_thread(void* arg)
{
    int thread_id = *(int*) arg;
    printf("Hello from thread %d\n", thread_id);

    // Return a result
    int* result = malloc(sizeof(int));
    *result = thread_id * 10;

    pthread_exit(result);  // Same as "return result;"
}

// Thread function with a loop
void* counting_thread(void* arg)
{
    int thread_id = *(int*) arg;
    int count_to = thread_id * 3;

    printf("[Thread %d] Counting to %d...\n", thread_id, count_to);

    for (int i = 1; i <= count_to; i++)
    {
        printf("[Thread %d] Count: %d\n", thread_id, i);
        usleep(500000);  // Sleep for 500ms
    }

    printf("[Thread %d] Counting completed\n", thread_id);

    return NULL;
}

// Thread function that accepts a struct for multiple parameters
typedef struct
{
    int id;
    char message[100];
    int sleep_time;
} ThreadParams;

void* parameterized_thread(void* arg)
{
    ThreadParams* params = (ThreadParams*) arg;

    printf("[Thread %d] Received message: %s\n", params->id, params->message);
    sleep(params->sleep_time);
    printf("[Thread %d] Slept for %d seconds\n", params->id, params->sleep_time);

    return NULL;
}

// Thread function that can be cancelled
void* cancellable_thread(void* arg)
{
    int thread_id = *(int*) arg;

    // Set cancellation state
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    printf("[Thread %d] Starting long operation, can be cancelled...\n", thread_id);

    for (int i = 0; i < 10; i++)
    {
        printf("[Thread %d] Working... (%d/10)\n", thread_id, i + 1);

        // This is a cancellation point
        sleep(1);

        // You can also explicitly create a cancellation point
        pthread_testcancel();
    }

    printf("[Thread %d] Finished without being cancelled\n", thread_id);
    return NULL;
}

// Thread function that uses thread-specific data
pthread_key_t thread_log_key;

void cleanup_log(void* buffer)
{
    if (buffer)
    {
        printf("Cleaning up thread log: %s\n", (char*) buffer);
        free(buffer);
    }
}

void thread_log(const char* message)
{
    char* buffer = pthread_getspecific(thread_log_key);

    if (!buffer)
    {
        // First time initialization
        buffer = malloc(1024);
        buffer[0] = '\0';
        pthread_setspecific(thread_log_key, buffer);
    }

    // Append the message to the buffer
    strcat(buffer, message);
    strcat(buffer, "\n");
}

void* logging_thread(void* arg)
{
    int thread_id = *(int*) arg;

    char msg[100];

    snprintf(msg, sizeof(msg), "[Thread %d] Starting work", thread_id);
    thread_log(msg);

    sleep(1);

    snprintf(msg, sizeof(msg), "[Thread %d] Making progress", thread_id);
    thread_log(msg);

    sleep(1);

    snprintf(msg, sizeof(msg), "[Thread %d] Finishing work", thread_id);
    thread_log(msg);

    // The log buffer will be freed by the cleanup function
    char* log = pthread_getspecific(thread_log_key);
    printf("Thread %d log:\n%s", thread_id, log);

    return NULL;
}

// Basic thread creation and joining
void basic_thread_demo()
{
    printf("\n=== BASIC THREAD CREATION AND JOINING ===\n");

    pthread_t thread;
    int thread_id = 1;

    printf("Creating thread...\n");

    if (pthread_create(&thread, NULL, hello_thread, &thread_id) != 0)
    {
        fprintf(stderr, "Error creating thread\n");
        return;
    }

    printf("Main thread continues execution while thread runs...\n");

    // Wait for the thread to finish and get its result
    void* thread_result;
    pthread_join(thread, &thread_result);

    // Cast and dereference the result
    int result_value = *(int*) thread_result;
    printf("Thread returned: %d\n", result_value);

    // Free the memory allocated in the thread
    free(thread_result);
}

// Creating multiple threads
void multiple_threads_demo()
{
    printf("\n=== MULTIPLE THREADS DEMO ===\n");

#define NUM_THREADS 3
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Create multiple threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_ids[i] = i + 1;

        if (pthread_create(&threads[i], NULL, counting_thread, &thread_ids[i]) != 0)
        {
            fprintf(stderr, "Error creating thread %d\n", i);
            return;
        }

        printf("Created thread %d\n", i + 1);
    }

    printf("All threads are running...\n");

    // Join all threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
        printf("Thread %d has finished\n", i + 1);
    }

    printf("All threads have completed\n");
}

// Thread with parameters
void thread_parameters_demo()
{
    printf("\n=== THREAD PARAMETERS DEMO ===\n");

    pthread_t threads[2];
    ThreadParams thread_params[2] = {{1, "Hello from thread one!", 2},
                                     {2, "Greetings from thread two!", 3}};

    // Create threads with parameter structs
    for (int i = 0; i < 2; i++)
    {
        if (pthread_create(&threads[i], NULL, parameterized_thread, &thread_params[i]) != 0)
        {
            fprintf(stderr, "Error creating parameterized thread %d\n", i);
            return;
        }
    }

    // Join all threads
    for (int i = 0; i < 2; i++)
    {
        pthread_join(threads[i], NULL);
    }
}

// Thread attributes demonstration
void thread_attributes_demo()
{
    printf("\n=== THREAD ATTRIBUTES DEMO ===\n");

    pthread_t thread;
    pthread_attr_t attr;
    int thread_id = 1;

    // Initialize the attribute
    pthread_attr_init(&attr);

    // Set thread as detached (won't need to be joined)
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    // Set stack size (1MB)
    size_t stack_size = 1024 * 1024;
    pthread_attr_setstacksize(&attr, stack_size);

    printf("Creating thread with custom attributes...\n");
    printf("- Detached state: PTHREAD_CREATE_DETACHED\n");
    printf("- Stack size: %zu bytes\n", stack_size);

    // Create thread with attributes
    if (pthread_create(&thread, &attr, hello_thread, &thread_id) != 0)
    {
        fprintf(stderr, "Error creating thread with attributes\n");
        return;
    }

    // Destroy the attribute (cleanup)
    pthread_attr_destroy(&attr);

    printf("Thread created with custom attributes\n");
    printf("Since thread is detached, we don't join it\n");

    // Give the thread time to run and exit
    sleep(1);
}

// Thread cancellation demonstration
void thread_cancellation_demo()
{
    printf("\n=== THREAD CANCELLATION DEMO ===\n");

    pthread_t thread;
    int thread_id = 1;

    // Create a cancellable thread
    if (pthread_create(&thread, NULL, cancellable_thread, &thread_id) != 0)
    {
        fprintf(stderr, "Error creating cancellable thread\n");
        return;
    }

    printf("Thread started, waiting 3 seconds before cancelling...\n");
    sleep(3);

    // Cancel the thread
    printf("Sending cancellation request\n");
    if (pthread_cancel(thread) != 0)
    {
        fprintf(stderr, "Error cancelling thread\n");
    }

    // Wait for thread to acknowledge cancellation
    void* result;
    pthread_join(thread, &result);

    if (result == PTHREAD_CANCELED)
    {
        printf("Thread was successfully cancelled\n");
    }
    else
    {
        printf("Thread was not cancelled (unexpected)\n");
    }
}

// Thread-specific data demonstration
void thread_specific_data_demo()
{
    printf("\n=== THREAD-SPECIFIC DATA DEMO ===\n");

    // Create a key for thread-specific data
    pthread_key_create(&thread_log_key, cleanup_log);

    pthread_t threads[2];
    int thread_ids[2] = {1, 2};

    // Create threads that will use thread-specific storage
    for (int i = 0; i < 2; i++)
    {
        pthread_create(&threads[i], NULL, logging_thread, &thread_ids[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < 2; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Destroy the key
    pthread_key_delete(thread_log_key);

    printf("Thread-specific data demo completed\n");
}

// Explain pthread basics
void explain_pthreads()
{
    printf("\n=== POSIX THREADS (PTHREADS) BASICS ===\n");

    printf("1. Thread Creation:\n");
    printf("   - pthread_create() creates a new thread\n");
    printf("   - Arguments: thread ID, attributes, start function, args\n\n");

    printf("2. Thread Joining:\n");
    printf("   - pthread_join() waits for a thread to exit\n");
    printf("   - Can retrieve thread's return value\n\n");

    printf("3. Thread Attributes:\n");
    printf("   - Control thread behavior (stack size, scheduling, etc.)\n");
    printf("   - Initialized with pthread_attr_init()\n\n");

    printf("4. Thread Cancellation:\n");
    printf("   - pthread_cancel() requests thread termination\n");
    printf("   - Threads can control how/when they respond\n\n");

    printf("5. Thread-Specific Data:\n");
    printf("   - Like thread-local storage\n");
    printf("   - Uses keys created with pthread_key_create()\n\n");

    printf("6. Thread Management:\n");
    printf("   - pthread_self() - get caller's thread ID\n");
    printf("   - pthread_equal() - compare thread IDs\n");
    printf("   - pthread_detach() - mark thread as detached\n\n");

    printf("7. Compile and Link:\n");
    printf("   - Link with -pthread flag: gcc program.c -o program -pthread\n");
}

int main()
{
    printf("==== POSIX THREADS DEMONSTRATION ====\n");

    basic_thread_demo();
    multiple_threads_demo();
    thread_parameters_demo();
    thread_attributes_demo();
    thread_cancellation_demo();
    thread_specific_data_demo();
    explain_pthreads();

    return 0;
}
