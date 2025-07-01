# POSIX Threads (Pthreads) Programming

Threads are the smallest unit of program execution, representing an independent execution path within a process. Compared to creating new processes, threads offer advantages in:

- **Resource Sharing**: Multiple threads within the same process share the same memory space and system resources
- **Low Overhead**: Creating and switching threads requires much less overhead than creating and switching processes
- **Efficient Communication**: Communication between threads is simpler and faster than inter-process communication
- **Better Responsiveness**: Helps maintain continuous responsiveness in applications

POSIX threads (commonly called pthreads) are a standardized multi-threading programming interface widely used in Unix-like systems (such as Linux, macOS). It provides a set of C library functions for creating and managing threads.

## Thread Creation and Joining

### Creating Threads

Use the `pthread_create()` function to create a new thread:

```c
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine)(void*), void *arg);
```

Parameters:

- `thread`: Output parameter to store the identifier of the new thread
- `attr`: Thread attributes, can be set to NULL to use default attributes
- `start_routine`: Function to be executed by the thread
- `arg`: Parameters passed to the thread function

### Waiting for Thread Completion

Use the `pthread_join()` function to wait for a thread to terminate and obtain its return value:

```c
int pthread_join(pthread_t thread, void **retval);
```

Parameters:

- `thread`: Identifier of the thread to wait for
- `retval`: Used to store the return value of the thread

### Basic Example

Here is a simple example of thread creation and joining:

```c
void* hello_thread(void* arg) {
    int thread_id = *(int*)arg;
    printf("Hello from thread %d\n", thread_id);

    // Return a result
    int* result = malloc(sizeof(int));
    *result = thread_id * 10;
    return result;  // Equivalent to pthread_exit(result)
}

void thread_demo() {
    pthread_t thread;
    int thread_id = 1;

    // Create thread
    pthread_create(&thread, NULL, hello_thread, &thread_id);

    // Wait for thread to finish and get return value
    void* thread_result;
    pthread_join(thread, &thread_result);

    // Process the return value
    int result_value = *(int*)thread_result;
    printf("Thread returned: %d\n", result_value);

    // Free memory
    free(thread_result);
}
```

**Important Notes**:

- The return value of a thread function must be the address of a global variable or heap-allocated memory, not the address of a local variable
- The return value obtained through `pthread_join` must be freed by the caller

## Passing Parameters to Threads

There are two main ways to pass data to threads:

### 1. Passing Simple Data Types

Pass simple data through a pointer:

```c
int value = 42;
pthread_create(&thread, NULL, thread_function, &value);
```

In the thread function:

```c
void* thread_function(void* arg) {
    int value = *(int*)arg;
    // Use value...
    return NULL;
}
```

### 2. Passing Multiple Parameters

When you need to pass multiple parameters, use a structure:

```c
typedef struct {
    int id;
    char message[100];
    int sleep_time;
} ThreadParams;

void thread_params_demo() {
    pthread_t threads[2];
    ThreadParams params[2] = {
        {1, "Hello from thread one!", 2},
        {2, "Greetings from thread two!", 3}
    };

    // Create threads
    for (int i = 0; i < 2; i++) {
        pthread_create(&threads[i], NULL, parameterized_thread, &params[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }
}

void* parameterized_thread(void* arg) {
    ThreadParams* params = (ThreadParams*)arg;
    printf("[Thread %d] Received: %s\n", params->id, params->message);
    sleep(params->sleep_time);
    printf("[Thread %d] Slept for %d seconds\n", params->id, params->sleep_time);
    return NULL;
}
```

**Note**: Ensure that data passed to threads remains valid during thread execution. Avoid passing addresses of local variables unless you ensure these variables have a sufficiently long lifetime.

## Managing Multiple Threads

Basic pattern for creating multiple threads:

```c
#define NUM_THREADS 3
pthread_t threads[NUM_THREADS];
int thread_ids[NUM_THREADS];

void multiple_threads_demo() {
    // Create multiple threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, counting_thread, &thread_ids[i]);
        printf("Created thread %d\n", i + 1);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        printf("Thread %d has finished\n", i + 1);
    }
}

void* counting_thread(void* arg) {
    int thread_id = *(int*)arg;
    int count_to = thread_id * 3;

    printf("[Thread %d] Counting to %d...\n", thread_id, count_to);

    for (int i = 1; i <= count_to; i++) {
        printf("[Thread %d] Count: %d\n", thread_id, i);
        usleep(500000);  // Sleep for 500ms
    }

    printf("[Thread %d] Counting completed\n", thread_id);
    return NULL;
}
```

