#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Simple memory pool implementation
#define POOL_SIZE 1024
typedef struct
{
    unsigned char buffer[POOL_SIZE];
    size_t used;
} MemoryPool;

// Initialize memory pool
void pool_init(MemoryPool *pool)
{
    pool->used = 0;
}

// Allocate from memory pool
void *pool_alloc(MemoryPool *pool, size_t size)
{
    // Align to 8 bytes for better memory access
    size_t aligned_size = (size + 7) & ~7;

    if (pool->used + aligned_size > POOL_SIZE)
    {
        return NULL;  // Not enough space
    }

    void *ptr = &pool->buffer[pool->used];
    pool->used += aligned_size;
    return ptr;
}

// Reset memory pool (doesn't actually free memory, just allows reuse)
void pool_reset(MemoryPool *pool)
{
    pool->used = 0;
}

// Simple stack allocator
typedef struct
{
    unsigned char *buffer;
    size_t capacity;
    size_t used;
} StackAllocator;

// Initialize stack allocator
int stack_allocator_init(StackAllocator *stack, size_t capacity)
{
    stack->buffer = (unsigned char *) malloc(capacity);
    if (!stack->buffer) return 0;
    stack->capacity = capacity;
    stack->used = 0;
    return 1;
}

// Allocate from stack
void *stack_alloc(StackAllocator *stack, size_t size)
{
    // Align to 8 bytes
    size_t aligned_size = (size + 7) & ~7;

    if (stack->used + aligned_size > stack->capacity)
    {
        return NULL;  // Not enough space
    }

    void *ptr = &stack->buffer[stack->used];
    stack->used += aligned_size;
    return ptr;
}

// Create a marker for the current position
size_t stack_get_marker(StackAllocator *stack)
{
    return stack->used;
}

// Roll back to a previous marker
void stack_free_to_marker(StackAllocator *stack, size_t marker)
{
    if (marker <= stack->used)
    {
        stack->used = marker;
    }
}

// Free the entire stack
void stack_allocator_destroy(StackAllocator *stack)
{
    free(stack->buffer);
    stack->buffer = NULL;
    stack->capacity = 0;
    stack->used = 0;
}

// Block allocator for fixed-size objects
typedef struct BlockNode
{
    struct BlockNode *next;
} BlockNode;

typedef struct
{
    size_t block_size;
    size_t blocks_per_chunk;
    BlockNode *free_list;
    void **chunks;
    size_t chunk_count;
    size_t max_chunks;
} BlockAllocator;

// Initialize block allocator
int block_allocator_init(BlockAllocator *alloc,
                         size_t block_size,
                         size_t blocks_per_chunk,
                         size_t max_chunks)
{
    if (block_size < sizeof(BlockNode))
    {
        block_size = sizeof(BlockNode);  // Minimum size
    }

    alloc->block_size = block_size;
    alloc->blocks_per_chunk = blocks_per_chunk;
    alloc->free_list = NULL;
    alloc->chunk_count = 0;
    alloc->max_chunks = max_chunks;

    alloc->chunks = (void **) malloc(max_chunks * sizeof(void *));
    if (!alloc->chunks) return 0;

    return 1;
}

// Allocate a new chunk of memory
int block_allocator_add_chunk(BlockAllocator *alloc)
{
    if (alloc->chunk_count >= alloc->max_chunks)
    {
        return 0;  // Too many chunks
    }

    // Allocate a new chunk
    void *chunk = malloc(alloc->block_size * alloc->blocks_per_chunk);
    if (!chunk) return 0;

    // Add to chunks array
    alloc->chunks[alloc->chunk_count++] = chunk;

    // Add all blocks to free list
    char *block = (char *) chunk;
    for (size_t i = 0; i < alloc->blocks_per_chunk; i++)
    {
        BlockNode *node = (BlockNode *) block;
        node->next = alloc->free_list;
        alloc->free_list = node;
        block += alloc->block_size;
    }

    return 1;
}

// Allocate a block
void *block_alloc(BlockAllocator *alloc)
{
    // If no free blocks, allocate a new chunk
    if (!alloc->free_list)
    {
        if (!block_allocator_add_chunk(alloc))
        {
            return NULL;
        }
    }

    // Take a block from the free list
    BlockNode *node = alloc->free_list;
    alloc->free_list = node->next;

    return node;
}

