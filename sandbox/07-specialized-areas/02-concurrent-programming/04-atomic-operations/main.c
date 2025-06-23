#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

// Global counter variables
int non_atomic_counter = 0;
atomic_int atomic_counter = 0;

// Non-atomic flag for thread termination
int should_exit = 0;

// Atomic flag for thread termination
atomic_flag exit_flag = ATOMIC_FLAG_INIT;

// Thread function that increments non-atomic counter
void* increment_non_atomic(void* arg) {
    int thread_id = *(int*)arg;
    int iterations = 0;
    
    printf("[Thread %d] Incrementing non-atomic counter\n", thread_id);
    
    for (int i = 0; i < 1000000; i++) {
        non_atomic_counter++; // This operation is not atomic!
        iterations++;
    }
    
    printf("[Thread %d] Done. Performed %d increments.\n", thread_id, iterations);
    return NULL;
}

// Thread function that increments atomic counter
void* increment_atomic(void* arg) {
    int thread_id = *(int*)arg;
    int iterations = 0;
    
    printf("[Thread %d] Incrementing atomic counter\n", thread_id);
    
    for (int i = 0; i < 1000000; i++) {
        atomic_fetch_add(&atomic_counter, 1); // Atomic increment
        iterations++;
    }
    
    printf("[Thread %d] Done. Performed %d atomic increments.\n", thread_id, iterations);
    return NULL;
}

// Thread function that uses compare-and-exchange
void* compare_exchange_thread(void* arg) {
    int thread_id = *(int*)arg;
    int expected, desired;
    int successful_exchanges = 0;
    int failed_exchanges = 0;
    
    printf("[Thread %d] Starting compare-exchange operations\n", thread_id);
    
    for (int i = 0; i < 100; i++) {
        // Get the current value
        expected = atomic_load(&atomic_counter);
        
        // Try to increment it by a thread-specific amount
        desired = expected + thread_id;
        
        // Compare-and-exchange: Only succeeds if no other thread modified
        // the value between our read and our update attempt
        if (atomic_compare_exchange_weak(&atomic_counter, &expected, desired)) {
            printf("[Thread %d] Successfully changed counter %d -> %d\n",
                   thread_id, expected, desired);
            successful_exchanges++;
        } else {
            printf("[Thread %d] Failed to exchange: expected %d but found %d\n", 
                   thread_id, expected, atomic_load(&atomic_counter));
            failed_exchanges++;
        }
        
        // Add some randomness to create contention
        usleep((rand() % 5) * 1000);
    }
    
    printf("[Thread %d] Compare-exchange stats: %d successful, %d failed\n",
           thread_id, successful_exchanges, failed_exchanges);
    
    return NULL;
}

// Thread function using memory order relaxed
void* relaxed_memory_thread(void* arg) {
    int thread_id = *(int*)arg;
    
    printf("[Thread %d] Using memory_order_relaxed\n", thread_id);
    
    for (int i = 0; i < 1000; i++) {
        // Relaxed ordering - only guarantees atomicity, but no ordering constraints
        atomic_fetch_add_explicit(&atomic_counter, 1, memory_order_relaxed);
        
        // No particular ordering between atomic operations
        if (i % 100 == 0) {
            printf("[Thread %d] Relaxed counter: %d\n", thread_id, 
                   atomic_load_explicit(&atomic_counter, memory_order_relaxed));
        }
    }
    
    return NULL;
}

// Thread function using memory order sequential consistency
void* sequential_memory_thread(void* arg) {
    int thread_id = *(int*)arg;
    
    printf("[Thread %d] Using memory_order_seq_cst\n", thread_id);
    
    for (int i = 0; i < 1000; i++) {
        // Sequential consistency - strongest ordering, all threads see same order
        atomic_fetch_add_explicit(&atomic_counter, 1, memory_order_seq_cst);
        
        // Operations will appear to execute in a global, consistent order
        if (i % 100 == 0) {
            printf("[Thread %d] Sequential counter: %d\n", thread_id, 
                   atomic_load_explicit(&atomic_counter, memory_order_seq_cst));
        }
    }
    
    return NULL;
}

// Thread function that uses atomic flag as a spinlock
void* spinlock_thread(void* arg) {
    int thread_id = *(int*)arg;
    
    printf("[Thread %d] Trying to acquire spinlock\n", thread_id);
    
    // Try to acquire the spinlock by setting the flag
    while (atomic_flag_test_and_set(&exit_flag)) {
        // If flag was already set, we spin and try again
        printf("[Thread %d] Spinlock busy, spinning...\n", thread_id);
        usleep(500000); // 500ms
    }
    
    // We have the spinlock now
    printf("[Thread %d] Acquired spinlock, working...\n", thread_id);
    
    // Do some work
    sleep(2);
    
    // Release the spinlock
    printf("[Thread %d] Releasing spinlock\n", thread_id);
    atomic_flag_clear(&exit_flag);
    
    return NULL;
}

// Demonstration of race conditions with non-atomic counters
void race_condition_demo() {
    printf("\n=== RACE CONDITION DEMONSTRATION ===\n");
    
    pthread_t threads[4];
    int thread_ids[4] = {1, 2, 3, 4};
    
    // Reset counters
    non_atomic_counter = 0;
    atomic_store(&atomic_counter, 0);
    
    printf("Creating 4 threads to increment non-atomic counter...\n");
    printf("Each thread will increment 1,000,000 times\n");
    printf("Expected final value: 4,000,000\n");
    
    // Create threads
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, increment_non_atomic, &thread_ids[i]);
    }
    
    // Join threads
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Final non-atomic counter value: %d\n", non_atomic_counter);
    printf("If less than 4,000,000, a race condition occurred!\n");
}

