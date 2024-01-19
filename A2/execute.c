#include "decode.h"
#include "execute.h"
#include "mem.h"

extern unsigned short REGFILE[MODES][REG_COUNT];
extern unsigned char memory[MEMORY_SIZE];
extern instructionInfo* instr;
extern long long clock;

unsigned short signextInstruction;
unsigned char extbit;

void print_clock() {
    printf("==========\n");
    printf("Clock ticks: %lld\n", clock);
    printf("==========\n");
}

void executeInstruction(instructionInfo* instr) {

    Operation operation = instr->op;

    signextInstruction = 0;
    extbit = 0;
    switch (operation) {
    case BL:
        printf("Executing BL instruction\n");
        LR = PC; // LR gets PC
        PC = instr->branchCounter; // PC gets PC + label
        break;
    case BEQ_BZ:
        printf("Executing BEQ/BZ instruction\n");
        if (pswptr->bit.Z == 1) PC = instr->branchCounter; // PC gets PC + label if Z = 1
        break;
    case BNE_BNZ:
        printf("Executing BNE/BNZ instruction\n");
        if (pswptr->bit.Z == 0) PC = instr->branchCounter; // PC gets PC + label if Z = 0
        break;
    case BC_BHS:
        printf("Executing BC/BHS instruction\n");
        if (pswptr->bit.C == 1) PC = instr->branchCounter; // PC gets PC + label if C = 1
        break;
    case BNC_BLO:
        printf("Executing BNC/BLO instruction\n");
        if (pswptr->bit.C == 0) PC = instr->branchCounter; // PC gets PC + label if C = 0
        break;
    case BN:
        printf("Executing BN instruction\n");
        if (pswptr->bit.N == 1) PC = instr->branchCounter; // PC gets PC + label if N = 1
        break;
    case BGE:
        printf("Executing BGE instruction\n");
        if ((pswptr->bit.N ^ pswptr->bit.V) == 0) PC = instr->branchCounter; // PC gets PC + label if N xor V = 0
        break;
    case BLT:
        printf("Executing BLT instruction\n");
        if ((pswptr->bit.N ^ pswptr->bit.V) == 1) PC = instr->branchCounter; // PC gets PC + label if N xor V = 1
        break;
    case BRA:
        printf("Executing BRA instruction\n");
        PC = instr->branchCounter; // PC gets PC + label
        break;
    case ADD:
        printf("Executing ADD instruction\n");
        // Code for ADD instruction

        arithOp(instr, instr->srcRegister, instr->dstRegister, 0, TRUE);   // Call with add parameters
        break;
    case ADDC:
        printf("Executing ADDC instruction\n");
        // Code for ADDC instruction

        arithOp(instr, instr->srcRegister, instr->dstRegister, pswptr->bit.C, TRUE);   // Call with addc parameters
        break;
    case SUB:
        printf("Executing SUB instruction\n");
        // Code for SUB instruction

        arithOp(instr, ~(instr->srcRegister), instr->dstRegister, 1, TRUE);   // Call with sub parameters
        break;
    case SUBC:
        printf("Executing SUBC instruction\n");
        // Code for SUBC instruction

        arithOp(instr, ~(instr->srcRegister), instr->dstRegister, pswptr->bit.C, TRUE);   // Call with subc parameters
        break;
    case DADD:
        printf("Executing DADD instruction\n");
        // Code for DADD instruction

        DADDexe(instr);     // Call the BCD arithmetic instruction
        break;
    case CMP:
        printf("Executing CMP instruction\n");
        // Code for CMP instruction

        arithOp(instr, ~(instr->srcRegister), instr->dstRegister, 1, FALSE);   // Call with COMPARE parameters
        break;
    case XOR:
        printf("Executing XOR instruction\n");
        // Code for XOR instruction

        XORexe(instr);  // Exclusive or between src and dst
        break;
    case AND:
        printf("Executing AND instruction\n");
        // Code for AND instruction

        ANDexe(instr);  // Just and'ing src and dst
        break;
    case OR:
        printf("Executing OR instruction\n");
        // Code for OR instruction

        ORexe(instr);   // Just or'ing src and dst
        break;
    case BIT:
        printf("Executing BIT instruction\n");
        // Code for BIT instruction

        BITexe(instr);  // perform non-destructive bit testing
        break;
    case BIC:
        printf("Executing BIC instruction\n");
        // Code for BIC instruction

        BICexe(instr);  // Perform bit clearing
        break;
    case BIS:
        printf("Executing BIS instruction\n");
        // Code for BIS instruction

        BISexe(instr);  // Perform bit setting
        break;
    case MOV:
        printf("Executing MOV instruction\n");
        // Code for MOV instruction

        MOVexe(instr);       //Call MOV function
        break;
    case SWAP:
        printf("Executing SWAP instruction\n");
        // Code for SWAP instruction

        SWAPexe(instr);      //Call SWAP function
        break;
    case SRA:
        printf("Executing SRA instruction\n");
        // Code for SRA instruction

        SRAexe(instr);
        break;
    case RRC:
        printf("Executing RRC instruction\n");
        // Code for RRC instruction

        RRCexe(instr);
        break;
    case COMP:
        printf("Executing COMP instruction\n");
        // Code for COMP instruction

        COMPexe(instr);     // Call the COMP function
        break;
    case SWPB:
        printf("Executing SWPB instruction\n");
        // Code for SWPB instruction

        SWPBexe(instr);      //Call SWPB function 
        break;
    case SXT:
        printf("Executing SXT instruction\n");
        // Code for SXT instruction

        SXTexe(instr);
        break;
        //case SETPRI:                          //Not being emulated
        //    printf("Executing SETPRI instruction\n");
        //    // Code for SETPRI instruction
        //    break;
        //case SVC:
        //    printf("Executing SVC instruction\n");
        //    // Code for SVC instruction
        //    break;
        //case SETCC:
        //    printf("Executing SETCC instruction\n");
        //    // Code for SETCC instruction
        //    break;
        //case CLRCC:
        //    printf("Executing CLRCC instruction\n");
        //    // Code for CLRCC instruction
        //    break;
        //case CEX:
        //    printf("Executing CEX instruction\n");
        //    // Code for CEX instruction
        //    break;
    case LD:
        printf("Executing LD instruction\n");
        
        LDexe(instr);    //Call the LD function to execute the instruction
        break;
    case ST:
        printf("Executing ST instruction\n");
        
        STexe(instr);    //Call the ST function to execute the instruction
        break;
    case MOVL:
        printf("Executing MOVL instruction\n");
        // Code for MOVL instruction
        REGFILE[REG][instr->dstRegister] = REGFILE[REG][instr->dstRegister] | instr->immediate;
        break;
    case MOVLZ:
        printf("Executing MOVLZ instruction\n");
        // Code for MOVLZ instruction
        REGFILE[REG][instr->dstRegister] = (REGFILE[REG][instr->dstRegister] & 0x00ff) | instr->immediate;

        break;
    case MOVLS:
        printf("Executing MOVLS instruction\n");
        // Code for MOVLS instruction
        REGFILE[REG][instr->dstRegister] = instr->immediate;
        //this works as during the decoding the MSB of immediate is already set to 0's

        break;
    case MOVH:
        printf("Executing MOVH instruction\n");
        // Code for MOVH instruction
        REGFILE[REG][instr->dstRegister] = (REGFILE[REG][instr->dstRegister] | ((instr->immediate << 8) & 0xff00));

        break;
    case LDR:
        printf("Executing LDR instruction\n");

        LDRexe(instr);   //Call the LDR function
        break;
    case STR:
        printf("Executing STR instruction\n");

        STRexe(instr);   //Call the STR function
        break;
    default:
        printf("Unrecognized instruction\n");
        break;
    }
    printf("========================================================================\n\n");

    //increment the clock
    clock++;
}

