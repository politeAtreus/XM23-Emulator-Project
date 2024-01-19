// Setting memory size
#define MEMORY_SIZE 65536

// Constants for registers and built-in constants
#define REG_COUNT 8
#define CONST_COUNT 8
#define CONST_0 0
#define CONST_1 1
#define CONST_2 2
#define CONST_4 4
#define CONST_8 8
#define CONST_16 16
#define CONST_32 32
#define CONST_NEG_1 0xffff
#define MODES 2
#define READ 0
#define WRITE 1
#define EMPTY_STACK 0x10000 // Just above the maximum addressable memory

#define PC REGFILE[0][7]
#define SP REGFILE[0][6]
#define LR REGFILE[0][5]

#define REG 0
#define CONST 1
#define TRUE 1
#define FALSE 0
#define B15(x)	(((x)>>15) & 0x01)
#define B7(x)	(((x)>>7) & 0x01)

#define CACHE_SIZE 16
#define DM_MASK 0x3F