// Demonstration of atomic counters
void atomic_counter_demo() {
    printf("\n=== ATOMIC COUNTER DEMONSTRATION ===\n");
    
    pthread_t threads[4];
    int thread_ids[4] = {1, 2, 3, 4};
    
    // Reset counters
    non_atomic_counter = 0;
    atomic_store(&atomic_counter, 0);
    
    printf("Creating 4 threads to increment atomic counter...\n");
    printf("Each thread will increment 1,000,000 times\n");
    printf("Expected final value: 4,000,000\n");
    
    // Create threads
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], NULL, increment_atomic, &thread_ids[i]);
    }
    
    // Join threads
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Final atomic counter value: %d\n", atomic_load(&atomic_counter));
    printf("With atomic operations, the result should be exactly 4,000,000\n");
}

// Demonstration of atomic compare-exchange operations
void compare_exchange_demo() {
    printf("\n=== COMPARE-EXCHANGE DEMONSTRATION ===\n");
    
    pthread_t threads[3];
    int thread_ids[3] = {10, 20, 30}; // Thread-specific increment values
    
    // Reset counter
    atomic_store(&atomic_counter, 0);
    
    printf("Creating 3 threads to perform compare-exchange operations...\n");
    
    // Create threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, compare_exchange_thread, &thread_ids[i]);
    }
    
    // Join threads
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Final atomic counter value after compare-exchange: %d\n", 
           atomic_load(&atomic_counter));
    
    printf("Compare-exchange operations allow atomic updates\n");
    printf("while avoiding the 'lost update' problem.\n");
}

// Demonstration of memory ordering
void memory_ordering_demo() {
    printf("\n=== MEMORY ORDERING DEMONSTRATION ===\n");
    
    pthread_t threads[4];
    int thread_ids[4] = {1, 2, 3, 4};
    
    // Reset counter
    atomic_store(&atomic_counter, 0);
    
    printf("Creating threads with different memory ordering semantics...\n");
    
    // Create threads: 2 relaxed and 2 sequential
    pthread_create(&threads[0], NULL, relaxed_memory_thread, &thread_ids[0]);
    pthread_create(&threads[1], NULL, relaxed_memory_thread, &thread_ids[1]);
    pthread_create(&threads[2], NULL, sequential_memory_thread, &thread_ids[2]);
    pthread_create(&threads[3], NULL, sequential_memory_thread, &thread_ids[3]);
    
    // Join threads
    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Final atomic counter value: %d\n", atomic_load(&atomic_counter));
    
    printf("\nMemory ordering affects how atomic operations are ordered\n");
    printf("between threads, which can impact correctness and performance.\n");
}

// Demonstration of atomic flag as spinlock
void atomic_flag_demo() {
    printf("\n=== ATOMIC FLAG SPINLOCK DEMONSTRATION ===\n");
    
    pthread_t threads[3];
    int thread_ids[3] = {1, 2, 3};
    
    // Initialize the flag (clear it)
    atomic_flag_clear(&exit_flag);
    
    printf("Creating 3 threads that will use atomic_flag as a spinlock...\n");
    printf("Only one thread can acquire the spinlock at a time.\n");
    
    // Create threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, spinlock_thread, &thread_ids[i]);
        usleep(100000); // Small delay between thread creation
    }
    
    // Join threads
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("All threads have released the spinlock\n");
}

// Explain atomic operations and memory models
void explain_atomics() {
    printf("\n=== ATOMIC OPERATIONS EXPLAINED ===\n");
    
    printf("1. Atomic Operations:\n");
    printf("   - Appear to execute indivisibly (all or nothing)\n");
    printf("   - Cannot be interrupted by other threads\n");
    printf("   - Avoid the need for explicit locks in many cases\n");
    printf("   - Often map to special CPU instructions\n\n");
    
    printf("2. C11 Atomic Types:\n");
    printf("   - atomic_int, atomic_bool, etc.\n");
    printf("   - Can also use _Atomic type qualifier\n");
    printf("   - Operations include load, store, exchange, fetch_add, etc.\n\n");
    
    printf("3. Compare-and-Exchange:\n");
    printf("   - Atomic read-modify-write operation\n");
    printf("   - Only updates if value hasn't changed since last read\n");
    printf("   - Foundation for lock-free algorithms\n\n");
    
    printf("4. Memory Ordering Models:\n");
    printf("   - memory_order_relaxed: Only atomicity, no ordering guarantees\n");
    printf("   - memory_order_acquire: Synchronizes with release operations\n");
    printf("   - memory_order_release: Makes prior writes visible to acquire\n");
    printf("   - memory_order_seq_cst: Strongest guarantees, total global order\n\n");
    
    printf("5. Atomic Flags:\n");
    printf("   - Simplest atomic type, can be set or cleared\n");
    printf("   - Useful for simple signaling between threads\n");
    printf("   - Can be used to implement spinlocks\n\n");
    
    printf("6. Performance Considerations:\n");
    printf("   - Atomic operations are more efficient than locks for simple cases\n");
    printf("   - Stronger memory ordering = lower performance\n");
    printf("   - Useful for counters, flags, and simple shared state\n");
}

int main() {
    printf("==== ATOMIC OPERATIONS DEMONSTRATION ====\n");
    
    // Seed random number generator
    srand(time(NULL));
    
    // Run demonstrations
    race_condition_demo();
    atomic_counter_demo();
    compare_exchange_demo();
    memory_ordering_demo();
    atomic_flag_demo();
    explain_atomics();
    
    return 0;
}