#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Function to check system endianness
bool is_big_endian()
{
    uint16_t value = 0x1234;
    uint8_t *ptr = (uint8_t *) &value;
    return (ptr[0] == 0x12);
}

// Display the bytes of a 32-bit integer
void display_bytes(uint32_t value)
{
    uint8_t *bytes = (uint8_t *) &value;

    printf("Value: 0x%08X\n", value);
    printf("Memory layout: [ ");
    for (int i = 0; i < 4; i++)
    {
        printf("%02X ", bytes[i]);
    }
    printf("]\n");
}

// Convert a 16-bit value from host to big-endian
uint16_t host_to_big_endian_16(uint16_t value)
{
    if (is_big_endian())
    {
        return value;  // Already big endian
    }
    else
    {
        // Swap bytes for little endian
        return ((value & 0xFF) << 8) | ((value & 0xFF00) >> 8);
    }
}

// Convert a 32-bit value from host to big-endian
uint32_t host_to_big_endian_32(uint32_t value)
{
    if (is_big_endian())
    {
        return value;  // Already big endian
    }
    else
    {
        // Swap bytes for little endian
        return ((value & 0xFF) << 24) | ((value & 0xFF00) << 8)
               | ((value & 0xFF0000) >> 8) | ((value & 0xFF000000) >> 24);
    }
}

// Convert a 16-bit value from big-endian to host
uint16_t big_endian_to_host_16(uint16_t value)
{
    return host_to_big_endian_16(value);  // Same algorithm works both ways
}

// Convert a 32-bit value from big-endian to host
uint32_t big_endian_to_host_32(uint32_t value)
{
    return host_to_big_endian_32(value);  // Same algorithm works both ways
}

// Read a 16-bit value from unaligned memory with specified endianness
uint16_t read_unaligned_16(const uint8_t *buffer, bool big_endian)
{
    uint16_t value;

    if (big_endian)
    {
        value = (uint16_t) buffer[0] << 8 | buffer[1];
    }
    else
    {
        value = (uint16_t) buffer[1] << 8 | buffer[0];
    }

    return value;
}

// Read a 32-bit value from unaligned memory with specified endianness
uint32_t read_unaligned_32(const uint8_t *buffer, bool big_endian)
{
    uint32_t value;

    if (big_endian)
    {
        value = (uint32_t) buffer[0] << 24 | (uint32_t) buffer[1] << 16
                | (uint32_t) buffer[2] << 8 | buffer[3];
    }
    else
    {
        value = (uint32_t) buffer[3] << 24 | (uint32_t) buffer[2] << 16
                | (uint32_t) buffer[1] << 8 | buffer[0];
    }

    return value;
}

// Write a 16-bit value to unaligned memory with specified endianness
void write_unaligned_16(uint8_t *buffer, uint16_t value, bool big_endian)
{
    if (big_endian)
    {
        buffer[0] = (value >> 8) & 0xFF;
        buffer[1] = value & 0xFF;
    }
    else
    {
        buffer[0] = value & 0xFF;
        buffer[1] = (value >> 8) & 0xFF;
    }
}

// Write a 32-bit value to unaligned memory with specified endianness
void write_unaligned_32(uint8_t *buffer, uint32_t value, bool big_endian)
{
    if (big_endian)
    {
        buffer[0] = (value >> 24) & 0xFF;
        buffer[1] = (value >> 16) & 0xFF;
        buffer[2] = (value >> 8) & 0xFF;
        buffer[3] = value & 0xFF;
    }
    else
    {
        buffer[0] = value & 0xFF;
        buffer[1] = (value >> 8) & 0xFF;
        buffer[2] = (value >> 16) & 0xFF;
        buffer[3] = (value >> 24) & 0xFF;
    }
}

