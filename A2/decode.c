#include "decode.h"

extern instructionInfo instr;
extern unsigned short REGFILE[MODES][REG_COUNT];
extern long long clock;

void infoinit(instructionInfo** instr) {
    *instr = (instructionInfo*)malloc(sizeof(instructionInfo));
    if (*instr == NULL) {
        // Handle malloc failure
        fprintf(stderr, "Failed to allocate memory for instructionInfo");
        exit(1); // or return an error code, depending on your program's structure
    }
    else {
        // Zero out the structure
        memset(*instr, 0, sizeof(instructionInfo));
    }
}

char* returnStr(Operation op) {
    char* operationStr[] = {
    "BL", "BEQ/BZ", "BNE/BNZ", "BC/BHS", "BNC/BLO", "BN", "BGE", "BLT", "BRA",
    "ADD", "ADDC", "SUB", "SUBC", "DADD", "CMP", "XOR", "AND", "OR", "BIT",
    "BIC", "BIS", "MOV", "SWAP", "SRA", "RRC", "COMP", "SWPB", "SXT", "SETPRI",
    "SVC", "SETCC", "CLRCC", "CEX", "LD", "ST", "MOVL", "MOVLZ", "MOVLS",
    "MOVH", "LDR", "STR"
    };

    return operationStr[op];
}

