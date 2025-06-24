#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// Simulated hardware device registers
typedef struct
{
    volatile uint32_t STATUS;     // Status register
    volatile uint32_t CONTROL;    // Control register
    volatile uint32_t DATA;       // Data register
    volatile uint32_t INTERRUPT;  // Interrupt register
} Device_Registers;

// Global device registers (simulating memory-mapped hardware)
Device_Registers* device;

// Thread for simulating hardware behavior
pthread_t hardware_thread;

// Flag to stop the hardware simulation
volatile int stop_simulation = 0;

// Simulated hardware thread function
void* hardware_simulation(void* arg)
{
    while (!stop_simulation)
    {
        // Simulate random hardware status changes
        device->STATUS = (device->STATUS & 0xFFFFFF00) | (rand() % 5);  // Random status 0-4

        // Simulate data changing
        if (device->CONTROL & 0x01)
        {                                             // If device is enabled
            device->DATA = (device->DATA + 1) % 100;  // Increment data
        }

        // Simulate interrupt generation
        if ((device->CONTROL & 0x02) && (rand() % 10 == 0))
        {  // 10% chance of interrupt if enabled
            device->INTERRUPT = 1;
        }

        usleep(100000);  // Sleep for 100ms
    }

    return NULL;
}

// Wait for a specific device status
void wait_for_status(uint32_t status)
{
    printf("Waiting for device status %u...\n", status);

    // Non-volatile approach (might be optimized out!)
    uint32_t current_status = device->STATUS;
    while (current_status != status)
    {
        // This loop might be optimized away by some compilers
        // without the volatile keyword on the device registers!
        current_status = device->STATUS;
    }

    printf("Device reached status %u\n", status);
}

// Poll device until ready (status == 0)
void wait_for_device_ready()
{
    printf("Waiting for device to be ready...\n");

    // Since device->STATUS is volatile, this loop won't be optimized out
    while ((device->STATUS & 0xFF) != 0)
    {
        // The compiler knows the value might change between reads
        usleep(10000);  // Small delay
    }

    printf("Device is ready (status 0)\n");
}

// Example of potential optimization issues without volatile
void demonstrate_optimization_issue()
{
    printf("\n=== Demonstrating Optimization Issues ===\n");

    // This is a simplified example to show the concept
    // In reality, compiler behavior is more complex

    // Assume we have a non-volatile status register
    uint32_t non_volatile_status = 1;

    printf("Waiting on non-volatile register...\n");

    // A compiler might optimize this to an infinite loop
    // or remove it entirely if it doesn't see how non_volatile_status could change
    int counter = 0;
    while (non_volatile_status != 0 && counter < 5)
    {
        printf("  Status is still %u, waiting...\n", non_volatile_status);
        non_volatile_status =
            device->STATUS & 0xFF;  // This actually changes it, but compiler might not realize
        counter++;
    }

    printf("With optimizations, the compiler might not realize the value can change!\n");
}

// Read and process data with interrupt handling
void process_data_with_interrupts()
{
    printf("\n=== Processing Data with Interrupts ===\n");

    // Clear any pending interrupt
    device->INTERRUPT = 0;

    // Enable the device and interrupts
    device->CONTROL = 0x03;  // Bits 0 and 1 set (enable device and interrupts)

    printf("Device enabled, processing data...\n");

    // Process 5 data points or until interrupted
    for (int i = 0; i < 5; i++)
    {
        printf("Reading data: %u\n", device->DATA);

        // Check for interrupt
        if (device->INTERRUPT)
        {
            printf("Interrupt detected!\n");
            device->INTERRUPT = 0;  // Clear interrupt
            break;
        }

        usleep(200000);  // Wait 200ms
    }

    // Disable device
    device->CONTROL = 0;
    printf("Device disabled\n");
}

// Explain volatile keyword
void explain_volatile()
{
    printf("\n=== Volatile Keyword Explained ===\n");

    printf("The 'volatile' keyword tells the compiler that a variable's value\n");
    printf("might change at any time, without any action by the code.\n\n");

    printf("Common uses of volatile:\n");
    printf("1. Memory-mapped hardware registers\n");
    printf("2. Variables shared between threads or with signal handlers\n");
    printf("3. Variables modified by external hardware (DMA, interrupts)\n");
    printf("4. Global variables updated from the debugger\n\n");

    printf("Effects of volatile:\n");
    printf("1. Prevents compiler from optimizing out reads/writes\n");
    printf("2. Prevents register caching of the variable\n");
    printf("3. Maintains order of operations relative to volatile accesses\n");
    printf("4. Forces each access to actually go to memory\n\n");

    printf("Volatile is NOT:\n");
    printf("1. NOT related to thread synchronization (doesn't provide atomicity)\n");
    printf("2. NOT related to memory barriers or memory ordering\n");
    printf("3. NOT a substitute for proper synchronization primitives\n");
}

int main()
{
    printf("=== VOLATILE KEYWORD DEMONSTRATION ===\n\n");

    // Initialize random seed
    srand(time(NULL));

    // Allocate memory for our simulated device
    device = (Device_Registers*) malloc(sizeof(Device_Registers));
    device->STATUS = 1;     // Initial status: busy
    device->CONTROL = 0;    // Initial control: disabled
    device->DATA = 0;       // Initial data: 0
    device->INTERRUPT = 0;  // No interrupts pending

    // Start the hardware simulation thread
    pthread_create(&hardware_thread, NULL, hardware_simulation, NULL);

    // Wait a moment for the simulation to start
    usleep(100000);

    // Wait for the device to become ready
    wait_for_device_ready();

    // Show potential issues with optimization
    demonstrate_optimization_issue();

    // Process data with interrupt handling
    process_data_with_interrupts();

    // Example: Directly manipulate device registers
    printf("\n=== Direct Register Manipulation ===\n");

    printf("Writing to control register...\n");
    device->CONTROL = 0x05;  // Enable device and set bit 2

    printf("Reading status register: 0x%08X\n", device->STATUS);

    // Example: Busy-wait polling
    printf("\n=== Busy-Wait Polling ===\n");

    printf("Waiting for specific data value...\n");
    while (device->DATA != 42)
    {
        // Without volatile, this might be optimized to an infinite loop
        if (device->STATUS == 3)
        {
            printf("Device is in error state, resetting...\n");
            device->CONTROL = 0x80;  // Reset command
            usleep(50000);
            device->CONTROL = 0x01;  // Enable again
        }
        usleep(50000);  // Small delay

        // Break after a few seconds to avoid hanging the demo
        static int timeout = 0;
        if (++timeout > 10)
        {
            printf("Timeout waiting for data value\n");
            break;
        }
    }

    // Theoretical explanation
    explain_volatile();

    // Clean up
    printf("\n=== Cleaning Up ===\n");
    stop_simulation = 1;
    pthread_join(hardware_thread, NULL);
    free(device);

    return 0;
}
