#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// =================================================================
// Memory Manager: A comprehensive example combining various concepts
// =================================================================

// Define block sizes for small object pool
#define TINY_BLOCK_SIZE 16
#define SMALL_BLOCK_SIZE 64
#define MEDIUM_BLOCK_SIZE 256

// Maximum number of blocks per pool
#define MAX_BLOCKS_PER_POOL 100

// Magic number for detecting overwrites
#define MEMORY_MAGIC 0xDEADBEEF

// Internal tracking header
typedef struct MemoryHeader
{
    size_t size;                // Size of the allocation
    uint32_t magic;             // Magic number for validation
    const char *file;           // Source file where allocation happened
    int line;                   // Line number where allocation happened
    struct MemoryHeader *prev;  // Previous allocation in the list
    struct MemoryHeader *next;  // Next allocation in the list
} MemoryHeader;

// Block size categories
typedef enum
{
    BLOCK_TINY,
    BLOCK_SMALL,
    BLOCK_MEDIUM,
    BLOCK_LARGE  // For any size larger than medium blocks
} BlockCategory;

// Fixed-size block pool
typedef struct
{
    void *blocks[MAX_BLOCKS_PER_POOL];
    bool used[MAX_BLOCKS_PER_POOL];
    size_t block_size;
    size_t count;
} BlockPool;

// Memory Manager
typedef struct
{
    // Block pools for different sizes
    BlockPool tiny_pool;    // For allocations <= 16 bytes
    BlockPool small_pool;   // For allocations <= 64 bytes
    BlockPool medium_pool;  // For allocations <= 256 bytes

    // Tracking for all allocations (including large ones)
    MemoryHeader *allocations;
    size_t allocation_count;
    size_t total_allocated;
    size_t peak_allocated;

    // Statistics
    size_t malloc_calls;
    size_t free_calls;
    size_t pool_hits;
} MemoryManager;

// Global memory manager
static MemoryManager g_memory_manager = {0};

// Initialize a block pool
void init_block_pool(BlockPool *pool, size_t block_size)
{
    pool->block_size = block_size;
    pool->count = 0;
    memset(pool->used, 0, sizeof(pool->used));
    memset(pool->blocks, 0, sizeof(pool->blocks));
}

// Initialize memory manager
void memory_manager_init()
{
    // Initialize pools
    init_block_pool(&g_memory_manager.tiny_pool, TINY_BLOCK_SIZE);
    init_block_pool(&g_memory_manager.small_pool, SMALL_BLOCK_SIZE);
    init_block_pool(&g_memory_manager.medium_pool, MEDIUM_BLOCK_SIZE);

    // Initialize tracking
    g_memory_manager.allocations = NULL;
    g_memory_manager.allocation_count = 0;
    g_memory_manager.total_allocated = 0;
    g_memory_manager.peak_allocated = 0;

    // Initialize statistics
    g_memory_manager.malloc_calls = 0;
    g_memory_manager.free_calls = 0;
    g_memory_manager.pool_hits = 0;
}

// Add memory block to pool
bool add_block_to_pool(BlockPool *pool)
{
    if (pool->count >= MAX_BLOCKS_PER_POOL)
    {
        return false;  // Pool is full
    }

    // Allocate a new block
    void *block = malloc(pool->block_size);
    if (!block)
    {
        return false;  // Out of memory
    }

    // Add to pool
    pool->blocks[pool->count] = block;
    pool->used[pool->count] = false;
    pool->count++;

    return true;
}

// Get block category based on size
BlockCategory get_block_category(size_t size)
{
    if (size <= TINY_BLOCK_SIZE) return BLOCK_TINY;
    if (size <= SMALL_BLOCK_SIZE) return BLOCK_SMALL;
    if (size <= MEDIUM_BLOCK_SIZE) return BLOCK_MEDIUM;
    return BLOCK_LARGE;
}

// Get corresponding pool for block category
BlockPool *get_pool_for_category(BlockCategory category)
{
    switch (category)
    {
    case BLOCK_TINY:
        return &g_memory_manager.tiny_pool;
    case BLOCK_SMALL:
        return &g_memory_manager.small_pool;
    case BLOCK_MEDIUM:
        return &g_memory_manager.medium_pool;
    default:
        return NULL;
    }
}

