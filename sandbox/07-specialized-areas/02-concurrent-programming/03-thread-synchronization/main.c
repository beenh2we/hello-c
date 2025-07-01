
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Global data that will be shared between threads
int shared_counter = 0;

// Mutex for protecting the shared counter
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

// Condition variable for signaling between threads
pthread_cond_t counter_threshold_cv = PTHREAD_COND_INITIALIZER;

// Semaphore for limiting resource access
sem_t resource_semaphore;

// Readers-writer lock for shared data access
pthread_rwlock_t shared_data_lock = PTHREAD_RWLOCK_INITIALIZER;
int shared_data = 0;

// Thread function demonstrating data race
void* increment_without_sync(void* arg)
{
    int thread_id = *(int*) arg;
    int local_counter = 0;

    printf("[Thread %d] Starting (no synchronization)\n", thread_id);

    // Update the counter without synchronization
    for (int i = 0; i < 100000; i++)
    {
        shared_counter++;  // This is not thread-safe!
        local_counter++;
    }

    printf("[Thread %d] Done. Added %d, counter = %d\n",
           thread_id,
           local_counter,
           shared_counter);

    return NULL;
}

// Thread function with mutex synchronization
void* increment_with_mutex(void* arg)
{
    int thread_id = *(int*) arg;
    int local_counter = 0;

    printf("[Thread %d] Starting (with mutex)\n", thread_id);

    // Update the counter with mutex protection
    for (int i = 0; i < 100000; i++)
    {
        pthread_mutex_lock(&counter_mutex);
        shared_counter++;
        local_counter++;
        pthread_mutex_unlock(&counter_mutex);
    }

    printf("[Thread %d] Done. Added %d, counter = %d\n",
           thread_id,
           local_counter,
           shared_counter);

    return NULL;
}

// Condition variable: worker thread
void* counter_worker(void* arg)
{
    int thread_id = *(int*) arg;

    printf("[Worker %d] Starting...\n", thread_id);

    // Increment the counter until threshold
    while (1)
    {
        pthread_mutex_lock(&counter_mutex);

        // Check if we need to increment or we're done
        if (shared_counter >= 10)
        {
            printf("[Worker %d] Counter reached threshold, exiting\n",
                   thread_id);
            pthread_mutex_unlock(&counter_mutex);
            break;
        }

        // Increment the counter
        shared_counter++;
        printf("[Worker %d] Incremented counter to %d\n",
               thread_id,
               shared_counter);

        // Signal if we reached the threshold
        if (shared_counter == 10)
        {
            printf("[Worker %d] Counter reached threshold, signaling\n",
                   thread_id);
            pthread_cond_signal(&counter_threshold_cv);
        }

        pthread_mutex_unlock(&counter_mutex);

        // Simulate work
        usleep(500000);  // 500ms
    }

    return NULL;
}

// Condition variable: watcher thread
void* counter_watcher(void* arg)
{
    printf("[Watcher] Starting...\n");

    // Wait for the counter to reach the threshold
    pthread_mutex_lock(&counter_mutex);

    while (shared_counter < 10)
    {
        printf("[Watcher] Counter = %d, waiting for threshold...\n",
               shared_counter);
        pthread_cond_wait(&counter_threshold_cv, &counter_mutex);
    }

    printf("[Watcher] Received signal! Counter = %d\n", shared_counter);

    pthread_mutex_unlock(&counter_mutex);

    return NULL;
}

// Semaphore: resource user thread
void* resource_user(void* arg)
{
    int thread_id = *(int*) arg;

    printf("[Thread %d] Waiting to access resource...\n", thread_id);

    // Wait for semaphore
    sem_wait(&resource_semaphore);

    printf("[Thread %d] Acquired resource, using it...\n", thread_id);

    // Simulate resource usage
    sleep(2);

    printf("[Thread %d] Finished using resource, releasing\n", thread_id);

    // Release semaphore
    sem_post(&resource_semaphore);

    return NULL;
}

// Reader thread for readers-writer lock
void* reader_thread(void* arg)
{
    int thread_id = *(int*) arg;

    for (int i = 0; i < 3; i++)
    {
        // Acquire read lock
        printf("[Reader %d] Trying to acquire read lock...\n", thread_id);
        pthread_rwlock_rdlock(&shared_data_lock);

        // Read data
        printf("[Reader %d] Read lock acquired. Reading data: %d\n",
               thread_id,
               shared_data);

        // Simulate reading
        usleep((rand() % 1000) * 1000);  // 0-1000ms

        // Release lock
        printf("[Reader %d] Releasing read lock\n", thread_id);
        pthread_rwlock_unlock(&shared_data_lock);

        // Wait a bit before next read
        usleep((rand() % 500) * 1000);  // 0-500ms
    }

    return NULL;
}

