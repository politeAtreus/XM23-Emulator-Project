#include "processrec.h"

// Function to calculate the checksum of an S-Record
unsigned char calculateChecksum(const char* record) {
    unsigned char checksum = 0;
    int byteCount, i;

    sscanf_s(record, "S%*1x%2x", &byteCount);

    for (i = 0; i < byteCount + 1; i++) {
        unsigned int byte;
        sscanf_s(&record[i * 2 + 2], "%2x", &byte);
        checksum += (unsigned char)byte;
    }

    checksum = (~checksum) & 0xFF;
    return checksum;
}

// Function to decode and process an S-Record
void processSRecord(const char* record) {
    unsigned char checksum;
    char type;
    int byteCount, addressHI, addressLO, address, i;

    // Extract fields from the S-Record
    sscanf_s(record, "S%c%02x%2x%2x", &type, 1, &byteCount, &addressHI, &addressLO);

    // Calculate the expected checksum
    checksum = calculateChecksum(record);

    // Verify the checksum
    if (checksum != 0) {
        printf("Error: Invalid checksum in the S-Record\n");
        return;
    }

    // Combine the address using bit shifting and or operation
    address = (addressHI << 8) | addressLO;

    // Process the S-Record based on its type
    switch (type) {
    case '0': // Header record instruction
    {
        char fileName[256];
        int dataIndex = 8;
        int fileNameIndex = 0;

        // Read the hexadecimal data as characters and convert it to alphabets
        while (dataIndex < strlen(record) - 2) {
            unsigned int byte;
            sscanf_s(&record[dataIndex], "%2x", &byte);
            fileName[fileNameIndex] = (char)byte;
            dataIndex += 2;
            fileNameIndex++;
        }

        fileName[fileNameIndex] = '\0'; // Null-terminate the file name string

        printf("\nFile Name: %s\n\n", fileName);
    }
    break;

    case '1': // Data record Instruction
        printf("Address: %04x\n", address);
        printf("Data: ");
        for (i = 0; i < byteCount - 3; i += 2) {
            unsigned int byte;
            sscanf_s(&record[i + 8], "%2x", &byte);
            printf("%02x ", byte);
        }
        printf("\n\n");
        break;

    case '9': // Starting address record
        printf("Starting Address: %04x\n", address);
        break;

    default:
        printf("Error: Unknown S-Record type\n");
        break;
    }
}