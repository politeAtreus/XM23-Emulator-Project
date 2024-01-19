#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "def.c"

typedef enum { BYTE = 1, WORD = 0 } DataType;

extern void LoadMem(char* record);

extern void StartMem();

extern void display_mem();

extern void memory_bus(unsigned short mar, unsigned short* mbr, int control, DataType dt);