// Allocate from block pool
void *allocate_from_pool(BlockPool *pool)
{
    // Find an unused block
    for (size_t i = 0; i < pool->count; i++)
    {
        if (!pool->used[i])
        {
            pool->used[i] = true;
            g_memory_manager.pool_hits++;
            return pool->blocks[i];
        }
    }

    // No unused blocks, try to add a new one
    if (add_block_to_pool(pool))
    {
        size_t index = pool->count - 1;
        pool->used[index] = true;
        return pool->blocks[index];
    }

    return NULL;  // Failed to allocate
}

// Track allocation in the linked list
void track_allocation(void *ptr, size_t size, const char *file, int line)
{
    MemoryHeader *header = (MemoryHeader *) ptr;

    // Set header fields
    header->size = size;
    header->magic = MEMORY_MAGIC;
    header->file = file;
    header->line = line;
    header->prev = NULL;
    header->next = g_memory_manager.allocations;

    // Update linked list
    if (g_memory_manager.allocations)
    {
        g_memory_manager.allocations->prev = header;
    }
    g_memory_manager.allocations = header;

    // Update statistics
    g_memory_manager.allocation_count++;
    g_memory_manager.total_allocated += size;

    // Update peak memory usage
    if (g_memory_manager.total_allocated > g_memory_manager.peak_allocated)
    {
        g_memory_manager.peak_allocated = g_memory_manager.total_allocated;
    }
}

// Untrack allocation from the linked list
void untrack_allocation(MemoryHeader *header)
{
    // Validate header
    if (header->magic != MEMORY_MAGIC)
    {
        printf("ERROR: Memory corruption detected! Magic number mismatch.\n");
        return;
    }

    // Remove from linked list
    if (header->prev)
    {
        header->prev->next = header->next;
    }
    else
    {
        g_memory_manager.allocations = header->next;
    }

    if (header->next)
    {
        header->next->prev = header->prev;
    }

    // Update statistics
    g_memory_manager.allocation_count--;
    g_memory_manager.total_allocated -= header->size;
}

// Custom memory allocation function
void *memory_alloc(size_t size, const char *file, int line)
{
    g_memory_manager.malloc_calls++;

    // Account for header size
    size_t total_size = size + sizeof(MemoryHeader);

    // Determine block category
    BlockCategory category = get_block_category(total_size);

    void *ptr = NULL;

    // Try to allocate from pool for small sizes
    if (category != BLOCK_LARGE)
    {
        BlockPool *pool = get_pool_for_category(category);
        ptr = allocate_from_pool(pool);
    }

    // Fall back to malloc for large blocks or if pool allocation failed
    if (!ptr)
    {
        ptr = malloc(total_size);
        if (!ptr)
        {
            return NULL;  // Out of memory
        }
    }

    // Set up header
    track_allocation(ptr, size, file, line);

    // Return pointer after header
    return (char *) ptr + sizeof(MemoryHeader);
}

// Return memory to pool or free it
void return_to_pool(void *ptr, BlockCategory category)
{
    // For large blocks, just free them
    if (category == BLOCK_LARGE)
    {
        free(ptr);
        return;
    }

    // For pooled blocks, find the block in the pool and mark it unused
    BlockPool *pool = get_pool_for_category(category);

    for (size_t i = 0; i < pool->count; i++)
    {
        if (pool->blocks[i] == ptr)
        {
            pool->used[i] = false;
            return;
        }
    }

    // If not found in pool, it must be a large block
    free(ptr);
}

// Custom memory free function
void memory_free(void *ptr, const char *file, int line)
{
    if (!ptr)
    {
        return;
    }

    g_memory_manager.free_calls++;

    // Get the header
    MemoryHeader *header = (MemoryHeader *) ((char *) ptr - sizeof(MemoryHeader));

    // Check for memory corruption
    if (header->magic != MEMORY_MAGIC)
    {
        printf("ERROR: Memory corruption detected in free! Magic number mismatch at %s:%d\n", file, line);
        printf("  Original allocation at %s:%d\n", header->file, header->line);
        return;
    }

    // Calculate block category
    BlockCategory category = get_block_category(header->size + sizeof(MemoryHeader));

    // Remove from tracking
    untrack_allocation(header);

    // Clear magic number to detect double-frees
    header->magic = 0;

    // Return to pool or free
    return_to_pool(header, category);
}

