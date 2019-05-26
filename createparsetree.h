#ifndef var_ex1
#define var_ex1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stack.c"
#include "list.c"
extern int GLOBAL_DEPTH;
extern int MAX_DEPTH;

/*

typedef struct{
	int value;
}element;
typedef element* Element;

*/
typedef struct{
	char* str;
	struct node* child;
	int num;//number of child
	int depth;
	int rule_no;
	Element lexeme;
}node;
typedef node* Node;

typedef struct{
	Node child;
	int current;
	int n;

}iter;
typedef iter* Iter;




//Element createEle(int i);
Node createNode(int i);
Iter getChild(Node n);
int hasNext(Iter i);
Node getNext(Iter i);
Node createTree1(char*** deriv_array,int num,int whichrow,FILE* fp,struct iterator* iter);
Node createTree(char*** deriv_array,FILE* fp,struct iterator* iter);
void visitNode(Node n,FILE* fp,struct iterator* iter);
void dfs(Node root);
void dfs1(Node root,FILE* fp,struct iterator* iter);
void printparsetree(Node root, FILE* fp, struct iterator* iter);
int numeleinrhs(char** row);
Node returnroot(SeqList1* ll,char*** gram_array,FILE* fp,struct iterator* iter,struct iterator* iter2);
#endif