void arithOp(instructionInfo* instr, unsigned short srcReg, unsigned short dstReg, int C, int updateDst) {
    
    /*Function works for ADD, ADDC, SUB, SUBC, CMP
        -Depends on how srcRegister is passed to function and what carry value is passed*/

    unsigned short srcOriginal;
    unsigned short dstOriginal;
    unsigned short result;

    if (instr->wbFlag == 0) { // word operation
        srcOriginal = REGFILE[instr->rcFlag][srcReg];
        dstOriginal = REGFILE[REG][dstReg];

        result = dstOriginal + srcOriginal + C;
        if (updateDst)
            REGFILE[REG][dstReg] = result;
    }
    else { // byte operation
        srcOriginal = (REGFILE[instr->rcFlag][srcReg] & 0x00FF);
        dstOriginal = (REGFILE[REG][dstReg] & 0x00FF);

        result = ((dstOriginal + srcOriginal + C) & 0x00FF);
        if (updateDst) {
            REGFILE[REG][dstReg] = result;
            REGFILE[REG][dstReg] &= 0x00FF;
        }
    }

    update_psw(srcOriginal, dstOriginal, result, instr->wbFlag);
}

void DADDexe(instructionInfo* instr) {
    unsigned short dstRegister = instr->dstRegister;
    unsigned short srcRegister = instr->srcRegister;
    int numberOfNibbles = instr->wbFlag ? 2 : 4; // Process 2 nibbles for byte operation and 4 for word operation

    // Iterate through each nibble of the byte or word
    for (int i = 0; i < numberOfNibbles; i++) {
        // Extract the nibbles from the registers
        unsigned short dstNibble = (REGFILE[REG][dstRegister] >> (i * 4)) & 0xF;
        unsigned short srcNibble = (REGFILE[instr->rcFlag][srcRegister] >> (i * 4)) & 0xF;

        // Check if the nibble values are valid BCD values (less than 10)
        if (dstNibble > 9 || srcNibble > 9) {
            printf("Invalid BCD values in registers\n");
            return;
        }

        // Prepare nibbles for BCD arithmetic using half carry
        if (dstNibble > 9) {
            dstNibble -= 10;
            REGFILE[REG][dstRegister] += 6 << (4 * ((i + 1) % numberOfNibbles)); // Half carry to next nibble
        }
        if (srcNibble > 9) {
            srcNibble -= 10;
            REGFILE[instr->rcFlag][srcRegister] += 6 << (4 * ((i + 1) % numberOfNibbles)); // Half carry to next nibble
        }

        // Put the prepared nibbles back in the registers
        REGFILE[REG][dstRegister] = (REGFILE[REG][dstRegister] & ~(0xF << (i * 4))) | (dstNibble << (i * 4));
        REGFILE[instr->rcFlag][srcRegister] = (REGFILE[instr->rcFlag][srcRegister] & ~(0xF << (i * 4))) | (srcNibble << (i * 4));
    }

    // Perform the BCD addition
    unsigned short result = REGFILE[REG][dstRegister] + REGFILE[instr->rcFlag][srcRegister] + pswptr->bit.C;

    // Check if the result is greater than 99 for byte operation or 9999 for word operation
    if ((instr->wbFlag && result > 99) || (!instr->wbFlag && result > 9999)) {
        result -= (instr->wbFlag ? 100 : 10000); // Reset to 0
        pswptr->bit.C = 1; // Set the carry bit
    }
    else {
        pswptr->bit.C = 0; // Clear the carry bit
    }

    // Update the destination register with the result
    REGFILE[REG][dstRegister] = result;

    // Update PSW flags
    update_psw(REGFILE[instr->rcFlag][srcRegister], REGFILE[REG][dstRegister], REGFILE[REG][dstRegister], instr->wbFlag);
}

