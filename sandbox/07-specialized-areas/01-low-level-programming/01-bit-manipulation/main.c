#include <stdint.h>
#include <stdio.h>

// Print a byte in binary format
void print_binary(uint8_t byte)
{
    printf("0b");
    for (int i = 7; i >= 0; i--)
    {
        printf("%d", (byte >> i) & 1);
    }
}

// Print a 32-bit word in binary format with spaces for readability
void print_binary32(uint32_t value)
{
    printf("0b ");
    for (int i = 31; i >= 0; i--)
    {
        printf("%d", (value >> i) & 1);
        if (i % 8 == 0 && i != 0) printf(" ");
    }
}

int main()
{
    printf("=== Basic Bitwise Operations ===\n");

    uint8_t a = 0x5A;  // 01011010 in binary
    uint8_t b = 0x3F;  // 00111111 in binary

    printf("a = 0x%02X (", a);
    print_binary(a);
    printf(")\n");

    printf("b = 0x%02X (", b);
    print_binary(b);
    printf(")\n\n");

    // Bitwise AND
    uint8_t and_result = a & b;
    printf("a & b = 0x%02X (", and_result);
    print_binary(and_result);
    printf(")\n");

    // Bitwise OR
    uint8_t or_result = a | b;
    printf("a | b = 0x%02X (", or_result);
    print_binary(or_result);
    printf(")\n");

    // Bitwise XOR
    uint8_t xor_result = a ^ b;
    printf("a ^ b = 0x%02X (", xor_result);
    print_binary(xor_result);
    printf(")\n");

    // Bitwise NOT
    uint8_t not_result = ~a;
    printf("~a = 0x%02X (", not_result);
    print_binary(not_result);
    printf(")\n");

    // Left shift
    uint8_t left_shift = a << 2;
    printf("a << 2 = 0x%02X (", left_shift);
    print_binary(left_shift);
    printf(")\n");

    // Right shift
    uint8_t right_shift = a >> 2;
    printf("a >> 2 = 0x%02X (", right_shift);
    print_binary(right_shift);
    printf(")\n");

    printf("\n=== Bit Masks and Flags ===\n");

    // Define bit flags for a hypothetical status register
    const uint8_t STATUS_READY = 0x01;      // 00000001
    const uint8_t STATUS_ERROR = 0x02;      // 00000010
    const uint8_t STATUS_BUSY = 0x04;       // 00000100
    const uint8_t STATUS_INTERRUPT = 0x08;  // 00001000
    const uint8_t STATUS_OVERFLOW = 0x10;   // 00010000
    const uint8_t STATUS_SIGNED = 0x20;     // 00100000
    const uint8_t STATUS_UNUSED = 0x40;     // 01000000
    const uint8_t STATUS_POWER = 0x80;      // 10000000

    // Initialize an empty status
    uint8_t status = 0x00;
    printf("Initial status: 0x%02X (", status);
    print_binary(status);
    printf(")\n");

    // Set flags
    status |= STATUS_READY;      // Set READY bit
    status |= STATUS_INTERRUPT;  // Set INTERRUPT bit
    printf("After setting READY & INTERRUPT: 0x%02X (", status);
    print_binary(status);
    printf(")\n");

    // Check a flag
    if (status & STATUS_READY)
    {
        printf("System is READY\n");
    }

    if (status & STATUS_ERROR)
    {
        printf("ERROR flag is set\n");
    }
    else
    {
        printf("No ERROR reported\n");
    }

    // Clear a flag
    status &= ~STATUS_READY;  // Clear the READY bit
    printf("After clearing READY: 0x%02X (", status);
    print_binary(status);
    printf(")\n");

    // Toggle flags
    status ^= STATUS_BUSY;  // Toggle BUSY bit (off→on)
    printf("After toggling BUSY: 0x%02X (", status);
    print_binary(status);
    printf(")\n");

    status ^= STATUS_BUSY;  // Toggle BUSY bit again (on→off)
    printf("After toggling BUSY again: 0x%02X (", status);
    print_binary(status);
    printf(")\n");

    printf("\n=== Bit Fields ===\n");

    // Define a struct using bit fields
    struct PixelFormat
    {
        uint16_t blue : 5;   // 5 bits for blue (0-31)
        uint16_t green : 6;  // 6 bits for green (0-63)
        uint16_t red : 5;    // 5 bits for red (0-31)
    };

    // Create and initialize a pixel
    struct PixelFormat pixel;
    pixel.red = 31;   // Maximum red (11111)
    pixel.green = 0;  // No green (000000)
    pixel.blue = 31;  // Maximum blue (11111)

    printf("Pixel color (R,G,B): (%u,%u,%u)\n", pixel.red, pixel.green, pixel.blue);

    // Show the memory layout (assuming little endian)
    uint16_t *pixel_value = (uint16_t *) &pixel;
    printf("Memory representation: 0x%04X (", *pixel_value);
    for (int i = 15; i >= 0; i--)
    {
        printf("%d", (*pixel_value >> i) & 1);
        if (i == 5 || i == 11) printf("|");
    }
    printf(")\n");

    printf("\n=== Practical Bit Manipulation Examples ===\n");

    // Example 1: Extract specific bits from a value (bit field extraction)
    uint32_t sensor_data = 0x12345678;
    printf("Sensor data: 0x%08X\n", sensor_data);

    // Extract bits 16-23 (third byte)
    uint8_t third_byte = (sensor_data >> 16) & 0xFF;
    printf("Third byte: 0x%02X\n", third_byte);

    // Extract bits 4-11
    uint8_t custom_field = (sensor_data >> 4) & 0xFF;
    printf("Custom field (bits 4-11): 0x%02X\n", custom_field);

    // Example 2: Swap endianness of a 32-bit value
    uint32_t original = 0x12345678;
    uint32_t swapped = ((original & 0xFF000000) >> 24) | ((original & 0x00FF0000) >> 8) |
                       ((original & 0x0000FF00) << 8) | ((original & 0x000000FF) << 24);

    printf("\nOriginal value: 0x%08X\n", original);
    printf("After endian swap: 0x%08X\n", swapped);

    // Example 3: Count bits set in a value (population count)
    uint32_t value = 0xA5A5A5A5;  // 10100101 10100101 10100101 10100101
    int bit_count = 0;

    printf("\nCounting bits in 0x%08X: ", value);
    print_binary32(value);
    printf("\n");

    // Method 1: Iterate through each bit
    for (int i = 0; i < 32; i++)
    {
        bit_count += (value >> i) & 1;
    }
    printf("Number of bits set (method 1): %d\n", bit_count);

    // Method 2: Brian Kernighan's algorithm
    bit_count = 0;
    uint32_t v = value;
    while (v)
    {
        v &= (v - 1);  // Clear the least significant set bit
        bit_count++;
    }
    printf("Number of bits set (method 2): %d\n", bit_count);

    // Example 4: Find the position of the most significant bit
    v = 0x00080000;  // Bit 19 is set
    printf("\nFinding MSB in 0x%08X: ", v);
    print_binary32(v);
    printf("\n");

    int position = -1;
    for (int i = 31; i >= 0; i--)
    {
        if ((v >> i) & 1)
        {
            position = i;
            break;
        }
    }
    printf("Most significant bit position: %d\n", position);

    return 0;
}
