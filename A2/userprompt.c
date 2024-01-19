#include "userprompt.h"
#include "linkedlist.h" // include the header for linked list
#include "processrec.h"
#include "cache.h"

extern unsigned short REGFILE[MODES][REG_COUNT];
extern unsigned char memory[MEMORY_SIZE];
extern FILE* file;
extern MappingType mapping_type;
extern WritePolicy write_policy;

node* breakpointsHead = NULL; // Head node for the linked list storing breakpoints
int currentBreakpointID = 1; // ID for the next breakpoint to add
node* removedNode;

//Function to print the 7 cpu registers
void printReg() {
    int i = 0;
    while (i <= 7) {
        printf("Register R%d: %04x\n", i, REGFILE[0][i]);
        i++;
    }
}

void cacheSubMenu() {
    int cacheOption;

    do {
        printf("\nCache sub-menu:\n");
        printf("1. Set mapping type (0 for direct, 1 for associative)\n");
        printf("2. Set write policy (0 for write back, 1 for write through)\n");
        printf("3. Display Cache content\n");
        printf("4. Return to main menu\n");
        printf("Type a number from 1 to 4 and hit enter.\n\n");

        do {
            printf("Enter your selection for cache sub-menu: ");
            scanf_s("%d", &cacheOption);

            if (cacheOption < 1 || cacheOption > 4) {
                printf("Invalid option, please try again!\n");
            }
        } while (cacheOption < 1 || cacheOption > 4);

        switch (cacheOption) {
        case 1: // Set mapping type
            printf("Enter the mapping type (0 for direct, 1 for associative): ");
            scanf_s("%d", &mapping_type);
            break;

        case 2: // Set write policy
            printf("Enter the write policy (0 for write back, 1 for write through): ");
            scanf_s("%d", &write_policy);
            break;

        case 3: // Display cache content
            printf("Displaying cache content:\n");
            print_cache();
            break;

        case 4: // Return to main menu
            printf("Returning to main menu...\n");
            break;

        default:
            printf("Invalid choice. Program will stop.\n");
            exit(0);
        }

    } while (cacheOption != 4); // Repeat the cache sub-menu until the user wants to return to the main menu
}

void handleBreakpoints() {
    int option;
    do {
        printf("Choose from one of the options below:\n\n");
        printf("====================================\n");
        printf("1. View breakpoints\n");
        printf("2. Add a breakpoint\n");
        printf("3. Remove a breakpoint\n");
        printf("4. Back to main menu\n");
        printf("====================================\n\n");
        printf("Type either 1, 2, 3, or 4 and hit enter.\n\n");

        do {
            printf("Enter your selection: ");
            scanf_s("%d", &option);

            if (option < 1 || option > 4) {
                printf("Invalid option, please try again!\n");
            }
        } while (option < 1 || option > 4);

        switch (option) {
        case 1:
            printBreakpoints(breakpointsHead);
            break;

        case 2:
            printf("Enter the address of the new breakpoint (in hexadecimal): ");
            unsigned short breakpoint;
            scanf_s("%hx", &breakpoint);
            enqueue(&breakpointsHead, createNode(currentBreakpointID++, breakpoint));
            break;

        case 3:
            removedNode = dequeue(&breakpointsHead);
            if (removedNode) {
                printf("Removed breakpoint ID: %d at address: %04x\n", removedNode->ID, removedNode->breakpoint);
                free(removedNode); // Don't forget to free the node
            }
            break;

        case 4:
            // Return to main menu
            return;
        }

    } while (option != 4);
}

void PromptUser(instructionInfo* instr) {

    unsigned short instruction = instr->instruction;
    static int endlessMode = 0;

    printf("Program Counter is at: %04x\n", PC);
    printf("Current Instruction: %04x\n\n", instruction);

    node* current = breakpointsHead;
    while (current != NULL) {
        if (endlessMode && PC == current->breakpoint) {
            printf("\n====================================\n");
            printf("Breakpoint reached at address: %04x\n", PC);
            printf("====================================\n\n");
            endlessMode = 0;
            break;
        }
        current = current->next;
    }

    if (!endlessMode) {
        printf("Choose from one of the options below:\n");
        printf("1. Continue execution\n");
        printf("2. Change the Program Counter to another location\n");
        printf("3. Display the 7 Registers\n");
        printf("4. Stop\n");
        printf("5. Endless execution\n");
        printf("6. Breakpoints\n");
        printf("7. Load new .xme file\n");
        printf("8. Display Cache content\n"); // New option added here
        printf("Type a number from 1 to 8 and hit enter.\n\n");
    }

    int option;

    do {
        if (!endlessMode) {
            do {
                printf("Enter your selection: ");
                scanf_s("%d", &option);

                if (option < 1 || option > 8) {
                    printf("Invalid option, please try again!\n");
                }
            } while (option < 1 || option > 8);
        }
        else {
            option = 1;
        }

        switch (option) {
        case 1:
            break;

        case 2:
            printf("=========================================================================\n\n");

            printf("Enter the new Program Counter you wish to start at (in hexadecimal): ");
            scanf_s("%x", &PC);
            display_mem();

            break;

        case 3:
            printf("\n");
            printReg();
            break;

        case 4:
            printf("Program stopped.\n");
            exit(0);

        case 5:
            printf("Program now in endless execution mode. Use Ctrl+C to stop.\n");
            endlessMode = 1;
            break;

        case 6:
            printf("Managing breakpoints now:\n\n");
            handleBreakpoints();
            option = 3;
            printf("\nChoose from one of the options below:\n");
            printf("1. Continue execution\n");
            printf("2. Change the Program Counter to another location\n");
            printf("3. Display the 7 Registers\n");
            printf("4. Stop\n");
            printf("5. Endless execution\n");
            printf("6. Breakpoints\n");
            printf("7. Load new .xme file\n");
            printf("8. Display Cache content\n"); // New option added here
            printf("Type a number from 1 to 8 and hit enter.\n\n");
            break;

        case 7:
            printf("\n====================================\n");
            char path[1024];
            printf("Enter the path of the .xme file you wish to load: ");
            scanf_s(" %[^\n]", path, (unsigned)sizeof(path));
            printf("\n====================================\n\n");

            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            loadFile(path);
            printf("\nChoose from one of the options below:\n");
            printf("1. Continue execution\n");
            printf("2. Change the Program Counter to another location\n");
            printf("3. Display the 7 Registers\n");
            printf("4. Stop\n");
            printf("5. Endless execution\n");
            printf("6. Breakpoints\n");
            printf("7. Load new .xme file\n");
            printf("8. Display Cache content\n"); // New option added here
            printf("Type a number from 1 to 8 and hit enter.\n\n");
            break;

        case 8: // Case for managing cache settings and displaying content
            cacheSubMenu(); // Call the cacheSubMenu function here
            break;

        default:
            printf("Invalid choice. Program will stop.\n");
            exit(0);
        }

    } while (!endlessMode && (option == 3 || option == 7 || option == 8)); // Option 8 added here
    printf("\n");
}

void loadFile(char* filePath) {
    char record[512] = { 0 };

    // Close the currently open file if it exists
    if (file) {
        fclose(file);
    }

    if (fopen_s(&file, filePath, "r") != 0) {
        char errorMessage[255];
        strerror_s(errorMessage, sizeof(errorMessage), errno);
        printf("Error: Unable to open the file. Reason: %s\n", errorMessage);
        return;
    }

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

    // Close the file
    fclose(file);
}