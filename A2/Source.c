#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h> /* Signal handling software */

#include "decode.h"
#include "execute.h"
#include "processrec.h"
#include "userprompt.h"
#include "mem.h"
#include "linkedlist.h"
#include "psw.h"
#include "stack.h"
#include "cache.h"
#include "def.c"

// declare the ctrl_c_fnd variable and signal handler function
volatile sig_atomic_t ctrl_c_fnd; /* T|F - indicates whether ^C detected */

void sigint_hdlr() {
    ctrl_c_fnd = TRUE;
    signal(SIGINT, (_crt_signal_t)sigint_hdlr); /* Reinitialize SIGINT */
}

// unsigned char REGISTERS[REG_NUM];
unsigned char memory[MEMORY_SIZE];

// 2-D array to represent registers: REGFILE[MODE][REG_COUNT]
unsigned short REGFILE[MODES][REG_COUNT];

// Creating the clock variable to increment appropriately whenever a operation is performed
long long clock = 0;

//Creating an instance of the instructionInfo struct to store info of current instruction
instructionInfo* instr;

// defining variables to control cache setup
cache_line cache_mem[CACHE_SIZE]; // Cache Memory
MappingType mapping_type;
WritePolicy write_policy;

// Function to initialize REGFILE[1][REG_COUNT] with constant values
void initConstantRegs() {
    REGFILE[1][0] = CONST_0;
    REGFILE[1][1] = CONST_1;
    REGFILE[1][2] = CONST_2;
    REGFILE[1][3] = CONST_4;
    REGFILE[1][4] = CONST_8;
    REGFILE[1][5] = CONST_16;
    REGFILE[1][6] = CONST_32;
    REGFILE[1][7] = CONST_NEG_1;
}

// Function to fetch instructions from memory using the program counter
void fetchInstruction(instructionInfo* instr) {
    memory_bus(PC, &(instr->instruction), READ, WORD);
    PC += 2;  // Increment the program counter by 2
    //increment clock
    clock++;
}

/***TO CHANGE INPUT METHOD** To do fopen_s comment out the argc and argv check statements, 
as long as these run the code will detect that no file was opened with the .exe file*/

FILE* file; // define file pointer globally so it can be closed to open a new file by the emulator

int main(int argc, char** argv) {

    // Add this at the beginning of your main function
    ctrl_c_fnd = FALSE;
    signal(SIGINT, (_crt_signal_t)sigint_hdlr);

    
    if (argc < 2) {
        printf("Error: Input file not specified\n");
        return 1;
    }

    char record[512] = { 0 };
    //fopen_s(&file, "C:\\Users\\Lenovo\\OneDrive - Dalhousie University\\Dal stuff\\Summer 2023\\ECED 3403\\Lab3\\Lab3\\ArrayInit.xme", "r");

    // Open the S-Records file    
    
    if (fopen_s(&file, argv[1], "r") != 0) {
        printf("Error: Unable to open the file\n");
        return 1;
    }

    // Allocate space to the instructionInfo struct
    infoinit(&instr);

    // Allocate space to the psw
    pswinit();

    //Initialize constants in the REGFILE tabel to the appropriate constants
    initConstantRegs();

    // Clear the memory array
    memset(memory, 0, sizeof(memory));

    // Initialize registers 0->7 to zeros
    memset(REGFILE[REG], 0, sizeof(REGFILE[0]));

    // Process each S-Record in the file
    while (fgets(record, sizeof(record), file) != NULL) {
        processSRecord(record);
        LoadMem(record);
    }    

    printf("=========================================================================\n");

    StartMem();

    //Start FDE cycle ( fetch, decode, execute functions)
    while (!ctrl_c_fnd) {
        print_clock();
        PromptUser(instr);
        fetchInstruction(instr);
        decodeInstruction(instr);
        executeInstruction(instr);
    }

    // Close the file
    fclose(file);
    // Free mallocs
    free(instr);
    free(pswptr);

    return 0;
}