**Key Concepts**:

- Threads execute in an unpredictable order
- The main thread and all other threads execute concurrently
- Use `pthread_join` to ensure waiting for all threads to complete

## Thread Attributes

Thread attributes allow customizing thread behavior, such as stack size and detach state:

```c
void thread_attributes_demo() {
    pthread_t thread;
    pthread_attr_t attr;
    int thread_id = 1;

    // Initialize attributes
    pthread_attr_init(&attr);

    // Set to detached state (no need for pthread_join)
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    // Set stack size (1MB)
    size_t stack_size = 1024 * 1024;
    pthread_attr_setstacksize(&attr, stack_size);

    // Create thread with custom attributes
    pthread_create(&thread, &attr, hello_thread, &thread_id);

    // Destroy attribute object
    pthread_attr_destroy(&attr);

    // Since the thread is detached, no need to call pthread_join
    sleep(1);  // Give the thread enough time to run and exit
}
```

**Thread Detach States**:

- **Joinable State**: Default state, resources must be reclaimed by calling `pthread_join` after the thread ends
- **Detached State**: Resources are automatically released when the thread ends, cannot be joined

You can also set an already created thread to detached state:

```c
pthread_detach(pthread_self());  // Thread sets itself to detached state
```

## Thread Cancellation

Thread cancellation is used to request that a thread terminate execution:

```c
void thread_cancellation_demo() {
    pthread_t thread;
    int thread_id = 1;

    // Create cancellable thread
    pthread_create(&thread, NULL, cancellable_thread, &thread_id);

    // Wait for a while and then send cancellation request
    sleep(3);
    printf("Sending cancellation request\n");
    pthread_cancel(thread);

    // Wait for thread to respond to cancellation request
    void* result;
    pthread_join(thread, &result);

    if (result == PTHREAD_CANCELED) {
        printf("Thread was successfully cancelled\n");
    }
}

void* cancellable_thread(void* arg) {
    int thread_id = *(int*)arg;

    // Set cancellation state and type
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    for (int i = 0; i < 10; i++) {
        printf("[Thread %d] Working... (%d/10)\n", thread_id, i + 1);
        sleep(1);  // This is a cancellation point
        pthread_testcancel();  // Explicit cancellation point
    }

    return NULL;
}
```

**Cancellation Types**:

- **Deferred Cancellation**: Default type, checks cancellation requests only at specific cancellation points
- **Asynchronous Cancellation**: Can be cancelled at any moment, but unsafe to use

**Cancellation Points** include:

- Many blocking system calls (such as `read()`, `write()`, `sleep()`)
- `pthread_testcancel()` (explicit cancellation point)
- pthread functions like `pthread_join()`, `pthread_cond_wait()`, etc.

## Thread-specific Data

Thread-specific data allows each thread to maintain its own copy of data:

```c
pthread_key_t thread_log_key;

void thread_specific_data_demo() {
    // Create key with cleanup function
    pthread_key_create(&thread_log_key, cleanup_log);

    pthread_t threads[2];
    int thread_ids[2] = {1, 2};

    // Create threads that use thread-specific data
    for (int i = 0; i < 2; i++) {
        pthread_create(&threads[i], NULL, logging_thread, &thread_ids[i]);
    }

    // Wait for threads to finish
    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the key
    pthread_key_delete(thread_log_key);
}

void* logging_thread(void* arg) {
    int thread_id = *(int*)arg;
    char msg[100];

    // Log a few messages
    snprintf(msg, sizeof(msg), "[Thread %d] Starting work", thread_id);
    thread_log(msg);

    sleep(1);

    snprintf(msg, sizeof(msg), "[Thread %d] Making progress", thread_id);
    thread_log(msg);

    sleep(1);

    snprintf(msg, sizeof(msg), "[Thread %d] Finishing work", thread_id);
    thread_log(msg);

    // Display log
    char* log = pthread_getspecific(thread_log_key);
    printf("Thread %d log:\n%s", thread_id, log);

    return NULL;
}

void thread_log(const char* message) {
    // Get log buffer for current thread
    char* buffer = pthread_getspecific(thread_log_key);

    if (!buffer) {
        // First call, allocate buffer
        buffer = malloc(1024);
        buffer[0] = '\0';
        pthread_setspecific(thread_log_key, buffer);
    }

    // Append message
    strcat(buffer, message);
    strcat(buffer, "\n");
}

void cleanup_log(void* buffer) {
    // Called when thread exits
    if (buffer) {
        printf("Cleaning up thread log: %s\n", (char*)buffer);
        free(buffer);
    }
}
```