// Function to decode an instruction
void decodeInstruction(instructionInfo* instr) {
    // Extract the opcode and other fields from the instruction
    unsigned short instruction = instr->instruction;

    unsigned short check1 = (instruction >> 14) & 0x0003; //mask: 0011:  [(??)?? ???? ???? ????]
    unsigned short bit13 = (instruction >> 13) & 0x0001; //mask: 0001:   [??(?)? ???? ???? ????]
    unsigned short check2 = (instruction >> 10) & 0x0003; //mask: 0011:  [???(? ??)?? ???? ????]
    unsigned short movCheck = (instruction >> 11) & 0x0003;//mask: 0011: [???(? ?)??? ???? ????]
    unsigned short arithCheck = (instruction >> 8) & 0x0003;//mask: 0011:[???? ??(??) ???? ????]
    unsigned short check3 = (instruction >> 7) & 0x0007;//mask: 0111:    [???? ??(?? ?)??? ????]
    unsigned short shiftCheck = (instruction >> 3) & 0x0007;//mask: 0111:[???? ???? ??(?? ?)???]
    unsigned short setCheck = (instruction >> 5) & 0x0003;//mask: 0011:  [???? ???? ?(??)? ????]

    //setting up the data display variables, the printFlag determines which are printed
    instr->dstRegister = instruction & 0x0007; //mask: 0111
    instr->srcRegister = (instruction >> 3) & 0x0007;
    instr->branchoffset = instruction & 0x03ff;
    instr->BLoffset = instruction;
    instr->reloffset = (instruction >> 7) & 0x007f;//mask: [??(?? ???? ?)??? ????]
    instr->immediate = (instruction >> 3) & 0x00ff;
    instr->branchCounter = 0;

    int printFlag = 1;
    instr->rcFlag = 1;
    instr->wbFlag = 1;
    int srcFlag = 1;
    int PRPO, DEC, INC;

    //unsigned short immediate = instruction & 0xFF;

    // Extract the R/C bit, W/B bit, and decode the source and destination
    unsigned short rcBit = (instruction >> 7) & 0x0001; // 0000 0000 0000 0001
    unsigned short wbBit = (instruction >> 6) & 0x0001;

    // Decode the instruction based on the opcode
    switch (check1) {
    case check_val_0:
        //instructions from BL - BRA
        switch (bit13) {
        case check_val_0:
            //for instruction BL
            printf("BL\n");
            instr->op = BL;
            instr->branchCounter = PC + instr->BLoffset;
            printFlag = 0;
            break;

        case check_val_1:
            //for instructions BEQ/BZ - BRA
            switch (check2) {

            case check_val_0:
                printf("BEQ/BZ\n");
                instr->op = BEQ_BZ;
                instr->branchCounter = PC + instr->branchoffset;
                printFlag = 0;
                break;
            case check_val_1:
                printf("BNE/BNZ\n");
                instr->op = BNE_BNZ;
                instr->branchCounter = PC + instr->branchoffset;
                printFlag = 0;
                break;
            case check_val_2:
                printf("BC/BHS\n");
                instr->op = BC_BHS;
                instr->branchCounter = PC + instr->branchoffset;
                printFlag = 0;
                break;
            case check_val_3:
                printf("BNC/BLO\n");
                instr->branchCounter = PC + instr->branchoffset;
                instr->op = BNC_BLO;
                printFlag = 0;
                break;
            case check_val_4:
                printf("BN\n");
                instr->op = BN;
                instr->branchCounter = PC + instr->branchoffset;
                printFlag = 0;
                break;
            case check_val_5:
                printf("BGE\n");
                instr->op = BGE;
                instr->branchCounter = PC + instr->branchoffset;
                printFlag = 0;
                break;
            case check_val_6:
                printf("BLT\n");
                instr->branchCounter = PC + instr->branchoffset;
                instr->op = BLT;
                printFlag = 0;
                break;
            case check_val_7:
                printf("BRA\n");
                instr->branchCounter = PC + instr->branchoffset;
                instr->op = BRA;
                printFlag = 0;
                break;
            }
            break;
        }
        break;
    case check_val_1:
        //instructions from ADD - MOVH
        switch (bit13) {
        case check_val_0:
            //instructions from ADD - ST
            switch (check2) {
            case check_val_0:
                //instructions ADD - SUBC
                switch (arithCheck) {
                case check_val_0:
                    printf("ADD\n");
                    instr->op = ADD;
                    break;
                case check_val_1:
                    printf("ADDC\n");
                    instr->op = ADDC;
                    break;
                case check_val_2:
                    printf("SUB\n");
                    instr->op = SUB;
                    break;
                case check_val_3:
                    printf("SUBC\n");
                    instr->op = SUBC;
                    break;
                }
                break;
            case check_val_1:
                //instructions DADD - AND
                switch (arithCheck) {
                case check_val_0:
                    printf("DADD\n");
                    instr->op = DADD;
                    break;
                case check_val_1:
                    printf("CMP\n");
                    instr->op = CMP;
                    break;
                case check_val_2:
                    printf("XOR\n");
                    instr->op = XOR;
                    break;
                case check_val_3:
                    printf("AND\n");
                    instr->op = AND;
                    break;
                }
                break;
            case check_val_2:
                //instructions OR - BIS
                switch (arithCheck) {
                case check_val_0:
                    printf("OR\n");
                    instr->op = OR;
                    break;
                case check_val_1:
                    printf("BIT\n");
                    instr->op = BIT;
                    break;
                case check_val_2:
                    printf("BIC\n");
                    instr->op = BIC;
                    break;
                case check_val_3:
                    printf("BIS\n");
                    instr->op = BIS;
                    break;
                }
                break;
            case check_val_3:
                //instructions SWAP - CLRCC
                switch (check3) {

                case check_val_0:
                    //for MOV
                    printf("MOV\n");
                    instr->op = MOV;
                    break;
                case check_val_1:
                    //for SWAP
                    printf("SWAP\n");
                    instr->op = SWAP;
                    break;
                case check_val_2:
                    //instructions SRA - SXT
                    switch (shiftCheck) {
                    case check_val_0:
                        //for SRA
                        printf("SRA\n");
                        instr->op = SRA;
                        srcFlag = 0;
                        break;
                    case check_val_1:
                        //for RRC
                        printf("RRC\n");
                        instr->op = RRC;
                        srcFlag = 0;
                        break;
                    case check_val_2:
                        //for COMP
                        printf("COMP\n");
                        instr->op = COMP;
                        srcFlag = 0;
                        break;
                    case check_val_3:
                        //for SWPB
                        printf("SWPB\n");
                        instr->op = SWPB;
                        srcFlag = 0;
                        break;
                    case check_val_4:
                        //for SXT
                        printf("SXT\n");
                        instr->op = SXT;
                        srcFlag = 0;
                        break;
                    }
                    break;
                case check_val_3:
                    //instructions SETPRI - CLRCC
                    switch (setCheck) {
                    case check_val_0:
                        //for SETPRI
                        printf("SETPRI\n");
                        instr->op = SETPRI;
                        break;
                    case check_val_1:
                        //for SVC
                        printf("SVC\n");
                        instr->op = SVC;
                        break;
                    case check_val_2:
                        //for SETCC
                        printf("SETCC\n");
                        instr->op = SETCC;
                        break;
                    case check_val_3:
                        //for CLRCC
                        printf("CLRCC\n");
                        instr->op = CLRCC;
                        break;
                    }
                    break;
                }
                break;
            case check_val_4:
                //for CEX
                printf("CEX\n");
                instr->op = CEX;
                break;
                /*case check_val_5:
                            This is an unused instruction spot
                    break;*/
            case check_val_6:
                //for LD
                printf("LD\n");
                instr->op = LD;
                instr->rcFlag = 0;
                PRPO = (instruction >> 9) & 0x0001;
                DEC = (instruction >> 8) & 0x0001;
                INC = (instruction >> 7) & 0x0001;
                printf("PRPO: %d\n", PRPO);
                printf("DEC: %d\n", DEC);
                printf("INC: %d\n", INC);
                break;
            case check_val_7:
                //for ST
                printf("ST\n");
                instr->op = ST;
                instr->rcFlag = 0;
                PRPO = (instruction >> 9) & 0x0001;
                DEC = (instruction >> 8) & 0x0001;
                INC = (instruction >> 7) & 0x0001;
                printf("PRPO: %d\n", PRPO);
                printf("DEC: %d\n", DEC);
                printf("INC: %d\n", INC);
                break;
            }
            break;
        case check_val_1:
            //for MOV instructions
            switch (movCheck) {
            case check_val_0:
                instr->op = MOVL;
                instr->rcFlag = 0;
                instr->wbFlag = 0;
                srcFlag = 0;
                printf("MOVL\n");
                printf("Immediate byte: %04x\n", instr->immediate);
                break;
            case check_val_1:
                instr->op = MOVLZ;
                instr->rcFlag = 0;
                instr->wbFlag = 0;
                srcFlag = 0;
                printf("MOVLZ\n");
                printf("Immediate byte: %04x\n", instr->immediate);
                break;
            case check_val_2:
                instr->op = MOVLS;
                instr->rcFlag = 0;
                instr->wbFlag = 0;
                srcFlag = 0;
                printf("MOVLS\n");
                printf("Immediate byte: %04x\n", instr->immediate);
                break;
            case check_val_3:
                instr->op = MOVH;
                instr->rcFlag = 0;
                instr->wbFlag = 0;
                srcFlag = 0;
                printf("MOVH\n");
                printf("Immediate byte: %04x\n", instr->immediate);
                break;
            }
            break;
        }
        break;
    case check_val_2:
        //instruction LDR
        printf("LDR\n");
        instr->op = LDR;
        instr->rcFlag = 0;
        instr->reloffset = (instruction >> 7) & 0x007f;
        instr->branchCounter = PC + instr->reloffset;
        printf("Encoded offset: %04x\n", instr->reloffset);
        printf("Current PC: %04x\n", PC);
        printf("Branch PC: %04x\n", instr->branchCounter);
        break;
    case check_val_3:
        //instruction STR
        printf("STR\n");
        instr->op = STR;
        instr->rcFlag = 0;
        instr->reloffset = (instruction >> 7) & 0x007f;
        instr->branchCounter = PC + instr->reloffset;
        printf("Encoded offset: %04x\n", instr->reloffset);
        printf("Current PC: %04x\n", PC);
        printf("Branch PC: %04x\n", instr->branchCounter);
        break;
    default:
        //incase there's something weird
        printf("Unknown Instruction: %04X\n", instruction);
        break;
    }


    // Display the additional information
    if (printFlag) {
        if (instr->rcFlag) {
            printf("R/C bit: %d\n", rcBit);
        }
        if (instr->wbFlag) {
            printf("W/B bit: %d\n", wbBit);
        }
        if (srcFlag) {
            if (rcBit == 0) {       //0 means source is a register, 1 means constant
                printf("Source: %02x\n", instr->srcRegister);
            }
            else {
                printf("Constant: %02x\n", instr->srcRegister);
            }
        }

        printf("Destination: %02x\n", instr->dstRegister);
    }
    else {
        if (instr->op = BL) {
            printf("Encoded offset: %04x\n", instr->BLoffset);
        }
        else if (checkBranch(instr->op)) {
            printf("Encoded offset: %04x\n", instr->branchoffset);
        }
        else if (instr->op == LDR || instr->op == STR) {
            printf("Encoded offset: %04x\n", instr->reloffset);
        }
        else if (instr->op == MOVL || instr->op == MOVLZ || instr->op == MOVLS || instr->op == MOVH) {
            printf("Encoded offset: %04x\n", instr->immediate);
        }
        printf("Current PC: %04x\n", PC);
        printf("Branch PC: %04x\n", instr->branchCounter);
    }

    // increment clock
    clock++;
    //CALL EXECUTION FUNCTION

    printf("\n");

}

int checkBranch(Operation op) {
    if (op == BEQ_BZ || op == BNE_BNZ || op == BC_BHS || op == BNC_BLO || op == BN || op == BGE || op == BLT || op == BRA) {
        return 1;
    }
    else return 0;
}