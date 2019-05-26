#ifndef var_ex2
#define var_ex2
// C program for array implementation of st
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>





// A structure to represent a st
struct stack
{
    int top;
    unsigned capacity;
    char** array;
};
typedef struct stack* Stack;


 
struct stack* createstack(unsigned capacity);
int isFull(struct stack* st);
int isEmpty(struct stack* st);
void push(struct stack* st, char* item);
char* pop(struct stack* st);
char* top(struct stack* st);
 
#endif