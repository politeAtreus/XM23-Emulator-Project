//#pragma once
#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "def.c"

enum { check_val_0, check_val_1, check_val_2, check_val_3, check_val_4, check_val_5, check_val_6, check_val_7 };

typedef enum {
    BL, BEQ_BZ, BNE_BNZ, BC_BHS, BNC_BLO, BN, BGE, BLT, BRA, ADD, ADDC, SUB, SUBC,
    DADD, CMP, XOR, AND, OR, BIT, BIC, BIS, MOV, SWAP, SRA, RRC, COMP, SWPB, SXT, SETPRI, SVC, SETCC,
    CLRCC, CEX, LD, ST, MOVL, MOVLZ, MOVLS, MOVH, LDR, STR
} Operation;

typedef struct {
    Operation op;
    unsigned short instruction;
    unsigned short dstRegister;
    unsigned short srcRegister;
    unsigned short branchoffset;
    unsigned short BLoffset;
    unsigned short reloffset;
    unsigned short branchCounter;
    unsigned short immediate;
    int PRPO, DEC, INC, wbFlag, rcFlag;
}instructionInfo;

extern void infoinit(instructionInfo** instr);

//This function is used to return a string of the instruction for printing to screen
extern char* returnStr(Operation op);

extern void decodeInstruction(instructionInfo* instr);

extern int checkBranch(Operation op);

#endif DECODE_H