// Print memory usage report
void memory_print_report()
{
    printf("\n=== Memory Manager Report ===\n");
    printf("Current memory usage: %zu bytes\n", g_memory_manager.total_allocated);
    printf("Peak memory usage: %zu bytes\n", g_memory_manager.peak_allocated);
    printf("Total allocations: %zu\n", g_memory_manager.malloc_calls);
    printf("Total frees: %zu\n", g_memory_manager.free_calls);
    printf("Pool allocations: %zu\n", g_memory_manager.pool_hits);
    printf("Outstanding allocations: %zu\n", g_memory_manager.allocation_count);

    if (g_memory_manager.allocation_count > 0)
    {
        printf("\nMemory Leaks Detected:\n");
        MemoryHeader *current = g_memory_manager.allocations;
        size_t leak_count = 0;
        size_t total_leaked = 0;

        while (current)
        {
            printf("  Leak #%zu: %zu bytes at %s:%d\n", ++leak_count, current->size, current->file, current->line);
            total_leaked += current->size;
            current = current->next;
        }

        printf("\nTotal leaked memory: %zu bytes\n", total_leaked);
    }
}

// Check for memory leaks
bool memory_check_leaks()
{
    return g_memory_manager.allocation_count > 0;
}

// Clean up memory manager
void memory_manager_cleanup()
{
    // Report leaks
    if (memory_check_leaks())
    {
        memory_print_report();
    }

    // Free all pooled memory
    for (size_t i = 0; i < g_memory_manager.tiny_pool.count; i++)
    {
        free(g_memory_manager.tiny_pool.blocks[i]);
    }

    for (size_t i = 0; i < g_memory_manager.small_pool.count; i++)
    {
        free(g_memory_manager.small_pool.blocks[i]);
    }

    for (size_t i = 0; i < g_memory_manager.medium_pool.count; i++)
    {
        free(g_memory_manager.medium_pool.blocks[i]);
    }

    // Reset memory manager
    memset(&g_memory_manager, 0, sizeof(g_memory_manager));
}

// Macro to simplify allocation
#define MM_ALLOC(size) memory_alloc(size, __FILE__, __LINE__)

// Macro to simplify deallocation
#define MM_FREE(ptr) memory_free(ptr, __FILE__, __LINE__)

// Demo program using the memory manager
void run_memory_manager_demo()
{
    printf("==== MEMORY MANAGER DEMO ====\n\n");

    // Initialize memory manager
    memory_manager_init();

    printf("1. Allocating various sized objects\n");

    // Allocate small objects
    char *tiny_str = (char *) MM_ALLOC(10);
    strcpy(tiny_str, "Tiny");

    int *small_array = (int *) MM_ALLOC(5 * sizeof(int));
    for (int i = 0; i < 5; i++)
    {
        small_array[i] = i;
    }

    double *medium_array = (double *) MM_ALLOC(20 * sizeof(double));
    for (int i = 0; i < 20; i++)
    {
        medium_array[i] = i * 1.5;
    }

    // Allocate a large object
    int *large_array = (int *) MM_ALLOC(1000 * sizeof(int));
    for (int i = 0; i < 1000; i++)
    {
        large_array[i] = i;
    }

    // Print current memory usage
    memory_print_report();

    printf("\n2. Freeing some objects\n");

    // Free some objects
    MM_FREE(tiny_str);
    MM_FREE(medium_array);

    // Print updated memory usage
    memory_print_report();

    printf("\n3. Allocating more objects to demonstrate reuse\n");

    // Allocate more objects (some will reuse pooled memory)
    char *another_tiny = (char *) MM_ALLOC(12);
    strcpy(another_tiny, "Another");

    double *another_medium = (double *) MM_ALLOC(15 * sizeof(double));
    for (int i = 0; i < 15; i++)
    {
        another_medium[i] = i * 2.5;
    }

    // Print memory usage again
    memory_print_report();

    printf("\n4. Intentionally leave some leaks to demonstrate leak detection\n");

    // Free some but not all (to demonstrate leak detection)
    MM_FREE(another_tiny);
    MM_FREE(another_medium);

    // Don't free these to simulate leaks:
    // MM_FREE(small_array);
    // MM_FREE(large_array);

    printf("\n5. Final memory report (should show leaks)\n");

    // Final report and cleanup
    memory_manager_cleanup();
}

int main()
{
    run_memory_manager_demo();
    return 0;
}
