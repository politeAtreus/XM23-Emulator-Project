#include "mem.h"

extern unsigned short REGFILE[MODES][REG_COUNT];
extern unsigned char memory[MEMORY_SIZE];
extern long long clock;

void LoadMem(char* record) {
    // Load the data into memory
    if (record[1] == '1') {
        int byteCount, addressHI, addressLO, dataIndex, i;
        unsigned int byte;

        sscanf_s(record, "S1%02x%2x%2x", &byteCount, &addressHI, &addressLO);

        dataIndex = 8;
        for (i = 0; i < byteCount - 3; i += 2) {
            sscanf_s(&record[dataIndex], "%2x", &byte);
            memory_bus((addressHI << 8) | addressLO, &byte, WRITE, BYTE); // Assuming it's a byte operation
            dataIndex += 2;
            addressLO++;
        }
    }
}

void StartMem() {

    printf("Enter the starting address of the S-Record you want to view (in hexadecimal): ");
    if (scanf_s("%hx", &PC) != 1) {
        printf("Invalid input. Please enter a valid hexadecimal address.\n");
        return;
    }

    if (PC >= MEMORY_SIZE) {
        printf("Invalid address. Please enter an address within the memory range.\n");
        return;
    }

    printf("Memory Contents at Address 0x%04x: ", PC);

    display_mem();
}

void display_mem() {
    // Display the memory contents starting from the specified address
    for (unsigned int i = PC; i < MEMORY_SIZE && i < PC + 16; i++) {
        unsigned short temp = 0;
        memory_bus(i, &temp, READ, BYTE); // Assuming it's a byte operation
        printf("%02x ", temp);
    }
    printf("\n");
}

void memory_bus(unsigned short mar, unsigned short* mbr, int R_W, DataType dt) {
    if (mar >= MEMORY_SIZE) {
        printf("Invalid memory address.\n");
        return;
    }

    switch (R_W) {
    case READ:
        if (dt == WORD) {
            *mbr = (memory[mar] << 8) | memory[mar + 1];
        }
        else if (dt == BYTE) {
            *mbr = memory[mar];
        }
        break;

    case WRITE:
        if (dt == WORD) {
            memory[mar] = (*mbr >> 8) & 0xFF;
            memory[mar + 1] = *mbr & 0xFF;
        }
        else if (dt == BYTE) {
            memory[mar] = *mbr & 0xFF;
        }
        break;

    default:
        printf("Invalid READ/WRITE value.\n");
    }

    //increment clock
    clock += 3;
}