// Free a block
void block_free(BlockAllocator *alloc, void *ptr)
{
    if (!ptr) return;

    // Add back to free list
    BlockNode *node = (BlockNode *) ptr;
    node->next = alloc->free_list;
    alloc->free_list = node;
}

// Destroy block allocator
void block_allocator_destroy(BlockAllocator *alloc)
{
    // Free all chunks
    for (size_t i = 0; i < alloc->chunk_count; i++)
    {
        free(alloc->chunks[i]);
    }

    free(alloc->chunks);
    alloc->chunks = NULL;
    alloc->free_list = NULL;
    alloc->chunk_count = 0;
}

// Function to demonstrate memory pool usage
void memory_pool_example()
{
    printf("=== Memory Pool Example ===\n");

    MemoryPool pool;
    pool_init(&pool);

    // Allocate some objects from the pool
    int *int_array = (int *) pool_alloc(&pool, 5 * sizeof(int));
    char *string = (char *) pool_alloc(&pool, 20);
    float *float_array = (float *) pool_alloc(&pool, 3 * sizeof(float));

    if (!int_array || !string || !float_array)
    {
        printf("Memory pool allocation failed\n");
        return;
    }

    // Use the allocated memory
    for (int i = 0; i < 5; i++)
    {
        int_array[i] = i * 10;
    }

    strcpy(string, "Memory pool string");

    float_array[0] = 1.1f;
    float_array[1] = 2.2f;
    float_array[2] = 3.3f;

    // Print values to show memory is usable
    printf("Int array from pool: ");
    for (int i = 0; i < 5; i++)
    {
        printf("%d ", int_array[i]);
    }
    printf("\n");

    printf("String from pool: %s\n", string);

    printf("Float array from pool: ");
    for (int i = 0; i < 3; i++)
    {
        printf("%.1f ", float_array[i]);
    }
    printf("\n");

    printf("Pool usage: %zu/%u bytes\n", pool.used, POOL_SIZE);

    // Reset the pool (note: doesn't actually free memory)
    pool_reset(&pool);
    printf("Pool reset, usage: %zu/%u bytes\n", pool.used, POOL_SIZE);

    // Reuse the pool
    int *new_array = (int *) pool_alloc(&pool, 3 * sizeof(int));
    if (new_array)
    {
        new_array[0] = 100;
        new_array[1] = 200;
        new_array[2] = 300;
        printf("New array after reset: %d %d %d\n",
               new_array[0],
               new_array[1],
               new_array[2]);
    }
}

// Function to demonstrate stack allocator usage
void stack_allocator_example(void)
{
    printf("\n=== Stack Allocator Example ===\n");

    StackAllocator stack;
    if (!stack_allocator_init(&stack, 1024))
    {
        printf("Failed to initialize stack allocator\n");
        return;
    }

    // Allocate some memory
    int *numbers = (int *) stack_alloc(&stack, 4 * sizeof(int));
    if (numbers)
    {
        numbers[0] = 10;
        numbers[1] = 20;
        numbers[2] = 30;
        numbers[3] = 40;

        printf("Numbers: %d %d %d %d\n",
               numbers[0],
               numbers[1],
               numbers[2],
               numbers[3]);
    }

    // Save current position
    size_t marker = stack_get_marker(&stack);
    printf("Current stack usage: %zu bytes\n", marker);

    // Allocate more memory
    char *text = (char *) stack_alloc(&stack, 50);
    if (text)
    {
        strcpy(text, "This is a stack allocator example");
        printf("Text: %s\n", text);
        printf("Current stack usage: %zu bytes\n", stack.used);
    }

    // Roll back to the marker (effectively freeing the text buffer)
    stack_free_to_marker(&stack, marker);
    printf("After rollback to marker, stack usage: %zu bytes\n", stack.used);

    // Clean up
    stack_allocator_destroy(&stack);
}

