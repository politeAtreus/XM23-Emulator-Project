#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "psw.h"

extern void print_clock();

extern void executeInstruction(instructionInfo* instr);

extern void arithOp(instructionInfo* instr, unsigned short srcReg, unsigned short dstReg, int C, int updateDst);
extern void DADDexe(instructionInfo* instr);
extern void XORexe(instructionInfo* instr);
extern void ANDexe(instructionInfo* instr);
extern void ORexe(instructionInfo* instr);
extern void BITexe(instructionInfo* instr);
extern void BICexe(instructionInfo* instr);
extern void BISexe(instructionInfo* instr);
extern void MOVexe(instructionInfo* instr);
extern void SWAPexe(instructionInfo* instr);
extern void SRAexe(instructionInfo* instr);
extern void RRCexe(instructionInfo* instr);
extern void COMPexe(instructionInfo* instr);
extern void SWPBexe(instructionInfo* instr);
extern void SXTexe(instructionInfo* instr);
extern void LDRexe(instructionInfo* instr);
extern void STRexe(instructionInfo* instr);
extern void LDexe(instructionInfo* instr);
extern void STexe(instructionInfo* instr);