**Key Functions**:

- `pthread_key_create()`: Creates thread-specific data key, can specify cleanup function
- `pthread_setspecific()`: Sets specific data for current thread
- `pthread_getspecific()`: Gets specific data for current thread

## Advanced Topics and Best Practices

### Thread Synchronization

Thread synchronization is a key challenge in multi-threaded programming. Main synchronization tools include:

1. **Mutex (Mutual Exclusion)**

   ```c
   pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

   // Lock mutex
   pthread_mutex_lock(&mutex);
   // Access shared resources
   pthread_mutex_unlock(&mutex);
   ```

2. **Condition Variables**

   ```c
   pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
   pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

   // Wait for condition
   pthread_mutex_lock(&mutex);
   while (!condition_met) {
       pthread_cond_wait(&cond, &mutex);
   }
   pthread_mutex_unlock(&mutex);

   // Signal notification
   pthread_mutex_lock(&mutex);
   condition_met = true;
   pthread_cond_signal(&cond);
   pthread_mutex_unlock(&mutex);
   ```

3. **Read-Write Locks**

   ```c
   pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

   // Read operation
   pthread_rwlock_rdlock(&rwlock);
   // Multiple threads can read simultaneously
   pthread_rwlock_unlock(&rwlock);

   // Write operation
   pthread_rwlock_wrlock(&rwlock);
   // Only one thread can write
   pthread_rwlock_unlock(&rwlock);
   ```

### Common Pitfalls and Best Practices

1. **Avoid Data Races**

   - Identify and protect all shared data
   - Use mutexes or other synchronization mechanisms
   - Consider using thread-specific data instead of global variables

2. **Prevent Deadlocks**

   - Acquire multiple locks in a consistent order
   - Use `pthread_mutex_trylock()` to avoid permanent blocking
   - Design clean locking strategies, avoid nested locks

3. **Resource Management**

   - Ensure memory allocated by threads is freed
   - Appropriately use detached state and cancellation handlers
   - Properly clean up mutexes and condition variables

4. **Performance Optimization**

   - Minimize scope of locks
   - Balance number of threads (typically close to or equal to CPU core count)
   - Use thread pools to avoid frequent thread creation and destruction
   - Properly use read-write locks to distinguish between read and write operations

5. **Debugging Tips**
   - Use thread-safe logging mechanisms
   - Consider using tools like Valgrind, Helgrind to detect race conditions and memory issues
   - Add extra checks and assertions to verify thread behavior

### Thread Safety

Functions or code are "thread-safe" if they can be called by multiple threads simultaneously without causing issues. Methods for achieving thread safety:

1. **Reentrant Design**: Avoid using static data and global variables
2. **Synchronized Access**: Use mechanisms like mutexes to protect shared resources
3. **Thread Local Storage**: Use thread-specific data to isolate state for each thread
4. **Atomic Operations**: For simple operations, use atomic instructions to avoid lock overhead

### Practical Application Scenarios

1. **Parallel Computing**: Distribute computational tasks among multiple threads to fully utilize multi-core processors
2. **I/O Multiplexing**: Perform other tasks while waiting for I/O
3. **Server Applications**: Use thread pools to handle multiple client requests
4. **GUI Applications**: Keep the user interface responsive while performing background tasks
5. **Concurrent Data Processing**: Process multiple data streams or tasks simultaneously

## Summary

POSIX threads provide powerful and flexible multi-threading capabilities. Mastering basic concepts such as thread creation, parameter passing, synchronization, and thread-specific data is essential for developing efficient multi-threaded applications.

Thread programming offers significant advantages in performance and responsiveness but also brings challenges in synchronization and shared resource management. By following best practices, you can effectively avoid common multi-threading pitfalls and leverage the full potential of multi-threaded programming.

Remember, when compiling programs that include pthreads, you need to link the pthread library:

```bash
gcc -o program program.c -pthread
```
