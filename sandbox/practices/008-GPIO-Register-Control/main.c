#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// GPIO register definitions (simulating hardware registers)
typedef struct {
    uint32_t MODE;     // Pin mode (input, output, etc.)
    uint32_t OTYPE;    // Output type (push-pull or open-drain)
    uint32_t OSPEED;   // Output speed
    uint32_t PUPD;     // Pull-up/pull-down
    uint32_t IDR;      // Input data register (read-only)
    uint32_t ODR;      // Output data register
    uint32_t BSRR;     // Bit set/reset register
    uint32_t LCKR;     // Lock register
    uint32_t AFRL;     // Alternate function low register
    uint32_t AFRH;     // Alternate function high register
} GPIO_TypeDef;

// Simulated GPIO port
GPIO_TypeDef GPIOA;

// GPIO pin definitions
#define GPIO_PIN_0     ((uint16_t)0x0001)  // Pin 0
#define GPIO_PIN_1     ((uint16_t)0x0002)  // Pin 1
#define GPIO_PIN_2     ((uint16_t)0x0004)  // Pin 2
#define GPIO_PIN_3     ((uint16_t)0x0008)  // Pin 3
#define GPIO_PIN_4     ((uint16_t)0x0010)  // Pin 4
#define GPIO_PIN_5     ((uint16_t)0x0020)  // Pin 5
#define GPIO_PIN_6     ((uint16_t)0x0040)  // Pin 6
#define GPIO_PIN_7     ((uint16_t)0x0080)  // Pin 7
#define GPIO_PIN_8     ((uint16_t)0x0100)  // Pin 8
#define GPIO_PIN_9     ((uint16_t)0x0200)  // Pin 9
#define GPIO_PIN_10    ((uint16_t)0x0400)  // Pin 10
#define GPIO_PIN_11    ((uint16_t)0x0800)  // Pin 11
#define GPIO_PIN_12    ((uint16_t)0x1000)  // Pin 12
#define GPIO_PIN_13    ((uint16_t)0x2000)  // Pin 13
#define GPIO_PIN_14    ((uint16_t)0x4000)  // Pin 14
#define GPIO_PIN_15    ((uint16_t)0x8000)  // Pin 15
#define GPIO_PIN_All   ((uint16_t)0xFFFF)  // All pins

// Mode values
#define GPIO_MODE_INPUT     0x00000000U
#define GPIO_MODE_OUTPUT    0x00000001U
#define GPIO_MODE_AF        0x00000002U
#define GPIO_MODE_ANALOG    0x00000003U

// Print binary representation of a 32-bit value
void print_binary32(uint32_t value) {
    printf("0b ");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
        if (i % 8 == 0 && i != 0) printf(" ");
    }
}

// Print the state of pins 0-7
void print_pin_states(uint32_t port_value) {
    printf("Pin states: ");
    for (int i = 7; i >= 0; i--) {
        printf("%d", (port_value >> i) & 1);
    }
    printf("\n");
}

// Configure a pin as output
void GPIO_ConfigureOutput(GPIO_TypeDef* gpio, uint16_t pin) {
    // For each pin in the pin mask:
    for (int i = 0; i < 16; i++) {
        if (pin & (1 << i)) {
            // Each pin takes 2 bits in MODE register
            uint32_t temp = gpio->MODE;
            temp &= ~(3U << (i * 2)); // Clear the 2 bits
            temp |= (GPIO_MODE_OUTPUT << (i * 2)); // Set as output
            gpio->MODE = temp;
            
            // In a real system, we might also configure other parameters
            // like speed, output type, etc.
        }
    }
}

// Set pins to high state
void GPIO_SetPins(GPIO_TypeDef* gpio, uint16_t pin) {
    // Write to the Bit Set/Reset Register
    // The lower 16 bits set pins, upper 16 bits would reset pins
    gpio->BSRR = pin;
    
    // This also updates the ODR register
    gpio->ODR |= pin;
}

// Reset pins to low state
void GPIO_ResetPins(GPIO_TypeDef* gpio, uint16_t pin) {
    // Write to the Bit Set/Reset Register
    // Lower 16 bits set pins, upper 16 bits reset pins
    gpio->BSRR = (uint32_t)pin << 16;
    
    // This also updates the ODR register
    gpio->ODR &= ~pin;
}

// Toggle pins
void GPIO_TogglePins(GPIO_TypeDef* gpio, uint16_t pin) {
    gpio->ODR ^= pin;
}

// Read pin state
bool GPIO_ReadPin(GPIO_TypeDef* gpio, uint16_t pin) {
    // For simplicity, assume only one pin is specified
    return (gpio->IDR & pin) != 0;
}

int main() {
    printf("=== GPIO Register Manipulation Example ===\n\n");
    
    // Initialize GPIO registers
    GPIOA.MODE = 0; // All pins as input initially
    GPIOA.ODR = 0;  // All outputs low
    GPIOA.IDR = 0;  // All inputs read as low
    
    printf("Initial GPIO state:\n");
    printf("MODE register: 0x%08X\n", GPIOA.MODE);
    printf("ODR register: 0x%08X\n", GPIOA.ODR);
    print_pin_states(GPIOA.ODR);
    
    // Configure pins 0, 1, and 7 as outputs
    printf("\nConfiguring pins 0, 1, and 7 as outputs...\n");
    GPIO_ConfigureOutput(&GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7);
    
    printf("Updated MODE register: 0x%08X (", GPIOA.MODE);
    print_binary32(GPIOA.MODE);
    printf(")\n");
    
    // Set pins 0 and 7 high
    printf("\nSetting pins 0 and 7 high...\n");
    GPIO_SetPins(&GPIOA, GPIO_PIN_0 | GPIO_PIN_7);
    
    printf("ODR register: 0x%08X\n", GPIOA.ODR);
    print_pin_states(GPIOA.ODR);
    
    // Reset pin 0
    printf("\nResetting pin 0...\n");
    GPIO_ResetPins(&GPIOA, GPIO_PIN_0);
    
    printf("ODR register: 0x%08X\n", GPIOA.ODR);
    print_pin_states(GPIOA.ODR);
    
    // Toggle pins 1 and 7
    printf("\nToggling pins 1 and 7...\n");
    GPIO_TogglePins(&GPIOA, GPIO_PIN_1 | GPIO_PIN_7);
    
    printf("ODR register: 0x%08X\n", GPIOA.ODR);
    print_pin_states(GPIOA.ODR);
    
    // Read pin states
    printf("\nReading pin states:\n");
    
    // Simulate an external input setting pin 2 high
    printf("Simulating external input setting pin 2 high...\n");
    GPIOA.IDR |= GPIO_PIN_2;
    
    printf("IDR register: 0x%08X\n", GPIOA.IDR);
    
    printf("Pin 1 state: %d\n", GPIO_ReadPin(&GPIOA, GPIO_PIN_1) ? 1 : 0);
    printf("Pin 2 state: %d\n", GPIO_ReadPin(&GPIOA, GPIO_PIN_2) ? 1 : 0);
    printf("Pin 3 state: %d\n", GPIO_ReadPin(&GPIOA, GPIO_PIN_3) ? 1 : 0);
    
    printf("\n=== GPIO Register Access Patterns ===\n");
    
    printf("1. Bit-banding: Direct access to individual bits\n");
    printf("2. Read-modify-write: Read register, modify bits, write back\n");
    printf("3. Bit-set/reset registers: Atomic operations to avoid race conditions\n");
    printf("4. Bit masking: Using bit masks to access specific bits\n");
    
    return 0;
}