void XORexe(instructionInfo* instr) {
    unsigned short dstRegister = instr->dstRegister;
    unsigned short srcRegister = instr->srcRegister;

    // Perform the XOR operation
    unsigned short result = REGFILE[REG][dstRegister] ^ REGFILE[instr->rcFlag][srcRegister];

    // If byte operation, retain only lower byte of result and mask upper byte of the destination register
    if (instr->wbFlag) {
        result &= 0x00FF;    // keep only lower byte of result
        REGFILE[REG][dstRegister] &= 0xFF00;  // mask upper byte of destination register
    }

    // Store the result back into the destination register
    REGFILE[REG][dstRegister] |= result;

    // Update the PSW flags for only Z and N bits
    update_pswNZ(REGFILE[REG][dstRegister], instr->wbFlag);
}

void ANDexe(instructionInfo* instr) {
    unsigned short dstRegister = instr->dstRegister;
    unsigned short srcRegister = instr->srcRegister;

    // Perform the AND operation
    unsigned short result = REGFILE[REG][dstRegister] & REGFILE[instr->rcFlag][srcRegister];

    // If byte operation, retain only lower byte of result and mask upper byte of the destination register
    if (instr->wbFlag) {
        result &= 0x00FF;    // keep only lower byte of result
        REGFILE[REG][dstRegister] &= 0xFF00;  // mask upper byte of destination register
    }

    // Store the result back into the destination register
    REGFILE[REG][dstRegister] |= result;

    // Update the PSW flags for only Z and N bits
    update_pswNZ(REGFILE[REG][dstRegister], instr->wbFlag);
}

