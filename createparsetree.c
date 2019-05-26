#include<stdlib.h>
#include<stdio.h>
#include "createparsetree.h"
#include "lexer.h"
Element1* llnode;
int parse_noofnodes=0;
int GLOBAL_DEPTH =0;
int MAX_DEPTH=2;
int currentfirstrow=0;

char* gettoken(struct element* ele){
	return ele->token;
}
char* getvalue(struct element* ele){
	return ele->value;
}
int getlineno(struct element* ele){
	return ele->lineno;
}



Node createNode(int i){
	parse_noofnodes++;
	Node n = (Node)calloc(1, sizeof(node));
	n->str=(char*)calloc(1, sizeof(char)*10);
	n->child=NULL;
	n->num=0;
	n->depth=GLOBAL_DEPTH;
	n->rule_no=0;
	return n;
}

Iter getChild(Node n){
	Iter i = (Iter)calloc(1, sizeof(iter));
	i->child = (Node)n->child;
	i->current = 0;
	i->n = n->num;
	return i;
}

int hasNext(Iter i){
	if(i->current<=(i->n-1)){
		return 1;
	}
	else{
		return 0;
	}
}

Node getNext(Iter i){
	if(hasNext(i)==1){
		Node temp = &((i->child)[i->current]);
		i->current=i->current+1;
		return temp;
	}
	else{
		return NULL;
	}
}

int isleaf(char* str)
{
	if(str[0]=='T')
	{
		if(str[1]=='K'){
			if(str[2]=='_')return 1;
				}
	}
	if(strcmp(str,"eps")==0)return 1;
	return 0;}
int isleaf1(char* str)
	{
	if(str[0]=='T')
	{
		if(str[1]=='K'){
			if(str[2]=='_')return 1;
				}
	}
	return 0;
}

Node createTree1(char*** deriv_array,int num,int whichrow,FILE* fp,struct iterator* iter)
{
	
	Node child = (Node)calloc(num,sizeof(node));
	parse_noofnodes += num;
	int j=1;
	for(int i=0;i<num;i++)
	{
		child[i].str=(char*)calloc(30,sizeof(char));		
		strcpy(child[i].str,deriv_array[whichrow][j]);
		child[i].num=0;
		child[i].depth=GLOBAL_DEPTH;
		child[i].child=NULL;
		child[i].rule_no=-1;
		child[i].lexeme=(struct element*)calloc(1,sizeof(struct element));
		//if(isleaf(child[i].str))return child;*/
		j++;
	}
            // remove firstrule from deriv_array or at line 77
		for(int i=0;i<num;i++)
		{
			if(!isleaf(child[i].str))
			{
			child[i].num=numeleinrhs(deriv_array[++currentfirstrow]);
			GLOBAL_DEPTH++;
			if(llnode!=NULL){
				child[i].rule_no=llnode->data;
				llnode=llnode->next;
				}//printf("rn:%d",child[i].rule_no);}
			child[i].child=(struct node*)createTree1(deriv_array,child[i].num,currentfirstrow,fp,iter);
			
			
			// currentfirstrow++;		
			GLOBAL_DEPTH--;
			}
			else
			{		const char* temp="eps";
					if(strcmp((const char* )child[i].str,temp)==0){
					
					struct element* temp_ele=(struct element* )malloc(sizeof(struct element));
					temp_ele->token=(char*)malloc(sizeof(char)*10);
					temp_ele->value=(char*)malloc(sizeof(char)*10);
					const char* temp1="TK_eps";
					strcpy(temp_ele->token,
					temp1);
					temp1="eps";
					strcpy(temp_ele->value,
					temp1);
					temp_ele->next = NULL;
					child[i].lexeme=temp_ele;
				}
				else{
				if(iter->curr!=NULL){				
				child[i].lexeme=iter->curr;
				getNextToken(iter);}
			}
			}
		}

	return child;

}
int numeleinrhs(char** row)
{int num=0;int j=1;
if(row!=NULL){
while(strcmp(row[j],"^")!=0){j++;num++;}}return num;}

Node createTree(char*** deriv_array,FILE* fp,struct iterator* iter)
{

	Node root=(Node)malloc(sizeof(node));
	root->str=(char* )malloc(sizeof(char)*30);
	parse_noofnodes++;
	strcpy(root->str,deriv_array[0][0]);
	root->num=numeleinrhs(deriv_array[0]);
	root->depth=GLOBAL_DEPTH;
	GLOBAL_DEPTH++;
	root->rule_no=0;
	root->lexeme=(struct element*)malloc(sizeof(struct element));
	root->child=(struct node*)createTree1(deriv_array,root->num,0,fp,iter);
	GLOBAL_DEPTH--;
	return root;
}

void visitNode(Node n,FILE* fp,struct iterator* iter){

	if(isleaf1(n->str)){
		struct element* curr_ele = iter->curr;
		if(curr_ele!=NULL){		
		printf("Lexeme:'%s'  Line no:%d  Token:%s  Depth of node:%d  isleaf:yes  Node Symbol:%s Rule_no:%d\n",n->lexeme->token,getlineno(curr_ele),gettoken(curr_ele),n->depth,gettoken(curr_ele),n->rule_no );
				curr_ele =  getNextToken(iter); 	
			}
		}
		else {
			printf("Lexeme:%s  Line no:NA  Token:%s  Depth of node:%d  is leaf:no  Node Symbol:%s Rule_no:%d\n",n->lexeme->value,n->str,n->depth,n->str,n->rule_no);}
	
}

void dfs1(Node root,FILE* fp,struct iterator* iter){
	if(root==NULL){
		return;
	}
	else{
		//visit root
		visitNode(root,fp,iter);
		Iter i=getChild(root);

		while(hasNext(i)){
			dfs1(getNext(i),fp,iter);
			
		} 
	}
}

int size(SeqList1* list)
	{int num=0;Element1* node = list->head;
	while(node!=NULL){num++;node=node->next;}
	return num;}

void copy(char** a,char** b)
{
	int i=0,j=0;
	while(strcmp(b[i],"^")!=0)
		{strcpy(a[i],b[i]);i++;}
	strcpy(a[i],"^");
}


void printparsetree(Node root, FILE* fp, struct iterator* iter)
{
	
		dfs1(root,fp,iter);
		return ;
}

Node returnroot(SeqList1* ll,char*** gram_array,FILE* fp,struct iterator* iter,struct iterator* iter2)
{

	int temp=size(ll);
	 char*** deriv_array;
	   deriv_array = (char***)malloc(temp*sizeof(char**));

	for(int i=0;i<temp;i++){
		deriv_array[i] = (char**)malloc(20*sizeof(char*));
		for(int j=0;j<20;j++){
		deriv_array[i][j] = (char*)malloc(30*sizeof(char));}

	}
	 llnode=ll->head; 
	 int j=0;
		while(llnode!=NULL)
		{
	 copy(deriv_array[j],gram_array[llnode->data]);
		j++;
		llnode=llnode->next;
		}
		llnode=ll->head->next;
		Node root = createTree(deriv_array,fp,iter);
		return root;
}