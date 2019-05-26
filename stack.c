#include "stack.h"
#include "string.h"
int word_l=30;


// function to create a st of given capacity. It initializes size of
// st as 0
struct stack* createstack(unsigned capacity)
{
    struct stack* st = (struct stack*) malloc(sizeof(struct stack));
    st->capacity = capacity;
    st->top = -1;
    st->array = (char**) malloc(st->capacity * sizeof(char*));
    for(int i=0;i<capacity;i++){
        st->array[i] = (char*)malloc(word_l*sizeof(char));
    }
    return st;
}
 
// stack is full when top is equal to the last index
int isFull(struct stack* st)
{   return st->top == st->capacity - 1; }
 
// stack is empty when top is equal to -1
int isEmpty(struct stack* st)
{   return st->top == -1;  }
 
// Function to add an item to st.  It increases top by 1
void push(struct stack* st, char* item)
{
    if (isFull(st))
        return;
    strcpy(st->array[++st->top],item);
    //printf("%s %s\n","stack:",item);
}
 
// Function to remove an item from st.  It decreases top by 1
char* pop(struct stack* st)
{
    //printf("%s %s\n","pop:",st->array[st->top]);
    return st->array[st->top--];
}
char* top(struct stack* st)
{
    
    return st->array[st->top];
}