void ORexe(instructionInfo* instr) {
    unsigned short dstRegister = instr->dstRegister;
    unsigned short srcRegister = instr->srcRegister;

    // Perform the OR operation
    unsigned short result = REGFILE[REG][dstRegister] | REGFILE[instr->rcFlag][srcRegister];

    // If byte operation, retain only lower byte of result and mask upper byte of the destination register
    if (instr->wbFlag) {
        result &= 0x00FF;    // keep only lower byte of result
        REGFILE[REG][dstRegister] &= 0xFF00;  // mask upper byte of destination register
    }

    // Store the result back into the destination register
    REGFILE[REG][dstRegister] |= result;

    // Update the PSW flags for only Z and N bits
    update_pswNZ(REGFILE[REG][dstRegister], instr->wbFlag);
}

void BITexe(instructionInfo* instr) {
    unsigned short dstRegister = instr->dstRegister;
    unsigned short srcRegister = instr->srcRegister;
    //Perform operation and DO NOT RETURN val to dst register || non-destructive
    unsigned short result = REGFILE[REG][dstRegister] & REGFILE[instr->rcFlag][srcRegister];

    update_pswNZ(result, instr->wbFlag);
}

void BICexe(instructionInfo* instr) {
    unsigned short dstRegister = instr->dstRegister;
    unsigned short srcRegister = instr->srcRegister;
    //Perform operation and return val to dst register
    REGFILE[REG][dstRegister] &= ~REGFILE[instr->rcFlag][srcRegister];

    update_pswNZ(REGFILE[REG][dstRegister], instr->wbFlag);
}

void BISexe(instructionInfo* instr) {
    unsigned short dstRegister = instr->dstRegister;
    unsigned short srcRegister = instr->srcRegister;
    //Perform operation and return val to dst register
    REGFILE[REG][dstRegister] |= REGFILE[instr->rcFlag][srcRegister];

    update_pswNZ(REGFILE[REG][dstRegister], instr->wbFlag);
}

void MOVexe(instructionInfo* instr) {
    if (instr->wbFlag) { // Byte operation
        REGFILE[REG][instr->dstRegister] &= 0xFF00; // Clear the LSB of the destination register
        REGFILE[REG][instr->dstRegister] |= (REGFILE[REG][instr->srcRegister] & 0x00FF); // Move the LSB of source register to the LSB of destination register
    }
    else { // Word operation
        REGFILE[REG][instr->dstRegister] = REGFILE[REG][instr->srcRegister]; // Copy entire word
    }
}

