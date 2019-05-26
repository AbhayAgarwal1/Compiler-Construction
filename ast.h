#ifndef var_x5
#define var_x5

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "createparsetree.h"

typedef struct node_ast* Node_ast;
typedef struct tree_ast* Tree_ast; 

//Note that the element and char will not be used in one node
//
struct node_ast{
	int tag;//tag is 1 for lexeme(element) and 2 for other nodes(str)
	char* str;
	Element ele; 
	Node_ast sibling;
	Node_ast children;
	Node_ast parent;
};

struct tree_ast{
	Node_ast root;
};

Tree_ast createast(Node parse_tree);
Node_ast create_ast_ele(Node parse_tree_node);
Node_ast create_ast_node(Node parse_tree_node);
void recursive_tree(Node_ast nod, Node parse_tree_node);
void arraytolink(Node_ast nod, Node parse_tree_node, int siz);
void recursive_func_list(Node_ast node, Node parse_tree_node);
void complete_idlist(Node_ast nod, Node parse_tree_node);
void complete_param_list(Node_ast nod, Node parse_tree_node);
void print_ast(Tree_ast ast_temp);
void print1(Node_ast nod);
void print2(Node_ast nod);
void st_handle_otherstmts(Node_ast otherstmts,char* parentfun);
void st_otherstatements(Node_ast root);
#endif