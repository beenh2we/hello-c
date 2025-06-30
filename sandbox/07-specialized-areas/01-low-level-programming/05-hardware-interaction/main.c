#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// === Simulated Hardware Device ===

// LED Controller Register Set
typedef struct
{
    volatile uint32_t CONTROL;    // Control register
    volatile uint32_t STATUS;     // Status register
    volatile uint32_t DATA;       // Data register (LED patterns)
    volatile uint32_t INTENSITY;  // LED intensity
} LED_Controller;

// ADC (Analog-to-Digital Converter) Register Set
typedef struct
{
    volatile uint32_t CONTROL;     // Control register
    volatile uint32_t STATUS;      // Status register
    volatile uint32_t DATA;        // Conversion result
    volatile uint32_t CHANNEL;     // Channel selection
    volatile uint32_t SAMPLERATE;  // Sample rate
    volatile uint32_t INTERRUPT;   // Interrupt control
} ADC_Controller;

// Timer Register Set
typedef struct
{
    volatile uint32_t CONTROL;    // Control register
    volatile uint32_t STATUS;     // Status register
    volatile uint32_t COUNTER;    // Counter value
    volatile uint32_t COMPARE;    // Compare value
    volatile uint32_t PRESCALER;  // Clock prescaler
    volatile uint32_t INTERRUPT;  // Interrupt control
} Timer_Controller;

// Full Device Memory Map
typedef struct
{
    LED_Controller LED;
    ADC_Controller ADC;
    Timer_Controller TIMER;
    volatile uint32_t GLOBAL_STATUS;   // Global status register
    volatile uint32_t GLOBAL_CONTROL;  // Global control register
} Device_Registers;

// Global device registers (simulating memory-mapped hardware)
Device_Registers* device;

// Thread for simulating hardware behavior
pthread_t hardware_thread;

// Flag to stop the hardware simulation
volatile int stop_simulation = 0;

// === Control Bit Definitions ===

// LED Controller
#define LED_CTRL_ENABLE  (1 << 0)  // Enable LED controller
#define LED_CTRL_BLINK   (1 << 1)  // Enable blinking
#define LED_CTRL_PATTERN (1 << 2)  // Enable pattern mode
#define LED_CTRL_RESET   (1 << 7)  // Reset controller

// LED Status
#define LED_STATUS_ENABLED (1 << 0)  // Controller is enabled
#define LED_STATUS_BUSY    (1 << 1)  // Controller is busy
#define LED_STATUS_ERROR   (1 << 2)  // Error condition

// ADC Controller
#define ADC_CTRL_ENABLE     (1 << 0)  // Enable ADC
#define ADC_CTRL_START      (1 << 1)  // Start conversion
#define ADC_CTRL_CONTINUOUS (1 << 2)  // Continuous conversion mode
#define ADC_CTRL_RESET      (1 << 7)  // Reset controller

// ADC Status
#define ADC_STATUS_ENABLED  (1 << 0)  // ADC is enabled
#define ADC_STATUS_BUSY     (1 << 1)  // Conversion in progress
#define ADC_STATUS_DONE     (1 << 2)  // Conversion complete
#define ADC_STATUS_OVERFLOW (1 << 3)  // Overflow occurred

// ADC Interrupt
#define ADC_INT_ENABLE   (1 << 0)  // Enable interrupts
#define ADC_INT_DONE     (1 << 1)  // Conversion done interrupt
#define ADC_INT_OVERFLOW (1 << 2)  // Overflow interrupt

// Timer Controller
#define TIMER_CTRL_ENABLE  (1 << 0)  // Enable timer
#define TIMER_CTRL_ONESHOT (1 << 1)  // One-shot mode
#define TIMER_CTRL_RELOAD  (1 << 2)  // Auto-reload mode
#define TIMER_CTRL_RESET   (1 << 7)  // Reset timer

// Timer Status
#define TIMER_STATUS_ENABLED (1 << 0)  // Timer is enabled
#define TIMER_STATUS_RUNNING (1 << 1)  // Timer is running
#define TIMER_STATUS_EXPIRED (1 << 2)  // Timer expired
#define TIMER_STATUS_COMPARE (1 << 3)  // Compare match occurred

// Timer Interrupt
#define TIMER_INT_ENABLE  (1 << 0)  // Enable interrupts
#define TIMER_INT_EXPIRED (1 << 1)  // Timer expired interrupt
#define TIMER_INT_COMPARE (1 << 2)  // Compare match interrupt

// Global registers
#define GLOBAL_STATUS_POWER (1 << 0)  // Power status
#define GLOBAL_STATUS_ERROR (1 << 1)  // Global error indicator
#define GLOBAL_STATUS_INT   (1 << 7)  // Interrupt pending