void SWAPexe(instructionInfo* instr) {
    unsigned short temp = REGFILE[REG][instr->dstRegister]; // TMP gets DST register value
    REGFILE[REG][instr->dstRegister] = REGFILE[REG][instr->srcRegister]; // DST gets SRC register value
    REGFILE[REG][instr->srcRegister] = temp; // SRC gets TMP value
}

void SRAexe(instructionInfo* instr) {
    // Word mode
    if (instr->wbFlag == 0) {
        unsigned short* dstRegister = &REGFILE[REG][instr->dstRegister];
        unsigned short msb = *dstRegister & 0x8000;  // Store MSB
        pswptr->bit.C = *dstRegister & 0x1;  // Store LSB to Carry
        *dstRegister >>= 1;  // Shift right by 1
        *dstRegister |= msb;  // Keep sign bit unchanged
    }
    // Byte mode
    else {
        unsigned char* dstRegister = (unsigned char*)&REGFILE[REG][instr->dstRegister];
        unsigned char msb = *dstRegister & 0x80;  // Store MSB
        pswptr->bit.C = *dstRegister & 0x1;  // Store LSB to Carry
        *dstRegister >>= 1;  // Shift right by 1
        *dstRegister |= msb;  // Keep sign bit unchanged
    }

    // Update the original dstRegister value
    instr->dstRegister = REGFILE[REG][instr->dstRegister];
}

void RRCexe(instructionInfo* instr) {
    // Word mode
    if (instr->wbFlag == 0) {
        unsigned short* dstRegister = &REGFILE[REG][instr->dstRegister];
        pswptr->bit.C = *dstRegister & 0x1;  // Store LSB into Carry
        *dstRegister >>= 1;  // Shift right by 1
        *dstRegister |= (pswptr->bit.C << 15);  // Feed the Carry to MSB
    }
    // Byte mode
    else {
        unsigned char* dstRegister = (unsigned char*)&REGFILE[REG][instr->dstRegister];
        pswptr->bit.C = *dstRegister & 0x1;  // Store LSB into Carry
        *dstRegister >>= 1;  // Shift right by 1
        *dstRegister |= (pswptr->bit.C << 7);  // Feed the Carry to MSB
    }

    // Update the original dstRegister value
    instr->dstRegister = REGFILE[REG][instr->dstRegister];
}

void COMPexe(instructionInfo* instr) {
    // Word mode
    if (instr->wbFlag == 0) {
        unsigned short* dstRegister = &REGFILE[REG][instr->dstRegister];
        *dstRegister = ~(*dstRegister);
    }
    // Byte mode
    else {
        unsigned char* dstRegister = (unsigned char*)&REGFILE[REG][instr->dstRegister];
        *dstRegister = ~(*dstRegister);
    }

    // Update the original dstRegister value
    instr->dstRegister = REGFILE[REG][instr->dstRegister];

    // Update PSW
    update_pswNZ(instr->dstRegister, instr->wbFlag);
}

void SWPBexe(instructionInfo* instr) {
    unsigned short temp = REGFILE[REG][instr->dstRegister]; // Temp to hold the DST value
    REGFILE[REG][instr->dstRegister] = ((REGFILE[REG][instr->dstRegister] >> 8) & 0x00FF); // Shift the DST MSB to LSB
    temp = ((temp << 8) & 0xFF00); // Shift LSB to MSB
    REGFILE[REG][instr->dstRegister] |= temp; // Combine DST and temp
}

void SXTexe(instructionInfo* instr) {
    unsigned short bitcheck = REGFILE[REG][instr->dstRegister] & 0x00FF; // get the LSByte
    bitcheck = (bitcheck >> 7) & 0x0001; // check the value of the most significant bit in the LSByte
    if (bitcheck) { // if the bit is set
        REGFILE[REG][instr->dstRegister] |= 0xFF00; // Set the MSByte of the DST register to 1's
    }
    else { // if the bit is not set
        REGFILE[REG][instr->dstRegister] &= 0x00FF; // Set the MSByte of the DST register to 0's
    }
}

