#include "decode.h"
#include "execute.h"
#include "mem.h"
#include "def.c"

extern unsigned short REGFILE[MODES][REG_COUNT];
extern unsigned char memory[MEMORY_SIZE];
extern instructionInfo* instr;
extern long long clock;

// Push a value onto the stack.
void push(unsigned short* regAddress) {
    if (SP == 0x0000) {
        printf("Error: Stack overflow\n");
        return;
    }

    SP -= 2;
    memory_bus(SP, regAddress, WRITE, WORD);
    *regAddress = 0; // Clear the original register
}

 // pull a value from stack
unsigned short pull() {
    if (SP == 0xFFFF) {
        printf("Error: Stack underflow\n");
        return -1;
    }

    unsigned short data;
    memory_bus(SP, &data, READ, WORD);
    SP += 2;
    return data;
}