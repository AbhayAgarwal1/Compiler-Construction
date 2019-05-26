#include "ast.h"
int ast_no_ofnodes = 0;

void print_ast(Tree_ast ast_temp){
	printf("In order printing of abstract syntax tree\n");
	print2(ast_temp->root);
	print1(ast_temp->root);
}

void print1(Node_ast nod){
	if(nod->children!=NULL){
		nod = nod->children;
		print2(nod);
		print1(nod);
		while(nod->sibling!=NULL){
			nod = nod->sibling;
			print2(nod);
			print1(nod);
		}

	}
	return;
}

void print2(Node_ast nod){
	if(nod->tag==2)
		printf("Token Name:%s\n", nod->str);
	else if(nod->tag==1)
		printf("Token: %s and Value: %s\n", nod->ele->token, nod->ele->value);
}

Tree_ast createast(Node parse_tree_root){
	Tree_ast ast = (Tree_ast) calloc (1, sizeof(struct tree_ast));
	ast->root = create_ast_node(parse_tree_root);
	recursive_tree(ast->root, parse_tree_root);//use for creating children of a node
	return ast;
}

//remember to check that you covered all cases
void recursive_tree(Node_ast nod, Node parse_tree_node){
	int rule_no = parse_tree_node->rule_no;//have to add in the parse tree
	Node temp = (Node)parse_tree_node->child, temp2;
	switch(rule_no){
		//case 0 will return a list with first main function and then the 
		//remaining functions in siblings
		case 0: nod->children = create_ast_node(&temp[1]);
				nod->children->parent = nod;
				recursive_tree(nod->children, &temp[1]);
				recursive_func_list(nod->children, &temp[0]);//used for getting list of siblings
				break;
				
		//main will contain typedefinitions declarations, other stmts, return stmt
		case 1: arraytolink(nod, &temp[1], 4);
				break;
		//case 2 and 3 are not needed as they are never run if parse tree is correct
		//function ID is at the end
		case 4: nod->children = create_ast_node(&temp[1]);
				nod->children->parent = nod;
				nod = nod->children;
				recursive_tree(nod, &temp[1]);
				nod->sibling = create_ast_node(&temp[2]);
				nod->sibling->parent = nod->parent;
				recursive_tree(nod->sibling, &temp[2]);
				nod = nod->sibling;
				nod->sibling = create_ast_node(&temp[4]);
				nod->sibling->parent = nod->parent;
				recursive_tree(nod->sibling, &temp[4]);
				nod = nod->sibling;
				nod->sibling = create_ast_ele(&temp[0]);
				nod->sibling->parent = nod->parent;
				break;
		case 5: 
		case 6: nod->children = create_ast_node(&temp[4]);
				nod->children->parent = nod;
				nod = nod->children;
				complete_param_list(nod, &temp[4]);
				recursive_tree(nod, &temp[4]);
				break;
		case 8: recursive_tree(nod, &temp[0]);
				nod = nod->children;
				nod->sibling = create_ast_ele(&temp[1]);
				nod->sibling->parent = nod->parent;
				break;
		case 9:
		case 10:
		case 62: recursive_tree(nod, &temp[0]);
				 break;

		case 11:
		case 12:
		case 51:
		case 52:
		case 63:
		case 64:
		case 65:
		case 66:
		case 68:
		case 69:
		case 75:
		case 76:
		case 77:
		case 78:
		case 79:
		case 80:
		case 81:
		case 82:
		case 83:
		case 84:
		case 85: nod->children = create_ast_ele(&temp[0]);
				 nod->children->parent = nod;
				 break;
		case 13: nod->children = create_ast_ele(&temp[1]);
				 nod->children->parent = nod;
				 break;
		case 16: arraytolink(nod, parse_tree_node, 4);
				 break;
		case 17:
		case 24:
		case 29: nod->children = create_ast_node(&temp[0]);
				 nod->children->parent = nod;
				 recursive_tree(nod->children, &temp[0]);
				 recursive_func_list(nod->children, &temp[1]);//used for getting list of siblings
				 break;

		case 19: nod->children = create_ast_ele(&temp[1]);
				 nod->children->parent = nod;
				 nod = nod->children;
				 nod->sibling = create_ast_node(&temp[2]);
				 nod->sibling->parent = nod->parent;
				 recursive_tree(nod->sibling, &temp[2]);
				 break;
		case 20: nod->children = create_ast_node(&temp[0]);
				 nod->children->parent = nod;
				 nod = nod->children;
				 nod->sibling = create_ast_node(&temp[1]);
				 nod->sibling->parent = nod->parent;
				 recursive_tree(nod, &temp[0]);
				 recursive_tree(nod->sibling, &temp[1]);
				 recursive_func_list(nod->sibling,&temp[2]);
				 break;
		case 21: temp2 = (Node)temp[1].child;
				 nod->children = create_ast_ele(&temp2[0]);
				 nod->children->parent = nod;
				 nod = nod->children;
				 nod->sibling = create_ast_ele(&temp[3]);
				 nod->sibling->parent = nod->parent;
				 break;
		case 26: recursive_tree(nod, &temp[1]);
				 nod = nod->children;
				 nod->sibling = create_ast_ele(&temp[3]);
				 nod->sibling->parent = nod->parent;
				 if(temp[4].rule_no==27){
				 	temp = (Node)temp[4].child;
				 	nod = nod->sibling;
				 	nod->sibling = create_ast_ele(&temp[1]);
				 	nod->sibling->parent = nod->parent;
				 }
				 break;
		case 31:
		case 32:
		case 33:
		case 34:
		case 35: nod->children = create_ast_node(&temp[0]);
				 nod->children->parent = nod;
				 recursive_tree(nod->children, &temp[0]);
				 break;
		case 36: nod->children = create_ast_node(&temp[0]);
				 nod->children->parent = nod;
				 nod = nod->children;
				 recursive_tree(nod, &temp[0]);
				 nod->sibling = create_ast_node(&temp[2]);
				 nod->sibling->parent = nod->parent;
				 recursive_tree(nod->sibling, &temp[2]);
				 break;
		case 37: 
		case 50:
		case 67: nod->children = create_ast_ele(&temp[0]);
				 nod->children->parent = nod;
				 nod = nod->children;
				 if(temp[1].rule_no==39 || temp[1].rule_no==53 || temp[1].rule_no==71){
				 	temp = (Node)temp[1].child;
				 	nod->sibling = create_ast_ele(&temp[1]);
				 	nod->sibling->parent = nod->parent;
				 };
				 break;
		case 40: nod->children = create_ast_ele(&temp[2]);
				 nod->children->parent = nod;
				 nod = nod->children;
				 nod->sibling = create_ast_node(&temp[0]);
				 nod->sibling->parent = nod->parent;
				 recursive_tree(nod->sibling, &temp[0]);
				 nod = nod->sibling;
				 nod->sibling = create_ast_node(&temp[5]);
				 nod->sibling->parent = nod->parent;
				 recursive_tree(nod->sibling, &temp[5]);
				 break;
		case 44: nod->children = create_ast_node(&temp[2]);
				 nod->children->parent = nod;
				 nod = nod->children;
				 recursive_tree(nod, &temp[2]);
				 nod->sibling = create_ast_node(&temp[4]);
				 nod->sibling->parent = nod->parent;
				 recursive_tree(nod->sibling, &temp[4]);
				 nod = nod->sibling;
				 nod->sibling = create_ast_node(&temp[5]);
				 nod->sibling->parent = nod->parent;
				 recursive_tree(nod->sibling, &temp[5]);
				 break;
		case 45: nod->children = create_ast_node(&temp[2]);
				 nod->children->parent = nod;
				 nod = nod->children;
				 recursive_tree(nod, &temp[2]);
				 nod->sibling = create_ast_node(&temp[5]);
				 nod->sibling->parent = nod->parent;
				 recursive_tree(nod->sibling, &temp[5]);
				 nod = nod->sibling;
				 nod->sibling = create_ast_node(&temp[6]);
				 nod->sibling->parent = nod->parent;
				 recursive_tree(nod->sibling, &temp[6]);
				 nod= nod->sibling;
				 nod->sibling = create_ast_node(&temp[7]);
				 nod->sibling->parent = nod->parent;
				 recursive_tree(nod->sibling, &temp[7]);
				 break;
		case 46: nod->children = create_ast_node(&temp[1]);
				 nod->children->parent = nod;
				 nod = nod->children;
				 recursive_tree(nod, &temp[1]);
				 nod->sibling = create_ast_node(&temp[2]);
				 nod->sibling->parent = nod->parent;
				 recursive_tree(nod->sibling, &temp[2]);
				 break;
		case 48:
		case 49: nod->children = create_ast_ele(&temp[0]);
				 nod->children->parent = nod;
				 nod = nod->children;
				 nod->sibling = create_ast_node(&temp[2]);
				 nod->sibling->parent = nod->parent;
				 recursive_tree(nod->sibling, &temp[2]);
				 break;
	//////////////////////
		case 55:
		case 58: nod->children = create_ast_node(&temp[0]);
				 nod->children->parent = nod->parent;
				 nod = nod->children;
				 if(temp[1].rule_no == 57 || temp[1].rule_no==60)
				 	recursive_tree(nod, &temp[0]);
				 else if(temp[1].rule_no == 56 || temp[1].rule_no==59){
				 	recursive_tree(nod, &temp[1]);
				 	recursive_tree(nod, &temp[0]);
				 }
				 break;
		case 56:
		case 59: temp2 = (Node)temp[0].child;
				 nod->sibling = create_ast_ele(&temp2[0]);
				 nod->sibling->parent = nod->parent;
				 nod = nod->sibling;
				 nod->children = create_ast_node(&temp[1]);
				 nod->children->parent = nod;
				 recursive_tree(nod->children, &temp[2]);
				 recursive_tree(nod->children, &temp[1]);
				 break;
		///////////////
		case 61: nod->children = create_ast_node(&temp[1]);
				 nod->children->parent = nod->parent;
				 recursive_tree(nod->children, &temp[1]);
				 break;
		case 72: nod->children = create_ast_node(&temp[3]);
				 nod->children->parent = nod;
				 nod = nod->children;
				 recursive_tree(nod, &temp[3]);	
				 nod->sibling = create_ast_node(&temp[1]);
				 nod->sibling->parent  = nod->parent;
				 nod = nod->sibling;
				 recursive_tree(nod, &temp[1]);
				 nod->sibling = create_ast_node(&temp[5]);
				 nod->sibling->parent = nod->parent;
				 recursive_tree(nod->sibling, &temp[5]);
				 break;
		case 73: nod->children = create_ast_node(&temp[1]);
				 nod->children->parent = nod;
				 nod = nod->children;
				 recursive_tree(nod, &temp[1]);	
				 nod->sibling = create_ast_node(&temp[0]);
				 nod->sibling->parent  = nod->parent;
				 nod = nod->sibling;
				 recursive_tree(nod, &temp[0]);
				 nod->sibling = create_ast_node(&temp[2]);
				 nod->sibling->parent = nod->parent;
				 recursive_tree(nod->sibling, &temp[2]);
				 break;
		case 74: nod->children = create_ast_ele(&temp[0]);
				 nod->children->parent = nod;
				 nod = nod->children;
				 nod->sibling = create_ast_node(&temp[2]);
				 nod->sibling->parent  = nod->parent;
				 nod = nod->sibling;
				 recursive_tree(nod, &temp[2]);
				 break;
		case 41:
		case 43:
		case 86:
		case 87: recursive_tree(nod, &temp[1]);
				 break;
		case 89: nod->children = create_ast_ele(&temp[0]);
				 nod->children->parent = nod;
				 complete_idlist(nod->children, &temp[1]);
				 break;

	}
}