void LDRexe(instructionInfo* instr) {
    // Code for LDR instruction
    unsigned short tempData = 0; // to  temporarily hold the data from bus function

    extbit = (instr->reloffset >> 6) & 0x0001;
    signextInstruction = instr->reloffset;
    if (extbit) {
        signextInstruction = signextInstruction | 0xff80; //mask: 1111 1111 1000 0000 extends 1 to rest
    }
    else {
        signextInstruction = signextInstruction & 0x007f; //mask: 0000 0000 0111 1111 extends 0 to rest
    }
    cache((instr->srcRegister + signextInstruction), &tempData, READ, instr->wbFlag);
    REGFILE[REG][instr->dstRegister] = tempData;
}

void STRexe(instructionInfo* instr) {
    // Code for STR instruction
    unsigned short srcRegister = REGFILE[REG][instr->srcRegister]; // to store register value

    extbit = (instr->reloffset >> 6) & 0x0001;
    signextInstruction = instr->reloffset;
    if (extbit) {
        signextInstruction = signextInstruction | 0xff80; //mask: 1111 1111 1000 0000 extends 1 to rest
    }
    else {
        signextInstruction = signextInstruction & 0x007f; //mask: 0000 0000 0111 1111 extends 0 to rest
    }
    cache((instr->dstRegister + signextInstruction), &srcRegister, WRITE, instr->wbFlag);
}

void LDexe(instructionInfo* instr) {
    // Code for LD instruction
    unsigned short ea = REGFILE[REG][instr->srcRegister]; // Passed the value held by the source register
    unsigned short tempData = 0;

    if (instr->PRPO) {
        if (instr->DEC) {
            ea -= (instr->wbFlag) ? 1 : 2; // Subtract 1 for byte operation, 2 for word operation
        }
        if (instr->INC) {
            ea += (instr->wbFlag) ? 1 : 2; // Add 1 for byte operation, 2 for word operation
        }
        cache(ea, &tempData, READ, instr->wbFlag); // temp gets the value from memory
    }
    else {
        cache(ea, &tempData, READ, instr->wbFlag); // temp gets the value from memory
        if (instr->DEC) {
            ea -= (instr->wbFlag) ? 1 : 2; // Subtract 1 for byte operation, 2 for word operation
        }
        if (instr->INC) {
            ea += (instr->wbFlag) ? 1 : 2; // Add 1 for byte operation, 2 for word operation
        }
    }
    REGFILE[REG][instr->dstRegister] = tempData; // load temp value (from mem) into the destination register
    REGFILE[REG][instr->srcRegister] = ea; // Restore ea back to the source register
}

void STexe(instructionInfo* instr) {
    // Code for ST instruction
    unsigned short ea = REGFILE[REG][instr->dstRegister]; // Passed the value held by the destination register
    unsigned short srcRegister = REGFILE[REG][instr->srcRegister];

    if (instr->PRPO) {
        if (instr->DEC) {
            ea -= (instr->wbFlag) ? 1 : 2; // Subtract 1 for byte operation, 2 for word operation
        }
        if (instr->INC) {
            ea += (instr->wbFlag) ? 1 : 2; // Add 1 for byte operation, 2 for word operation
        }
        cache(ea, &srcRegister, WRITE, instr->wbFlag); // Memory gets the value from the source register
    }
    else {
        cache(ea, &srcRegister, WRITE, instr->wbFlag); // Memory gets the value from the source register
        if (instr->DEC) {
            ea -= (instr->wbFlag) ? 1 : 2; // Subtract 1 for byte operation, 2 for word operation
        }
        if (instr->INC) {
            ea += (instr->wbFlag) ? 1 : 2; // Add 1 for byte operation, 2 for word operation
        }
    }

    REGFILE[REG][instr->dstRegister] = ea; // Restore ea back to the destination register
}