// Print binary representation of a value
void print_binary(uint32_t value, int bits)
{
    printf("0b");
    for (int i = bits - 1; i >= 0; i--)
    {
        printf("%d", (value >> i) & 1);
        if (i % 4 == 0 && i > 0) printf("_");
    }
}

// Simulated hardware thread function
void* hardware_simulation(void* arg)
{
    // Initial setup
    device->LED.STATUS = LED_STATUS_ENABLED;
    device->ADC.STATUS = ADC_STATUS_ENABLED;
    device->TIMER.STATUS = TIMER_STATUS_ENABLED;
    device->GLOBAL_STATUS = GLOBAL_STATUS_POWER;

    // Timer and LED blink counters
    int timer_counter = 0;
    int blink_counter = 0;

    while (!stop_simulation)
    {
        // Simulate LED controller
        if (device->LED.CONTROL & LED_CTRL_ENABLE)
        {
            device->LED.STATUS = LED_STATUS_ENABLED;

            // Simulate LED blinking if enabled
            if (device->LED.CONTROL & LED_CTRL_BLINK)
            {
                blink_counter++;
                if (blink_counter >= 5)
                {
                    blink_counter = 0;
                    device->LED.DATA ^= 0xFF;  // Toggle all LEDs
                }
            }
        }
        else
        {
            device->LED.STATUS &= ~LED_STATUS_ENABLED;
            device->LED.DATA = 0;  // All LEDs off
        }

        // Simulate ADC
        if (device->ADC.CONTROL & ADC_CTRL_ENABLE)
        {
            device->ADC.STATUS = ADC_STATUS_ENABLED;

            if (device->ADC.CONTROL & ADC_CTRL_START)
            {
                // Start conversion
                device->ADC.STATUS |= ADC_STATUS_BUSY;
                device->ADC.STATUS &= ~ADC_STATUS_DONE;

                // Simulate conversion delay
                usleep(10000);

                // Complete the conversion
                device->ADC.STATUS &= ~ADC_STATUS_BUSY;
                device->ADC.STATUS |= ADC_STATUS_DONE;

                // Generate "analog" data based on channel
                switch (device->ADC.CHANNEL & 0x07)
                {
                case 0:
                    device->ADC.DATA = rand() % 100;  // Random 0-99
                    break;
                case 1:
                    device->ADC.DATA = 512 + (rand() % 100 - 50);  // ~512 ±50
                    break;
                case 2:
                    device->ADC.DATA = 1023;  // Full scale
                    break;
                case 3:
                    device->ADC.DATA = 0;  // Zero
                    break;
                default:
                    device->ADC.DATA = rand() % 1024;  // Full random 0-1023
                }

                // Generate interrupt if enabled
                if (device->ADC.INTERRUPT & ADC_INT_ENABLE
                    && device->ADC.INTERRUPT & ADC_INT_DONE)
                {
                    device->GLOBAL_STATUS |= GLOBAL_STATUS_INT;
                }

                // In one-shot mode, clear start bit
                if (!(device->ADC.CONTROL & ADC_CTRL_CONTINUOUS))
                {
                    device->ADC.CONTROL &= ~ADC_CTRL_START;
                }
            }
        }
        else
        {
            device->ADC.STATUS &= ~ADC_STATUS_ENABLED;
        }

        // Simulate Timer
        if (device->TIMER.CONTROL & TIMER_CTRL_ENABLE)
        {
            device->TIMER.STATUS = TIMER_STATUS_ENABLED | TIMER_STATUS_RUNNING;

            // Update counter
            timer_counter++;
            if (timer_counter >= 10)
            {
                timer_counter = 0;
                device->TIMER.COUNTER++;

                // Check for compare match
                if (device->TIMER.COUNTER == device->TIMER.COMPARE)
                {
                    device->TIMER.STATUS |= TIMER_STATUS_COMPARE;

                    // Generate interrupt if enabled
                    if (device->TIMER.INTERRUPT & TIMER_INT_ENABLE
                        && device->TIMER.INTERRUPT & TIMER_INT_COMPARE)
                    {
                        device->GLOBAL_STATUS |= GLOBAL_STATUS_INT;
                    }
                }

                // Check for expiration (counter overflow)
                if (device->TIMER.COUNTER == 0xFFFFFFFF)
                {
                    device->TIMER.STATUS |= TIMER_STATUS_EXPIRED;

                    // Generate interrupt if enabled
                    if (device->TIMER.INTERRUPT & TIMER_INT_ENABLE
                        && device->TIMER.INTERRUPT & TIMER_INT_EXPIRED)
                    {
                        device->GLOBAL_STATUS |= GLOBAL_STATUS_INT;
                    }

                    // Reset counter in reload mode
                    if (device->TIMER.CONTROL & TIMER_CTRL_RELOAD)
                    {
                        device->TIMER.COUNTER = 0;
                    }
                    else if (device->TIMER.CONTROL & TIMER_CTRL_ONESHOT)
                    {
                        // Disable timer in one-shot mode
                        device->TIMER.CONTROL &= ~TIMER_CTRL_ENABLE;
                        device->TIMER.STATUS &= ~TIMER_STATUS_RUNNING;
                    }
                }
            }
        }
        else
        {
            device->TIMER.STATUS
                &= ~(TIMER_STATUS_ENABLED | TIMER_STATUS_RUNNING);
        }

        // Short delay to simulate hardware timing
        usleep(20000);
    }

    return NULL;
}

