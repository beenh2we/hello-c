/* rtos_example.c - Real-time operating system concepts */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/* Note: This is a conceptual implementation to demonstrate RTOS principles.
   In practice, you would use an actual RTOS like FreeRTOS, Zephyr, or RTX. */

/* ---- Task Management ---- */

#define MAX_TASKS  5
#define STACK_SIZE 128

typedef struct
{
    uint32_t stack[STACK_SIZE];  // Task stack
    uint32_t *sp;                // Stack pointer
    void (*function)(void *);    // Task function
    void *argument;              // Task argument
    uint8_t priority;            // Task priority (0-highest)
    bool ready;                  // Task ready state
} Task;

// Task control block array
static Task tasks[MAX_TASKS];
static uint8_t current_task = 0;
static uint8_t next_task_id = 0;

// Initialize a task
uint8_t task_create(void (*task_func)(void *), void *arg, uint8_t priority)
{
    if (next_task_id >= MAX_TASKS)
    {
        return 0xFF;  // Error: task limit reached
    }

    uint8_t task_id = next_task_id++;

    tasks[task_id].function = task_func;
    tasks[task_id].argument = arg;
    tasks[task_id].priority = priority;
    tasks[task_id].ready = true;

    // Initialize stack (simplified, architecture dependent)
    // In a real implementation, this would set up stack frame for context switching
    tasks[task_id].sp = &tasks[task_id].stack[STACK_SIZE - 16];

    return task_id;
}

// Simple task scheduler (priority-based)
void scheduler(void)
{
    uint8_t highest_priority = 0xFF;
    uint8_t task_to_run = current_task;

    // Find highest priority ready task
    for (uint8_t i = 0; i < next_task_id; i++)
    {
        if (tasks[i].ready && tasks[i].priority < highest_priority)
        {
            highest_priority = tasks[i].priority;
            task_to_run = i;
        }
    }

    if (task_to_run != current_task)
    {
        // Save context of current task and load context of new task
        // This would involve architecture-specific context switching code
        // (registers, stack pointer, program counter, etc.)

        // For demonstration only:
        current_task = task_to_run;
    }
}

/* ---- Timing and Deadlines ---- */

// System tick counter
volatile uint32_t system_ticks = 0;

// Tick interrupt handler
void SysTick_Handler(void)
{
    system_ticks++;

    // Run scheduler on every tick
    scheduler();
}

// Get current system time
uint32_t get_time_ms(void)
{
    return system_ticks;
}

// Sleep for specified number of milliseconds
void task_delay(uint32_t ms)
{
    uint32_t start = get_time_ms();
    while (get_time_ms() - start < ms)
    {
        // In a real RTOS, this would yield to other tasks
        // by triggering context switch
    }
}

/* ---- Synchronization Primitives ---- */

// Mutex implementation
typedef struct
{
    bool locked;
    uint8_t owner;
} Mutex;

// Initialize mutex
void mutex_init(Mutex *mutex)
{
    mutex->locked = false;
    mutex->owner = 0xFF;  // Invalid task ID
}

// Acquire mutex (blocking)
void mutex_acquire(Mutex *mutex)
{
    // Disable interrupts for atomic operation
    // In real code: __disable_irq();

    while (mutex->locked)
    {
        // In a real RTOS, we'd block the task here
        // and re-enable interrupts while waiting

        // For demonstration, just spin (BAD practice!)
    }

    mutex->locked = true;
    mutex->owner = current_task;

    // Re-enable interrupts
    // In real code: __enable_irq();
}

// Release mutex
void mutex_release(Mutex *mutex)
{
    // Disable interrupts for atomic operation
    // In real code: __disable_irq();

    if (mutex->owner == current_task)
    {
        mutex->locked = false;
        mutex->owner = 0xFF;
    }

    // Re-enable interrupts
    // In real code: __enable_irq();
}

/* ---- Example Tasks ---- */

// Example mutex for shared resource
Mutex uart_mutex;

// LED blinking task
void led_task(void *arg)
{
    uint8_t led_pin = *(uint8_t *) arg;

    while (1)
    {
        // Toggle LED
        // GPIO_PORT_A ^= (1 << led_pin);

        mutex_acquire(&uart_mutex);
        // uart_puts("LED toggled\r\n");
        mutex_release(&uart_mutex);

        task_delay(500);  // 500 ms delay
    }
}

// Sensor reading task with deadline
void sensor_task(void *arg)
{
    uint32_t next_execution = 0;
    const uint32_t period = 100;  // 100 ms period

    while (1)
    {
        uint32_t start_time = get_time_ms();

        // Read sensor (simulated with delay)
        uint16_t sensor_value = 123;  // Placeholder value

        // Process sensor data
        // ...

        mutex_acquire(&uart_mutex);
        // uart_puts("Sensor read\r\n");
        mutex_release(&uart_mutex);

        // Check if deadline was missed
        uint32_t execution_time = get_time_ms() - start_time;
        if (execution_time > period)
        {
            // Deadline missed handling
            // uart_puts("WARNING: Deadline missed!\r\n");
        }

        // Calculate next execution time
        next_execution += period;

        // Sleep until next period
        int32_t sleep_time = next_execution - get_time_ms();
        if (sleep_time > 0)
        {
            task_delay(sleep_time);
        }
        else
        {
            // We're behind schedule
            next_execution = get_time_ms();
        }
    }
}

/* ---- Main Function ---- */

int main(void)
{
    // Initialize system
    // setup_clocks();
    // setup_systick();

    // Initialize synchronization primitives
    mutex_init(&uart_mutex);

    // Create tasks
    uint8_t led1_pin = 0;
    uint8_t led2_pin = 1;
    task_create(led_task, &led1_pin, 2);  // Lower priority
    task_create(led_task, &led2_pin, 2);
    task_create(sensor_task, NULL, 1);  // Higher priority

    // Start scheduler
    // start_systick();

    // This point should never be reached if scheduler is running
    while (1)
    {
        // Idle task
        // Could implement power saving here
    }

    return 0;
}
