#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    int ID;
    unsigned short breakpoint;
    struct node* next;

} node;

extern node* createNode(int ID, unsigned short breakpoint);

extern void enqueue(node** head, node* newNode);

extern node* dequeue(node** head);

extern int checkEmpty(node* head);

extern void printBreakpoints(node* head);

extern void freeQueue(node** head);