// Function to demonstrate block allocator usage
void block_allocator_example(void)
{
    printf("\n=== Block Allocator Example ===\n");

    // Create a block allocator for a specific struct
    typedef struct
    {
        int id;
        char name[20];
    } Entity;

    BlockAllocator entity_allocator;
    if (!block_allocator_init(&entity_allocator, sizeof(Entity), 10, 10))
    {
        printf("Failed to initialize block allocator\n");
        return;
    }

    // Allocate some entities
    Entity *entities[15];
    int count = 0;

    printf("Allocating 15 entities from block allocator...\n");
    for (int i = 0; i < 15; i++)
    {
        entities[i] = (Entity *) block_alloc(&entity_allocator);
        if (entities[i])
        {
            entities[i]->id = i + 1000;
            sprintf(entities[i]->name, "Entity %d", i);
            count++;
        }
    }

    printf("Successfully allocated %d entities\n", count);

    // Print some entities
    for (int i = 0; i < 5 && i < count; i++)
    {
        printf("Entity %d: id=%d, name=%s\n",
               i,
               entities[i]->id,
               entities[i]->name);
    }

    // Free every other entity to create "holes"
    printf("\nFreeing every other entity...\n");
    for (int i = 0; i < count; i += 2)
    {
        block_free(&entity_allocator, entities[i]);
        entities[i] = NULL;
    }

    // Allocate some more to show reuse
    printf("Allocating more entities to show block reuse...\n");
    int new_count = 0;
    for (int i = 0; i < 5; i++)
    {
        Entity *e = (Entity *) block_alloc(&entity_allocator);
        if (e)
        {
            e->id = 2000 + i;
            sprintf(e->name, "New Entity %d", i);
            new_count++;

            printf("New entity: id=%d, name=%s\n", e->id, e->name);
        }
    }

    printf("Successfully allocated %d new entities\n", new_count);

    // Clean up
    block_allocator_destroy(&entity_allocator);
}

// Function to benchmark different allocation methods
void benchmark_allocators(void)
{
    printf("\n=== Memory Allocator Benchmark ===\n");

    const int num_allocations = 100000;
    const size_t alloc_size = 32;  // Small allocations
    const int iterations = 5;

    clock_t start, end;
    double malloc_time = 0, pool_time = 0, stack_time = 0, block_time = 0;

    printf("Benchmarking %d allocations of %zu bytes...\n",
           num_allocations,
           alloc_size);

    // Average over iterations
    for (int iter = 0; iter < iterations; iter++)
    {
        // 1. Standard malloc/free
        void *ptrs[num_allocations];

        start = clock();
        for (int i = 0; i < num_allocations; i++)
        {
            ptrs[i] = malloc(alloc_size);
        }
        for (int i = 0; i < num_allocations; i++)
        {
            free(ptrs[i]);
        }
        end = clock();
        malloc_time += ((double) (end - start)) / CLOCKS_PER_SEC;

        // 2. Memory Pool
        MemoryPool pool;
        pool_init(&pool);

        start = clock();
        for (int i = 0; i < num_allocations && i * alloc_size < POOL_SIZE; i++)
        {
            pool_alloc(&pool, alloc_size);
        }
        pool_reset(&pool);  // Reset instead of individual frees
        end = clock();
        pool_time += ((double) (end - start)) / CLOCKS_PER_SEC;

        // 3. Stack Allocator
        StackAllocator stack;
        if (stack_allocator_init(&stack, num_allocations * alloc_size))
        {
            start = clock();
            for (int i = 0; i < num_allocations; i++)
            {
                stack_alloc(&stack, alloc_size);
            }
            stack_allocator_destroy(&stack);
            end = clock();
            stack_time += ((double) (end - start)) / CLOCKS_PER_SEC;
        }

        // 4. Block Allocator
        BlockAllocator block;
        if (block_allocator_init(&block, alloc_size, num_allocations / 10, 10))
        {
            start = clock();
            for (int i = 0; i < num_allocations; i++)
            {
                void *ptr = block_alloc(&block);
                if (!ptr) break;
            }
            block_allocator_destroy(&block);
            end = clock();
            block_time += ((double) (end - start)) / CLOCKS_PER_SEC;
        }
    }

    // Average the results
    malloc_time /= iterations;
    pool_time /= iterations;
    stack_time /= iterations;
    block_time /= iterations;

    printf("\nAverage times over %d iterations:\n", iterations);
    printf("malloc/free:      %.6f seconds\n", malloc_time);
    printf("Memory Pool:      %.6f seconds (%.2fx vs malloc)\n",
           pool_time,
           malloc_time / pool_time);
    printf("Stack Allocator:  %.6f seconds (%.2fx vs malloc)\n",
           stack_time,
           malloc_time / stack_time);
    printf("Block Allocator:  %.6f seconds (%.2fx vs malloc)\n",
           block_time,
           malloc_time / block_time);
}

int main(void)
{
    printf("==== CUSTOM MEMORY MANAGEMENT ====\n\n");

    memory_pool_example();
    stack_allocator_example();
    block_allocator_example();
    benchmark_allocators();

    return 0;
}