// --- LED Controller Functions ---

// Initialize the LED controller
void led_init()
{
    // Reset the controller
    device->LED.CONTROL = LED_CTRL_RESET;
    usleep(50000);  // Wait for reset to complete

    // Configure LED controller
    device->LED.CONTROL = 0;
    device->LED.DATA = 0;         // All LEDs off
    device->LED.INTENSITY = 128;  // Medium intensity

    // Enable the controller
    device->LED.CONTROL = LED_CTRL_ENABLE;

    printf("LED Controller initialized. Status: 0x%08X\n", device->LED.STATUS);
}

// Set LED pattern
void led_set_pattern(uint32_t pattern)
{
    device->LED.DATA = pattern & 0xFF;

    printf("LED pattern set to 0x%02X\n", pattern & 0xFF);
}

// Enable or disable LED blinking
void led_set_blink(int enable)
{
    if (enable)
    {
        device->LED.CONTROL |= LED_CTRL_BLINK;
    }
    else
    {
        device->LED.CONTROL &= ~LED_CTRL_BLINK;
    }

    printf("LED blinking %s\n", enable ? "enabled" : "disabled");
}

// --- ADC Functions ---

// Initialize the ADC
void adc_init()
{
    // Reset the ADC
    device->ADC.CONTROL = ADC_CTRL_RESET;
    usleep(50000);  // Wait for reset to complete

    // Configure ADC
    device->ADC.CONTROL = 0;
    device->ADC.CHANNEL = 0;
    device->ADC.SAMPLERATE = 1000;  // 1kHz sample rate
    device->ADC.INTERRUPT = 0;

    // Enable the ADC
    device->ADC.CONTROL = ADC_CTRL_ENABLE;

    printf("ADC initialized. Status: 0x%08X\n", device->ADC.STATUS);
}

// Read a value from the ADC
uint32_t adc_read(uint32_t channel)
{
    // Set channel
    device->ADC.CHANNEL = channel & 0x07;

    // Start conversion
    device->ADC.CONTROL |= ADC_CTRL_START;

    // Wait for conversion to complete
    while (!(device->ADC.STATUS & ADC_STATUS_DONE))
    {
        usleep(1000);
    }

    // Read and return the result
    uint32_t result = device->ADC.DATA;
    printf("ADC channel %u value: %u\n", channel, result);

    return result;
}

// --- Timer Functions ---

// Initialize the timer
void timer_init()
{
    // Reset the timer
    device->TIMER.CONTROL = TIMER_CTRL_RESET;
    usleep(50000);  // Wait for reset to complete

    // Configure timer
    device->TIMER.CONTROL = 0;
    device->TIMER.COUNTER = 0;
    device->TIMER.COMPARE = 10;
    device->TIMER.PRESCALER = 1;
    device->TIMER.INTERRUPT = 0;

    printf("Timer initialized. Status: 0x%08X\n", device->TIMER.STATUS);
}

// Start the timer
void timer_start(int mode)
{
    // Configure mode
    device->TIMER.CONTROL = TIMER_CTRL_ENABLE;
    if (mode == 1)
    {
        device->TIMER.CONTROL |= TIMER_CTRL_ONESHOT;
    }
    else if (mode == 2)
    {
        device->TIMER.CONTROL |= TIMER_CTRL_RELOAD;
    }

    printf("Timer started (mode: %d). Status: 0x%08X\n",
           mode,
           device->TIMER.STATUS);
}

// Wait for timer to expire
void timer_wait_expire()
{
    printf("Waiting for timer to expire...\n");

    while (!(device->TIMER.STATUS & TIMER_STATUS_EXPIRED))
    {
        printf("Timer Counter: %u\r", device->TIMER.COUNTER);
        fflush(stdout);
        usleep(100000);
    }

    printf("\nTimer expired! Status: 0x%08X\n", device->TIMER.STATUS);

    // Clear expired flag
    device->TIMER.STATUS &= ~TIMER_STATUS_EXPIRED;
}

