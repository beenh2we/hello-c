#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

// Demo struct for working with structured data in memory-mapped files
typedef struct
{
    int id;
    char name[32];
    double value;
} Item;

// Function to create a file with test data
int create_items_file(const char *filename, int num_items)
{
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("open");
        return -1;
    }

    // Set the file size
    size_t file_size = num_items * sizeof(Item);
    if (ftruncate(fd, file_size) == -1)
    {
        perror("ftruncate");
        close(fd);
        return -1;
    }

    // Create and initialize items
    Item *items = malloc(file_size);
    if (!items)
    {
        perror("malloc");
        close(fd);
        return -1;
    }

    for (int i = 0; i < num_items; i++)
    {
        items[i].id = i + 1;
        snprintf(items[i].name, sizeof(items[i].name), "Item %d", i + 1);
        items[i].value = (i + 1) * 10.5;
    }

    // Write items to file
    write(fd, items, file_size);
    free(items);

    printf("Created items file %s with %d items (size: %zu bytes)\n", filename, num_items, file_size);
    return fd;
}

// Function to demo read-only memory mapping
void demo_readonly_mapping(const char *filename)
{
    printf("\n=== Read-Only Memory Mapping Demo ===\n");

    // Open the file
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return;
    }

    // Get file size
    struct stat sb;
    if (fstat(fd, &sb) == -1)
    {
        perror("fstat");
        close(fd);
        return;
    }

    // Map the file
    void *addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return;
    }

    // We can close the file descriptor after mapping
    close(fd);

    // Access the mapped data as Item structures
    Item *items = (Item *) addr;
    int num_items = sb.st_size / sizeof(Item);

    printf("Reading %d items from mapped memory:\n", num_items);
    for (int i = 0; i < num_items; i++)
    {
        printf("Item %d: id=%d, name='%s', value=%.2f\n", i, items[i].id, items[i].name, items[i].value);
    }

    // Unmap when done
    if (munmap(addr, sb.st_size) == -1)
    {
        perror("munmap");
    }
}

// Function to demo read-write memory mapping
void demo_readwrite_mapping(const char *filename)
{
    printf("\n=== Read-Write Memory Mapping Demo ===\n");

    // Open the file
    int fd = open(filename, O_RDWR);
    if (fd == -1)
    {
        perror("open");
        return;
    }

    // Get file size
    struct stat sb;
    if (fstat(fd, &sb) == -1)
    {
        perror("fstat");
        close(fd);
        return;
    }

    // Map the file
    void *addr = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return;
    }

    // Close file descriptor after mapping
    close(fd);

    // Access mapped data as Item structures
    Item *items = (Item *) addr;
    int num_items = sb.st_size / sizeof(Item);

    printf("Original data from mapped memory:\n");
    for (int i = 0; i < num_items && i < 2; i++)
    {
        printf("Item %d: id=%d, name='%s', value=%.2f\n", i, items[i].id, items[i].name, items[i].value);
    }

    // Modify the mapped data
    printf("\nModifying mapped data...\n");
    for (int i = 0; i < num_items; i++)
    {
        items[i].id += 100;
        strcat(items[i].name, " (modified)");
        items[i].value *= 2;
    }

    // Sync changes to disk
    if (msync(addr, sb.st_size, MS_SYNC) == -1)
    {
        perror("msync");
    }

    printf("Modified data (now in memory and on disk):\n");
    for (int i = 0; i < num_items && i < 2; i++)
    {
        printf("Item %d: id=%d, name='%s', value=%.2f\n", i, items[i].id, items[i].name, items[i].value);
    }

    // Unmap when done
    if (munmap(addr, sb.st_size) == -1)
    {
        perror("munmap");
    }
}

// Function to demonstrate partial mapping (mapping only a portion of a file)
void demo_partial_mapping(const char *filename)
{
    printf("\n=== Partial File Memory Mapping Demo ===\n");

    // Open the file
    int fd = open(filename, O_RDWR);
    if (fd == -1)
    {
        perror("open");
        return;
    }

    // Get file size
    struct stat sb;
    if (fstat(fd, &sb) == -1)
    {
        perror("fstat");
        close(fd);
        return;
    }

    // Map only the first item
    size_t item_size = sizeof(Item);
    void *addr = mmap(NULL, item_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return;
    }

    // Access only the first item
    Item *item = (Item *) addr;
    printf("First item only: id=%d, name='%s', value=%.2f\n", item->id, item->name, item->value);

    // Map second item separately
    void *addr2 = mmap(NULL, item_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, item_size);
    if (addr2 == MAP_FAILED)
    {
        perror("mmap second item");
        munmap(addr, item_size);
        close(fd);
        return;
    }

    Item *item2 = (Item *) addr2;
    printf("Second item only: id=%d, name='%s', value=%.2f\n", item2->id, item2->name, item2->value);

    // Modify second item
    item2->id += 1000;

    // Sync and clean up
    msync(addr2, item_size, MS_SYNC);
    munmap(addr, item_size);
    munmap(addr2, item_size);
    close(fd);
}

int main()
{
    printf("=== Memory-Mapped Files Demo ===\n");

    // Create test file with items
    const char *filename = "items_mmap.bin";
    int fd = create_items_file(filename, 5);
    if (fd >= 0)
    {
        close(fd);
    }
    else
    {
        fprintf(stderr, "Failed to create test file\n");
        return 1;
    }

    // Run demos
    demo_readonly_mapping(filename);
    demo_readwrite_mapping(filename);
    demo_partial_mapping(filename);

    // Clean up
    unlink(filename);

    return 0;
}
