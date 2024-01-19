#include "linkedlist.h"

// Function used to create a new node
node* createNode(int ID, unsigned short breakpoint) {
    node* newNode = malloc(sizeof(node));
    if (newNode == NULL) {
        printf("Error encountered in malloc'ing a new node! Terminating.\n");
        return NULL;
    }
    //Assigning data to the new node
    newNode->ID = ID;
    newNode->breakpoint = breakpoint;
    newNode->next = NULL;

    return newNode;
}

void enqueue(node** head, node* newNode) {
    if (*head == NULL || newNode->breakpoint < (*head)->breakpoint) {
        // When a new node is being added to the queue (ready or blocked or finished),
        // check if the queue is empty or if an existing node in the queue has a higher priority than the new node.
        // If the node is new or has the highest priority, make it the head.
        newNode->next = *head;
        *head = newNode;
    }
    else {
        // If this is not the first node in the queue or not the highest priority,
        // find the right spot to insert the new node.
        node* current = *head;
        while (current->next != NULL && current->next->breakpoint <= newNode->breakpoint) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

//This function dequeues the process(node) with the highest priority, which is always the head.
node* dequeue(node** head) {
    if (*head == NULL) {
        printf("Queue is empty! Can't dequeue from an empty queue!\n");
        return NULL;
    }

    node* temp = *head;
    *head = (*head)->next;
    return temp;
}

//Function to check if the priority queue is empty
int checkEmpty(node* head) {
    return head == NULL;    //returns a '1' is the queue is empty, and returns a '0' if the queue has something in it.
}

//Function to print the details needed each tick
void printBreakpoints(node* head) {
    node* current = head;
    if (current == NULL) {
        printf("\nNo breakpoints exist currently!\n\n");
    }
    while (current != NULL) {
        printf("%d Breakpoint at address: %x\n", current->ID, current->breakpoint);
        current = current->next;
    }
}

//We want to free all the memory from heap at the end, this function travels nodes one by one and frees them
void freeQueue(node** head) {
    node* current = *head;
    while (current != NULL) {
        node* temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}