// --- Global Device Control ---

// Check for and handle interrupts
void check_interrupts()
{
    if (device->GLOBAL_STATUS & GLOBAL_STATUS_INT)
    {
        printf("*** INTERRUPT DETECTED ***\n");

        // Check ADC interrupts
        if (device->ADC.INTERRUPT & ADC_INT_ENABLE)
        {
            if (device->ADC.STATUS & ADC_STATUS_DONE
                && device->ADC.INTERRUPT & ADC_INT_DONE)
            {
                printf("  ADC conversion complete interrupt\n");
                // Clear the interrupt
                device->ADC.STATUS &= ~ADC_STATUS_DONE;
            }

            if (device->ADC.STATUS & ADC_STATUS_OVERFLOW
                && device->ADC.INTERRUPT & ADC_INT_OVERFLOW)
            {
                printf("  ADC overflow interrupt\n");
                // Clear the interrupt
                device->ADC.STATUS &= ~ADC_STATUS_OVERFLOW;
            }
        }

        // Check Timer interrupts
        if (device->TIMER.INTERRUPT & TIMER_INT_ENABLE)
        {
            if (device->TIMER.STATUS & TIMER_STATUS_EXPIRED
                && device->TIMER.INTERRUPT & TIMER_INT_EXPIRED)
            {
                printf("  Timer expired interrupt\n");
                // Clear the interrupt
                device->TIMER.STATUS &= ~TIMER_STATUS_EXPIRED;
            }

            if (device->TIMER.STATUS & TIMER_STATUS_COMPARE
                && device->TIMER.INTERRUPT & TIMER_INT_COMPARE)
            {
                printf("  Timer compare match interrupt\n");
                // Clear the interrupt
                device->TIMER.STATUS &= ~TIMER_STATUS_COMPARE;
            }
        }

        // Clear the global interrupt flag
        device->GLOBAL_STATUS &= ~GLOBAL_STATUS_INT;
    }
}

// Demonstrate a complete example using all devices
void run_demo()
{
    printf("=== Running Hardware Demo ===\n");

    // Initialize all controllers
    led_init();
    adc_init();
    timer_init();

    // Set up interrupts
    printf("Enabling ADC and Timer interrupts\n");
    device->ADC.INTERRUPT = ADC_INT_ENABLE | ADC_INT_DONE;
    device->TIMER.INTERRUPT = TIMER_INT_ENABLE | TIMER_INT_COMPARE;

    // Set up a LED pattern that alternates
    printf("Starting LED blinking\n");
    led_set_pattern(0xAA);  // 10101010
    led_set_blink(1);

    // Configure timer for auto-reload
    device->TIMER.COMPARE = 5;  // Compare value
    timer_start(2);             // Auto-reload mode

    // Main loop to demonstrate continuous operation
    printf("Running main loop (3 iterations)\n");

    for (int i = 0; i < 3; i++)
    {
        printf("\nIteration %d:\n", i + 1);

        // Read from each ADC channel
        for (int ch = 0; ch < 4; ch++)
        {
            uint32_t value = adc_read(ch);

            // Short delay
            usleep(100000);

            // Check for interrupts
            check_interrupts();
        }

        // Read timer
        printf("Timer Counter: %u\n", device->TIMER.COUNTER);

        // Read LED state
        printf("LED Pattern: 0x%02X\n", device->LED.DATA & 0xFF);

        // Check for interrupts
        check_interrupts();

        // Delay between iterations
        usleep(500000);
    }

    // Clean up
    printf("Stopping devices\n");
    device->LED.CONTROL = 0;
    device->ADC.CONTROL = 0;
    device->TIMER.CONTROL = 0;

    printf("Demo completed\n");
}

int main()
{
    printf("==== HARDWARE INTERACTION DEMONSTRATION ====\n");

    // Initialize random seed
    srand(time(NULL));

    // Allocate memory for our simulated device
    device = (Device_Registers*) malloc(sizeof(Device_Registers));
    memset(device, 0, sizeof(Device_Registers));

    // Start the hardware simulation thread
    pthread_create(&hardware_thread, NULL, hardware_simulation, NULL);

    // Wait a moment for the simulation to start
    usleep(100000);

    // Run the demo
    run_demo();

    // Clean up
    printf("Cleaning up resources\n");
    stop_simulation = 1;
    pthread_join(hardware_thread, NULL);
    free(device);

    return 0;
}
