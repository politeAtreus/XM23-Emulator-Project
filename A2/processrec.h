#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern unsigned char calculateChecksum(const char* record);

extern void processSRecord(const char* record);