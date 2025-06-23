/* embedded_basics.c - Fundamental concepts for embedded C programming */

// Standard headers are often limited or unavailable in embedded systems
// But we include them here for educational purposes
#include <stdint.h>  // For fixed-width integers (crucial in embedded)
#include <stdbool.h> // For boolean types

/* ---- Memory Management in Constrained Environments ---- */

// Use fixed-width integers to ensure consistent size across platforms
uint8_t  sensor_reading;    // Exactly 8 bits (1 byte)
uint16_t adc_value;         // Exactly 16 bits (2 bytes)
uint32_t system_time_ms;    // Exactly 32 bits (4 bytes)

// Use static allocation instead of dynamic to avoid fragmentation
#define MAX_SAMPLES 64
uint16_t sample_buffer[MAX_SAMPLES];

// Memory-efficient data structures using bit-fields
typedef struct {
    uint8_t mode       : 2;  // Uses only 2 bits (values 0-3)
    uint8_t power_save : 1;  // Uses only 1 bit (0 or 1)
    uint8_t enabled    : 1;  // Uses only 1 bit
    uint8_t error      : 1;  // Uses only 1 bit
    uint8_t reserved   : 3;  // Reserved bits
} DeviceStatus;

/* ---- Register Manipulation ---- */

// Memory-mapped register definitions (illustrative, not for a real device)
// In real code, these would be provided by manufacturer headers
#define IO_BASE_ADDR    0x40000000
#define GPIO_PORT_A     (*(volatile uint32_t*)(IO_BASE_ADDR + 0x00))
#define GPIO_PORT_B     (*(volatile uint32_t*)(IO_BASE_ADDR + 0x04))
#define TIMER1_CTRL     (*(volatile uint32_t*)(IO_BASE_ADDR + 0x10))
#define TIMER1_COUNTER  (*(volatile uint32_t*)(IO_BASE_ADDR + 0x14))

// Bit manipulation macros (avoiding function calls for efficiency)
#define SET_BIT(REG, BIT)     ((REG) |= (1UL << (BIT)))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(1UL << (BIT)))
#define READ_BIT(REG, BIT)    (((REG) >> (BIT)) & 1UL)
#define TOGGLE_BIT(REG, BIT)  ((REG) ^= (1UL << (BIT)))

// Function to initialize GPIO pins - demonstrating register manipulation
void gpio_init(void) {
    // Configure pins 0-3 of PORT A as outputs
    // Note: In real hardware, this typically requires multiple register operations
    GPIO_PORT_A &= ~(0x0FUL);    // Clear bits 0-3
    GPIO_PORT_A |= 0x0FUL;       // Set bits 0-3 as outputs
    
    // Configure pin 7 of PORT B as input with pull-up
    CLEAR_BIT(GPIO_PORT_B, 7);   // Clear bit 7 (input mode)
    SET_BIT(GPIO_PORT_B, 15);    // Set bit 15 (pull-up enable, assuming this bit controls it)
}

// LED control using direct register access
void led_control(uint8_t led_num, bool state) {
    if (led_num > 3) return;  // Only LEDs 0-3 are connected
    
    if (state) {
        SET_BIT(GPIO_PORT_A, led_num);    // Turn on LED
    } else {
        CLEAR_BIT(GPIO_PORT_A, led_num);  // Turn off LED
    }
}

/* ---- Interrupt Handling ---- */

// Interrupt service routine declaration
// In actual code, this would use specific compiler attributes
void TIMER1_IRQHandler(void) {
    // Clear interrupt flag first (device specific)
    TIMER1_CTRL |= (1UL << 0);  // Assuming bit 0 is the flag clear bit
    
    // Handle the interrupt - toggle status LED
    TOGGLE_BIT(GPIO_PORT_A, 0);
    
    // Critical: keep ISRs short and fast
    // Don't use floating point or complex logic in ISRs
}

/* ---- Timing and Delays ---- */

// Blocking delay (avoid in production code if possible)
void delay_ms(uint32_t ms) {
    // Reset timer counter
    TIMER1_COUNTER = 0;
    
    // Assuming timer is configured to increment every 1ms
    while (TIMER1_COUNTER < ms) {
        // Could add a low-power wait instruction here
        // e.g., __WFI() (Wait For Interrupt) on ARM
    }
}

// Non-blocking timing example
bool is_elapsed(uint32_t start_time, uint32_t duration_ms) {
    uint32_t current_time = TIMER1_COUNTER;
    
    // Handle timer overflow
    if (current_time < start_time) {
        return (0xFFFFFFFF - start_time + current_time) >= duration_ms;
    } else {
        return (current_time - start_time) >= duration_ms;
    }
}

/* ---- Low Power Techniques ---- */

void enter_sleep_mode(void) {
    // Prepare for sleep
    // - Disable peripherals not needed during sleep
    // - Configure wake-up sources
    
    // Set power-saving mode bits (device specific)
    TIMER1_CTRL |= (1UL << 8);  // Assuming bit 8 enables low-power mode
    
    // Execute sleep instruction (architecture specific)
    // For ARM: __WFI();  // Wait For Interrupt
    
    // Code resumes here after waking up
    // Restore peripherals as needed
}

/* ---- Main Function ---- */

int main(void) {
    // System initialization
    gpio_init();
    
    // Main loop - never exits in embedded systems
    uint32_t last_led_toggle = 0;
    
    while (1) {  // Infinite loop
        // Check if button is pressed (active low)
        if (READ_BIT(GPIO_PORT_B, 7) == 0) {
            // Button pressed, toggle LED 1 every 100ms
            if (is_elapsed(last_led_toggle, 100)) {
                TOGGLE_BIT(GPIO_PORT_A, 1);
                last_led_toggle = TIMER1_COUNTER;
            }
        } else {
            // Button not pressed, enter low power mode
            // This is just for demonstration - in practice you'd 
            // configure wake-up sources before sleeping
            enter_sleep_mode();
        }
    }
    
    // Will never reach this point
    return 0;
}