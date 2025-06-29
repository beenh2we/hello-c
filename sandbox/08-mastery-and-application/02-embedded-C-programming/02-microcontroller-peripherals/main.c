/* microcontroller_peripherals.c - Common peripheral interfacing */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/* ---- ADC (Analog-to-Digital Converter) ---- */

// ADC register definitions (example)
#define ADC_BASE 0x40012000
#define ADC_CR1  (*(volatile uint32_t*) (ADC_BASE + 0x00))  // Control register 1
#define ADC_CR2  (*(volatile uint32_t*) (ADC_BASE + 0x04))  // Control register 2
#define ADC_SMPR1 \
    (*(volatile uint32_t*) (ADC_BASE + 0x08))  // Sample time register 1
#define ADC_SMPR2 \
    (*(volatile uint32_t*) (ADC_BASE + 0x0C))  // Sample time register 2
#define ADC_SQR1 \
    (*(volatile uint32_t*) (ADC_BASE + 0x10))  // Regular sequence register 1
#define ADC_DR (*(volatile uint32_t*) (ADC_BASE + 0x4C))  // Data register

// Initialize ADC for single channel reading
void adc_init(uint8_t channel)
{
    // Enable ADC peripheral (device specific)
    ADC_CR1 = 0x00000100;  // Example: 12-bit resolution

    // Set sampling time for the channel
    if (channel < 10)
    {
        // Channels 0-9 are in SMPR2
        ADC_SMPR2 |= (0x07 << (channel * 3));  // Maximum sampling time
    }
    else
    {
        // Channels 10+ are in SMPR1
        ADC_SMPR1 |= (0x07 << ((channel - 10) * 3));
    }

    // Configure regular sequence
    ADC_SQR1 = 0x00000000;  // 1 conversion in regular sequence

    // Enable ADC
    ADC_CR2 |= 0x00000001;

    // Allow ADC to stabilize (device specific)
    for (volatile int i = 0; i < 10000; i++);
}

// Read ADC value from specified channel
uint16_t adc_read(uint8_t channel)
{
    // Select channel
    ADC_SQR1 = channel << 6;  // Assuming channel is set in bits [9:6]

    // Start conversion
    ADC_CR2 |= 0x40000000;  // Assuming bit 30 is SWSTART

    // Wait for conversion to complete
    while (
        !(ADC_CR1 & 0x00000002));  // Assuming bit 1 is EOC (End of Conversion)

    // Return conversion result
    return (uint16_t) (ADC_DR & 0x0000FFFF);
}

/* ---- UART (Universal Asynchronous Receiver-Transmitter) ---- */

// UART register definitions
#define UART_BASE 0x40013800
#define UART_SR   (*(volatile uint32_t*) (UART_BASE + 0x00))  // Status register
#define UART_DR   (*(volatile uint32_t*) (UART_BASE + 0x04))  // Data register
#define UART_BRR \
    (*(volatile uint32_t*) (UART_BASE + 0x08))  // Baud rate register
#define UART_CR1 \
    (*(volatile uint32_t*) (UART_BASE + 0x0C))  // Control register 1

// Initialize UART with specified baud rate
void uart_init(uint32_t baud_rate)
{
    // Configure baud rate (simplified)
    // In real systems, this would depend on clock frequencies
    UART_BRR = 0x00000683;  // Example: 9600 baud at 8MHz

    // Enable UART, transmitter and receiver
    UART_CR1 = 0x0000200C;  // UART enable, TX enable, RX enable
}

// Send a single character over UART
void uart_putc(char c)
{
    // Wait until transmit data register is empty
    while (!(UART_SR & 0x00000080));  // Assuming bit 7 is TXE

    // Write character to data register
    UART_DR = c;
}

// Send a string over UART
void uart_puts(const char* str)
{
    while (*str)
    {
        uart_putc(*str++);
    }
}

// Receive a character from UART (blocking)
char uart_getc(void)
{
    // Wait until data is received
    while (!(UART_SR & 0x00000020));  // Assuming bit 5 is RXNE

    // Read and return received data
    return (char) (UART_DR & 0xFF);
}

/* ---- I2C (Inter-Integrated Circuit) ---- */

// I2C register definitions
#define I2C_BASE 0x40005400
#define I2C_CR1  (*(volatile uint32_t*) (I2C_BASE + 0x00))
#define I2C_CR2  (*(volatile uint32_t*) (I2C_BASE + 0x04))
#define I2C_DR   (*(volatile uint32_t*) (I2C_BASE + 0x10))
#define I2C_SR1  (*(volatile uint32_t*) (I2C_BASE + 0x14))
#define I2C_SR2  (*(volatile uint32_t*) (I2C_BASE + 0x18))

// Initialize I2C peripheral
void i2c_init(void)
{
    // Set peripheral clock frequency (device specific)
    I2C_CR2 = 0x0008;  // 8 MHz peripheral clock

    // Set I2C clock configuration for 100 kHz
    I2C_CR1 = 0x0000;  // Disable I2C during configuration
    // Clock control register would be set here (CCR)

    // Enable I2C
    I2C_CR1 |= 0x0001;
}

// Write data to I2C device
bool i2c_write(uint8_t device_addr, uint8_t reg_addr, uint8_t data)
{
    // Wait until bus is not busy
    while (I2C_SR2 & 0x0002);  // Assuming bit 1 is BUSY

    // Generate START condition
    I2C_CR1 |= 0x0100;  // Assuming bit 8 is START

    // Wait for START to be generated
    while (!(I2C_SR1 & 0x0001));  // Assuming bit 0 is SB

    // Send device address (write mode)
    I2C_DR = device_addr << 1;  // LSB = 0 for write

    // Wait for address to be sent
    while (!(I2C_SR1 & 0x0002));  // Assuming bit 1 is ADDR

    // Clear ADDR flag by reading SR2
    volatile uint32_t temp = I2C_SR2;

    // Send register address
    I2C_DR = reg_addr;
    while (!(I2C_SR1 & 0x0080));  // Assuming bit 7 is TXE

    // Send data
    I2C_DR = data;
    while (!(I2C_SR1 & 0x0080));  // Wait for TXE

    // Generate STOP condition
    I2C_CR1 |= 0x0200;  // Assuming bit 9 is STOP

    return true;
}

/* ---- Main Function ---- */

int main(void)
{
    // Initialize peripherals
    adc_init(0);      // Initialize ADC for channel 0
    uart_init(9600);  // Initialize UART with 9600 baud
    i2c_init();       // Initialize I2C

    char buffer[50];
    uint16_t adc_value;

    // Main loop
    while (1)
    {
        // Read temperature from ADC
        adc_value = adc_read(0);

        // Format a message
        sprintf(buffer, "Temperature ADC: %d\r\n", adc_value);

        // Send via UART
        uart_puts(buffer);

        // Write to an I2C device (e.g., an external EEPROM)
        i2c_write(0x50, 0x10, (uint8_t) adc_value);

        // Delay
        for (volatile int i = 0; i < 1000000; i++);
    }

    return 0;
}
