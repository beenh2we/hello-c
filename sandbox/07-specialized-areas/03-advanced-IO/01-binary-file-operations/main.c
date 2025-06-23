#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// Define a structure for demonstration
typedef struct {
    uint32_t id;
    char name[64];
    double value;
    uint8_t flags;
} Record;

// Function to check endianness
bool is_big_endian() {
    uint16_t test = 0x0102;
    uint8_t *ptr = (uint8_t*)&test;
    return (ptr[0] == 0x01);  // Big endian if true
}

// Function to swap endianness of 32-bit integer
uint32_t swap_uint32(uint32_t value) {
    return ((value & 0xFF) << 24) |
           ((value & 0xFF00) << 8) |
           ((value & 0xFF0000) >> 8) |
           ((value & 0xFF000000) >> 24);
}

// Function to swap endianness of double
double swap_double(double value) {
    double result;
    char *src = (char*)&value;
    char *dst = (char*)&result;
    
    for (size_t i = 0; i < sizeof(double); ++i) {
        dst[i] = src[sizeof(double) - i - 1];
    }
    
    return result;
}

// Write a binary file with records
bool create_binary_file(const char *filename, Record *records, int count) {
    FILE *file = fopen(filename, "wb");
    if (!file) return false;
    
    // Write file header (magic number, version, record count)
    const char magic[] = "REC1";
    uint32_t version = 0x0100;
    
    fwrite(magic, 1, 4, file);
    fwrite(&version, sizeof(version), 1, file);
    fwrite(&count, sizeof(count), 1, file);
    
    // Write all records
    bool convert_needed = is_big_endian(); // Convert if on big endian system
    
    for (int i = 0; i < count; i++) {
        Record temp = records[i];
        
        // Handle endianness if needed
        if (convert_needed) {
            temp.id = swap_uint32(temp.id);
            temp.value = swap_double(temp.value);
        }
        
        fwrite(&temp, sizeof(Record), 1, file);
    }
    
    fclose(file);
    return true;
}

// Read records from a binary file
int read_binary_file(const char *filename, Record **records_out) {
    FILE *file = fopen(filename, "rb");
    if (!file) return -1;
    
    // Read header
    char magic[5] = {0};
    uint32_t version, record_count;
    
    fread(magic, 1, 4, file);
    fread(&version, sizeof(version), 1, file);
    fread(&record_count, sizeof(record_count), 1, file);
    
    // Verify magic number
    if (strcmp(magic, "REC1") != 0) {
        fclose(file);
        return -1;
    }
    
    // Allocate memory for records
    Record *records = malloc(record_count * sizeof(Record));
    if (!records) {
        fclose(file);
        return -1;
    }
    
    // Read all records
    bool convert_needed = is_big_endian();
    
    for (uint32_t i = 0; i < record_count; i++) {
        if (fread(&records[i], sizeof(Record), 1, file) != 1) {
            free(records);
            fclose(file);
            return -1;
        }
        
        // Handle endianness if needed
        if (convert_needed) {
            records[i].id = swap_uint32(records[i].id);
            records[i].value = swap_double(records[i].value);
        }
    }
    
    fclose(file);
    *records_out = records;
    return record_count;
}

// Update a specific record by seeking
bool update_record(const char *filename, int record_index, const Record *new_record) {
    FILE *file = fopen(filename, "r+b"); // Open for reading and writing
    if (!file) return false;
    
    // Read header to verify format
    char magic[5] = {0};
    uint32_t version, record_count;
    
    fread(magic, 1, 4, file);
    fread(&version, sizeof(version), 1, file);
    fread(&record_count, sizeof(record_count), 1, file);
    
    // Check if record_index is valid
    if (record_index < 0 || (uint32_t)record_index >= record_count) {
        fclose(file);
        return false;
    }
    
    // Seek to the position of the record
    long header_size = 4 + sizeof(uint32_t) + sizeof(uint32_t); // Magic + version + count
    long record_position = header_size + record_index * sizeof(Record);
    
    if (fseek(file, record_position, SEEK_SET) != 0) {
        fclose(file);
        return false;
    }
    
    // Write the new record
    Record temp = *new_record;
    bool convert_needed = is_big_endian();
    
    if (convert_needed) {
        temp.id = swap_uint32(temp.id);
        temp.value = swap_double(temp.value);
    }
    
    bool result = (fwrite(&temp, sizeof(Record), 1, file) == 1);
    fclose(file);
    return result;
}

int main() {
    printf("=== Binary File Operations Demo ===\n");
    
    // Check system endianness
    printf("System is %s endian\n", is_big_endian() ? "big" : "little");
    
    // Create sample records
    Record records[3] = {
        {1, "First Record", 123.456, 0x01},
        {2, "Second Record", 789.012, 0x02},
        {3, "Third Record", 345.678, 0x03}
    };
    
    // Create a binary file with the records
    const char *filename = "records.bin";
    if (create_binary_file(filename, records, 3)) {
        printf("Created binary file with 3 records\n");
    } else {
        fprintf(stderr, "Failed to create binary file\n");
        return 1;
    }
    
    // Read the records back
    Record *read_records = NULL;
    int read_count = read_binary_file(filename, &read_records);
    
    if (read_count > 0) {
        printf("\nRead %d records from binary file:\n", read_count);
        for (int i = 0; i < read_count; i++) {
            printf("Record %d: ID=%u, Name='%s', Value=%.3f, Flags=0x%02x\n",
                   i, read_records[i].id, read_records[i].name,
                   read_records[i].value, read_records[i].flags);
        }
    }
    
    // Update a record
    Record updated_record = {2, "Updated Record", 999.999, 0x0F};
    if (update_record(filename, 1, &updated_record)) {
        printf("\nUpdated record at index 1\n");
    }
    
    // Free allocated memory
    free(read_records);
    
    return 0;
}