// Writer thread for readers-writer lock
void* writer_thread(void* arg)
{
    int thread_id = *(int*) arg;

    for (int i = 0; i < 2; i++)
    {
        // Simulate thinking about what to write
        usleep((rand() % 1000) * 1000);  // 0-1000ms

        // Acquire write lock
        printf("[Writer %d] Trying to acquire write lock...\n", thread_id);
        pthread_rwlock_wrlock(&shared_data_lock);

        // Update data
        shared_data += 10 * thread_id;
        printf("[Writer %d] Write lock acquired. Updated data to: %d\n",
               thread_id,
               shared_data);

        // Simulate writing (this blocks all readers)
        usleep(1000 * 1000);  // 1000ms

        // Release lock
        printf("[Writer %d] Releasing write lock\n", thread_id);
        pthread_rwlock_unlock(&shared_data_lock);
    }

    return NULL;
}

// Demonstrate data race without synchronization
void data_race_demo()
{
    printf("\n=== DATA RACE DEMONSTRATION ===\n");

    shared_counter = 0;  // Reset counter

    pthread_t threads[2];
    int thread_ids[2] = {1, 2};

    printf("Creating threads without synchronization...\n");
    printf("Expected final counter value: 200,000\n");

    // Create threads
    for (int i = 0; i < 2; i++)
    {
        pthread_create(
            &threads[i], NULL, increment_without_sync, &thread_ids[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < 2; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Final counter value: %d\n", shared_counter);
    printf("If this is less than 200,000, a data race occurred!\n");
}

// Demonstrate mutex for synchronization
void mutex_demo()
{
    printf("\n=== MUTEX SYNCHRONIZATION DEMO ===\n");

    shared_counter = 0;  // Reset counter

    pthread_t threads[2];
    int thread_ids[2] = {1, 2};

    printf("Creating threads with mutex synchronization...\n");
    printf("Expected final counter value: 200,000\n");

    // Create threads
    for (int i = 0; i < 2; i++)
    {
        pthread_create(&threads[i], NULL, increment_with_mutex, &thread_ids[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < 2; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Final counter value: %d\n", shared_counter);
    printf(
        "With proper mutex synchronization, the result should be exactly 200,000\n");
}

// Demonstrate condition variables
void condition_variable_demo()
{
    printf("\n=== CONDITION VARIABLE DEMO ===\n");

    shared_counter = 0;  // Reset counter

    pthread_t worker_threads[2];
    pthread_t watcher_thread;
    int thread_ids[2] = {1, 2};

    printf("Creating worker threads and watcher thread...\n");

    // Create watcher thread
    pthread_create(&watcher_thread, NULL, counter_watcher, NULL);

    // Create worker threads
    for (int i = 0; i < 2; i++)
    {
        pthread_create(
            &worker_threads[i], NULL, counter_worker, &thread_ids[i]);
    }

    // Wait for all threads to finish
    pthread_join(watcher_thread, NULL);
    for (int i = 0; i < 2; i++)
    {
        pthread_join(worker_threads[i], NULL);
    }

    printf("All threads completed, final counter value: %d\n", shared_counter);
}

// Demonstrate semaphores
void semaphore_demo()
{
    printf("\n=== SEMAPHORE DEMO ===\n");

    // Initialize semaphore with 2 available resources
    sem_init(&resource_semaphore, 0, 2);

    pthread_t threads[5];
    int thread_ids[5] = {1, 2, 3, 4, 5};

    printf(
        "Creating 5 threads to access 2 resources (limited by semaphore)...\n");

    // Create threads
    for (int i = 0; i < 5; i++)
    {
        pthread_create(&threads[i], NULL, resource_user, &thread_ids[i]);
        usleep(100000);  // Small delay between thread creation
    }

    // Wait for all threads to finish
    for (int i = 0; i < 5; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Clean up
    sem_destroy(&resource_semaphore);

    printf("Semaphore demonstration completed\n");
}

// Demonstrate readers-writer locks
void rwlock_demo()
{
    printf("\n=== READERS-WRITER LOCK DEMO ===\n");

    pthread_t readers[3];
    pthread_t writers[2];
    int reader_ids[3] = {1, 2, 3};
    int writer_ids[2] = {1, 2};

    // Seed random number generator
    srand(time(NULL));

    printf("Creating 3 reader threads and 2 writer threads...\n");

    // Create threads
    for (int i = 0; i < 3; i++)
    {
        pthread_create(&readers[i], NULL, reader_thread, &reader_ids[i]);
    }

    for (int i = 0; i < 2; i++)
    {
        pthread_create(&writers[i], NULL, writer_thread, &writer_ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < 3; i++)
    {
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < 2; i++)
    {
        pthread_join(writers[i], NULL);
    }

    printf("Readers-writer lock demonstration completed\n");
    printf("Final shared data value: %d\n", shared_data);
}

int main()
{
    printf("==== THREAD SYNCHRONIZATION DEMONSTRATION ====\n");

    // Run demonstrations
    data_race_demo();
    mutex_demo();
    condition_variable_demo();
    semaphore_demo();
    rwlock_demo();

    // Clean up synchronization primitives
    pthread_mutex_destroy(&counter_mutex);
    pthread_cond_destroy(&counter_threshold_cv);
    pthread_rwlock_destroy(&shared_data_lock);

    return 0;
}
