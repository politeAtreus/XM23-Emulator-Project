#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "def.c"
#include "mem.h"
#include "decode.h"

extern void printReg();

extern void handleBreakpoints();

extern void PromptUser(instructionInfo* instr);

extern void loadFile(char* filePath);