void complete_idlist(Node_ast nod, Node parse_tree_node){
	int rule_no = parse_tree_node->rule_no;
	Node temp = (Node)parse_tree_node->child;
	if(rule_no == 90){
		temp = (Node)temp[1].child;
		Node_ast temp2  = create_ast_ele(&temp[0]);
		nod->sibling = temp2;
		temp2->parent = nod->parent;
		complete_idlist(nod->sibling, &temp[1]);
	}
	return;
};

void complete_param_list(Node_ast nod, Node parse_tree_node){
	Node temp = (Node)parse_tree_node->child;
	int rule_no = temp[2].rule_no;
	temp = (Node)temp[2].child;	
	if(rule_no==14){
		nod->sibling = create_ast_node(&temp[1]);
		nod->sibling->parent = nod->parent;
		nod = nod->sibling;
		recursive_tree(nod, &temp[1]);
		complete_param_list(nod, &temp[1]);
	}
	return;
};

//this function converts the array in parse tree to link list for node children
void arraytolink(Node_ast nod, Node parse_tree_node, int siz){
	Node temp = (Node)parse_tree_node->child;
	nod->children = create_ast_node(&temp[0]);
	nod->children->parent = nod;
	nod = nod->children;
	recursive_tree(nod, &temp[0]);
	for(int i=1; i<siz; i++){
		Node_ast temp2 = create_ast_node(&temp[i]);
		recursive_tree(temp2, &temp[i]);
		nod->sibling = temp2;
		temp2->parent = nod->parent;
		nod = nod->sibling;
	}
	return;
};

void recursive_func_list(Node_ast node, Node parse_tree_node){
	int rule_no = parse_tree_node->rule_no;
	if(rule_no == 2 || rule_no == 17 || rule_no==24 || rule_no==29 || rule_no==22){
		Node temp2 = (Node)parse_tree_node->child;
		Node_ast temp = create_ast_node(&temp2[0]);
		recursive_tree(temp, &temp2[0]);
		node->sibling = temp;
		temp->parent = node->parent;
		recursive_func_list(temp, &temp2[1]);
	}
	return;
}

Node_ast create_ast_node(Node parse_tree_node){
	ast_no_ofnodes++;
	Node_ast temp = calloc(1, sizeof(struct node_ast));
	temp->str = parse_tree_node->str;
	temp->tag = 2;
	return temp;
};

Node_ast create_ast_ele(Node parse_tree_node){
	ast_no_ofnodes++;
	Node_ast temp = calloc(1, sizeof(struct node_ast));
	temp->ele = parse_tree_node->lexeme;
	temp->str = parse_tree_node->str;
	temp->tag = 1;
	return temp;
};