int main()
{
    printf("=== Endianness Demonstration ===\n\n");

    // Check system endianness
    if (is_big_endian())
    {
        printf("System is BIG ENDIAN\n");
    }
    else
    {
        printf("System is LITTLE ENDIAN\n");
    }

    // Alternative way to determine endianness
    union
    {
        uint32_t i;
        char c[4];
    } endian_test = {0x01020304};

    printf("\nEndianness test using union:\n");
    printf("Memory layout: [ %02X %02X %02X %02X ]\n",
           endian_test.c[0],
           endian_test.c[1],
           endian_test.c[2],
           endian_test.c[3]);

    if (endian_test.c[0] == 0x01)
    {
        printf("Union test confirms BIG ENDIAN\n");
    }
    else if (endian_test.c[0] == 0x04)
    {
        printf("Union test confirms LITTLE ENDIAN\n");
    }
    else
    {
        printf("Union test shows MIXED ENDIAN (unusual)\n");
    }

    printf("\n=== Memory Layout of Multi-Byte Values ===\n\n");

    // Show how a 32-bit integer is stored in memory
    uint32_t test_value = 0x12345678;
    display_bytes(test_value);

    // Show layout of various types
    uint16_t value16 = 0xABCD;
    uint32_t value32 = 0x12345678;
    float float_value = 3.14159f;

    printf("\nUint16_t (0x%04X):\n", value16);
    uint8_t *bytes16 = (uint8_t *) &value16;
    printf("Memory layout: [ ");
    for (int i = 0; i < 2; i++)
    {
        printf("%02X ", bytes16[i]);
    }
    printf("]\n");

    printf("\nUint32_t (0x%08X):\n", value32);
    uint8_t *bytes32 = (uint8_t *) &value32;
    printf("Memory layout: [ ");
    for (int i = 0; i < 4; i++)
    {
        printf("%02X ", bytes32[i]);
    }
    printf("]\n");

    printf("\nFloat (%.5f):\n", float_value);
    uint8_t *bytesFloat = (uint8_t *) &float_value;
    printf("Memory layout: [ ");
    for (int i = 0; i < 4; i++)
    {
        printf("%02X ", bytesFloat[i]);
    }
    printf("]\n");

    printf("\n=== Endianness Conversion ===\n\n");

    // Convert values between endianness
    uint16_t original16 = 0x1234;
    uint16_t converted16 = host_to_big_endian_16(original16);

    printf("Original 16-bit: 0x%04X\n", original16);
    printf("Converted to big-endian: 0x%04X\n", converted16);

    uint32_t original32 = 0x12345678;
    uint32_t converted32 = host_to_big_endian_32(original32);

    printf("\nOriginal 32-bit: 0x%08X\n", original32);
    printf("Converted to big-endian: 0x%08X\n", converted32);
    printf("Converted back to host: 0x%08X\n",
           big_endian_to_host_32(converted32));

    printf("\n=== Network Protocol Example ===\n\n");

    // Network protocol typically uses big-endian (network byte order)
    // Simulate sending a simple packet with header and payload
    uint8_t packet[16] = {0};
    uint16_t length = 12;  // Length is 12 bytes
    uint32_t message_id = 0x12345678;
    char payload[] = "ABCD";

    // Write header in big-endian (network byte order)
    write_unaligned_16(packet, length, true);
    write_unaligned_32(packet + 2, message_id, true);

    // Copy payload
    memcpy(packet + 6, payload, 4);

    // Display the packet
    printf("Network packet (big-endian header):\n[ ");
    for (int i = 0; i < 10; i++)
    {
        printf("%02X ", packet[i]);
    }
    printf("]\n\n");

    // Simulate receiving the packet and parsing it
    uint16_t recv_length = read_unaligned_16(packet, true);
    uint32_t recv_message_id = read_unaligned_32(packet + 2, true);

    printf("Received packet:\n");
    printf("Length: %u\n", recv_length);
    printf("Message ID: 0x%08X\n", recv_message_id);
    printf("Payload: %.4s\n", packet + 6);

    return 0;
}
