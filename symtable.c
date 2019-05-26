
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"


int FUNCTION_MAX = 10;
int FUNCTION_CURRENT = -1 ;

int GLOBAL_MAX = 50;
int GLOBAL_CURRENT = -1;

int RECORD_MAX =50;
int RECORD_CURRENT = -1;

int VAR_OFFSET = 0;

int INT_SIZE = 2;
int REAL_SIZE = 4;

int AST_HELP_CURRENT = -1;



/*

ALSO HAVE 3 other global var:
maintable
globaltable
recordtable
*/

void init_globalvars(){
	FUNCTION_MAX = 10;
	FUNCTION_CURRENT = -1 ;

	GLOBAL_MAX = 50;
	GLOBAL_CURRENT = -1;

	RECORD_MAX =50;
	RECORD_CURRENT = -1;

	VAR_OFFSET = 0;

	INT_SIZE = 2;
	REAL_SIZE = 4;

	AST_HELP_CURRENT = -1;


}


typedef struct element2{
	int type;
	//0-int and 1 for real
	char varname[100];
	int offset;
	int linenum;
	//modify here for record
	struct element2* next;
}element2;
typedef element2* Element2;

Element2 newelement2(int type,char* varname,int offset,int linenum){
	Element2 t = (Element2)malloc(sizeof(element2));
	t->type = type;
	strcpy(t->varname,varname);
	t->offset = offset;
	t->linenum = linenum;
	t->next = NULL;
	return t;
}

int find_element2(Element2 start,char* idname){
	Element2 t = start;

	while(t!=NULL){
		if(strcmp(t->varname,idname)==0){
			return 1;
		}
		t=t->next;
	}
	return 0 ;

}

/*
if element structure changed:
modify in :
insert_element_globaltable
insert_element_symtable
function call newelement2 in insert_element_symtable - 
newelement2
and all global and local function call in populate symbal table
*/


/////////////////
//GLOBAL VAR TABLE
Element2 globaltable;


void createglobaltable(){
	globaltable = (Element2)malloc(GLOBAL_MAX*sizeof(element2));
}

int findglobalelement(char* varname){
	if(GLOBAL_CURRENT==-1){
		return 0;
	}
	for(int i=0;i<=GLOBAL_CURRENT;i++){
		if(strcmp(varname,globaltable[i].varname)==0){
			return 1;
		}
	}
	return 0;
}

int findglobalelement_type(char* varname){
	if(GLOBAL_CURRENT==-1){
		return -1;
	}
	for(int i=0;i<=GLOBAL_CURRENT;i++){
		if(strcmp(varname,globaltable[i].varname)==0){
			return globaltable[i].type;
		}
	}
	return -1;
}

int insert_element_globaltable(int type,char* varname,int offset,int linenum){
	// return 1 if element inserted elsr 0

	if(findglobalelement(varname)==1){
		printf("line:%d,semantic error,global varname:%s already declared\n",linenum,varname );
		return 0;
	}

	GLOBAL_CURRENT = GLOBAL_CURRENT+1;
	Element2 t = &globaltable[GLOBAL_CURRENT];
	t->type = type;
	strcpy(t->varname,varname);
	t->offset = offset;
	t->linenum = linenum;
	t->next = NULL;
	return 1;
}




//////////////////////////////////////////
//RECORD TABLE
typedef struct rec_ele{
	char name[100];
	int width;
	Element2 rec_list;
	Element2 rec_list_head;
}rec_ele;
typedef rec_ele* Rec_ele;
Rec_ele recordtable;

void createrecordtable(){
	recordtable = (Rec_ele)malloc(RECORD_MAX*sizeof(rec_ele));
	if(RECORD_CURRENT==-1){
		strcpy(recordtable[0].name,"int");
		strcpy(recordtable[1].name,"real");
		recordtable[0].width = INT_SIZE;
		recordtable[1].width =  REAL_SIZE;
		RECORD_CURRENT=1;
	}

}

int findrecord(char* recordname){
	//return index if found else -1
	
	for(int i=0;i<=RECORD_CURRENT;i++){
		if(strcmp(recordname,recordtable[i].name)==0){
			return i;
		}
	}
	return -1;
}

int find_record_element(char* fieldname,int r){
	Element2 temp = recordtable[r].rec_list;
	while(temp!=NULL){
		if(strcmp(temp->varname,fieldname)==0){
			return 1;
		}

		temp=temp->next;
	}
	return 0;
}
//returns index of inserted record
int insertrecordtable(char* recordname,int lineno){
	//return -1 on failure

	//check if there is an recordid present with this name
	if(findrecord(recordname)>=0){
		printf("line:%d,semantic error,record id: %s, already declared once\n",lineno,recordname );
		return -1;
	}


	RECORD_CURRENT=RECORD_CURRENT+1;
	strcpy(recordtable[RECORD_CURRENT].name,recordname);
	recordtable[RECORD_CURRENT].width=0;
	recordtable[RECORD_CURRENT].rec_list = NULL;
	recordtable[RECORD_CURRENT].rec_list_head = NULL;
	return RECORD_CURRENT;
}

int insert_element_recordtable(int record_num,int type,int lineno,char* idname){
	//// return 1 if element inserted elsr 0
	Rec_ele temp_r = &recordtable[record_num];
	int t;

	Element2 ele = newelement2(type,idname,0,0);
	
	if(temp_r->rec_list_head==NULL){
	
		temp_r->rec_list = ele;
		temp_r->rec_list_head = ele;
	}
	
	
	else{
		t=find_element2(temp_r->rec_list,idname);
		if(t==1){
			printf("line:%d,semantic error,field id: %s,already declared in record\n",lineno,idname );
			return 0;
			
		}
		else{
			temp_r->rec_list_head->next = ele;
			temp_r->rec_list_head = ele;
		}
	}
	
	return 1;


}	


void printrecordtype(int num){
	Element2 temp = recordtable[num].rec_list;
	printf("(");
	while(temp!=NULL){
		if(temp->type==0){
			printf("%s,","int");
		}
		else{
			printf("%s,","real");
		}
		temp=temp->next;
	}
	printf(")");
}

void printrecordtable(){
	if(RECORD_CURRENT==1){
		printf("No records in current program\n");
	}
	printf("-----------------RECORD TABLE-----------\n");
	for (int i = 2; i <= RECORD_CURRENT; i++)
	{
		printf("Name:%s ,Type:",recordtable[i].name );
		printrecordtype(i);
		printf(" ,Width: %d\n",recordtable[i].width );

	}
}



////////////////////////////


typedef struct symtableNode{
	char function_name[100];
	Element2 input_list;
	Element2 input_list_head;
	Element2 output_list;
	Element2 output_list_head;
	Element2 local_list;	
	Element2 local_list_head;
}symtableNode;
typedef symtableNode* Symtable;

Symtable maintable;

void createsymtable(){
	maintable = (Symtable)malloc(FUNCTION_MAX*sizeof(symtableNode));	
}

int findfunction_symtable(char* function_name){
	//1 if found
	if(FUNCTION_CURRENT==-1){
		return 0;
	}
	for(int i=0;i<=FUNCTION_MAX;i++){
		if(strcmp(function_name,maintable[i].function_name)==0){
			return 1;
		}
	}
	return 0;

}

int getfunctionNumber(char* function_name){
	if(FUNCTION_CURRENT==-1){
		return -1;
	}
	for(int i=0;i<=FUNCTION_MAX;i++){
		if(strcmp(function_name,maintable[i].function_name)==0){
			return i;
		}
	}
	return -1;
}

int insertfunction(char* function_name,int lineno){
	//returns 1 on sucessful insertion
	if(findfunction_symtable(function_name)==1){
		printf("line:%d, semantic error, function with name %s already defined \n",lineno,function_name);
		return 0;
	}

	FUNCTION_CURRENT = FUNCTION_CURRENT+1;
	strcpy(maintable[FUNCTION_CURRENT].function_name,function_name);
	maintable[FUNCTION_CURRENT].input_list = NULL;
	maintable[FUNCTION_CURRENT].input_list_head = NULL;
	maintable[FUNCTION_CURRENT].output_list = NULL;
	maintable[FUNCTION_CURRENT].output_list_head = NULL;
	maintable[FUNCTION_CURRENT].local_list = NULL;
	maintable[FUNCTION_CURRENT].local_list_head = NULL;
	return 1;
}

Symtable getfunctionNode(char* function_name){
	//returns NULL on failure and a semantic error
	if(FUNCTION_CURRENT==-1){
		return NULL;
	}

	for (int i = 0; i <=FUNCTION_CURRENT; i++)
	{
		if(strcmp(function_name,maintable[i].function_name)==0){
			return &maintable[i];
		}
	}
	//if not found then also semantic error
	return NULL;
}

int findsymboltable_element_list(Element2 list,char* name){
	// 1 if found
	Element2 temp = list;
	while(temp!=NULL){
			if(strcmp(temp->varname,name)==0){
				return 1;
			}
			temp=temp->next;
	}
	return 0;
}

int findsymboltable_element_list_type(Element2 list,char* name){
	// 1 if found
	Element2 temp = list;
	while(temp!=NULL){
			if(strcmp(temp->varname,name)==0){
				return temp->type;
			}
			temp=temp->next;
	}
	return -1;
}

int findsymboltable_element(Symtable node,char* varname){
	// 1 if found
	//search element in all the three lists
	if(findsymboltable_element_list(node->input_list,varname)==1){
		return 1;
	}
	if(findsymboltable_element_list(node->output_list,varname)==1){
		return 1;
	}
	if(findsymboltable_element_list(node->local_list,varname)==1){
		return 1;
	}
	return 0;

}

int findsymboltable_element_type(Symtable node,char* varname){
	// 1 if found
	//search element in all the three lists
	int t = findsymboltable_element_list_type(node->input_list,varname);
	if(t>=0){
		return t;
	}
	t=findsymboltable_element_list_type(node->output_list,varname);
	if(t>=0){
		return t;
	}
	t=findsymboltable_element_list_type(node->local_list,varname);
	if(t>=0){
		return t;
	}
	return -1;

}





int insert_element_symtable(char* function_name,int whichlist,int type,char* varname,int offset,int linenum){
	// return 1 if element inserted elsr 0
	//which list = 0-input, 1-output,2-local
	Symtable node = getfunctionNode(function_name);//sematic error never occurs heres


	//check in global table
	if(findglobalelement(varname)==1){
		printf("line:%d,semantic error, a global variable of name %s already declared \n",linenum,varname);
		return 0;

	}

	//check in function scope
	if(findsymboltable_element(node,varname)==1){
		printf("line:%d,semantic error, variable %s already declared once in this function\n",linenum,varname);
		return 0;

	}
	

	Element2 temp = newelement2(type,varname,offset,linenum);

	if(whichlist==0){
		if(node->input_list_head==NULL){
			node->input_list = temp;
			node->input_list_head = temp;
		}
		else{
			node->input_list_head->next = temp;
			node->input_list_head = temp;
		}
	}
	else if(whichlist==1)
	{
		if(node->output_list_head==NULL){
			node->output_list = temp;
			node->output_list_head = temp;
		}
		else{
			node->output_list_head->next = temp;
			node->output_list_head = temp;
		}
	}
	else if(whichlist==2)
	{

		if(node->local_list_head==NULL){
			node->local_list = temp;
			node->local_list_head = temp;
		}
		else{
			node->local_list_head->next = temp;
			node->local_list_head = temp;
		}
	}

	else{
		printf("%s\n","WRONG whichlist NUM" );
		return 0;
	}

	return 1;

}

void print_elementslist_symtable(Element2 start,int function_num){
	Element2 temp= start;
	char tempname[100];
	int tnum;
	strcpy(tempname,maintable[function_num].function_name);
	while(temp!=NULL){
		printf("Lexeme: %s, Type: ",temp->varname );
		tnum = temp->type;
		if(tnum==0){
			printf("int");
		}
		else if(tnum==1){
			printf("real");
		}
		else{
			printrecordtype(tnum);
		}
		printf(", Scope: %s , offset: %d\n",tempname,temp->offset );


		temp=temp->next;
	}


}

void printglobaltable(){
	printf("-GLOBAL VARIABLES:------------\n" );
	if(GLOBAL_CURRENT==-1){
		printf("No global variables defined in this program\n");
		return;
	}
	int tnum;
	
	for(int i=0;i<=GLOBAL_CURRENT;i++){
		printf("Lexeme: %s, Type: ",globaltable[i].varname );
		tnum = globaltable[i].type;
		if(tnum==0){
			printf("int");
		}
		else if(tnum==1){
			printf("real");
		}
		else{
			printrecordtype(tnum);
		}
		printf(", Scope: global , offset: --\n");
	}
}

void print_global_symtable(){
	printf("-----------SYMBOL TABLE------------\n" );
	printglobaltable();
	if(FUNCTION_CURRENT==-1){
		return;
	}
	printf("-LOCAL VARIABLES:------------\n" );
	for(int i=0;i<=FUNCTION_CURRENT;i++){
		print_elementslist_symtable(maintable[i].input_list,i);	
		print_elementslist_symtable(maintable[i].output_list,i);	
		print_elementslist_symtable(maintable[i].local_list,i);		

	}

}

int get_function_width(int a){
	int total=0;
	Element2 list = maintable[a].input_list;
	while(list!=NULL){
		total=total+recordtable[list->type].width;
		list=list->next;
	}
	list = maintable[a].output_list;
	while(list!=NULL){
		total=total+recordtable[list->type].width;
		list=list->next;
	}
	list = maintable[a].local_list;
	while(list!=NULL){
		total=total+recordtable[list->type].width;
		list=list->next;
	}
	return total;
}

void print_functionwidth_table(){
	if(FUNCTION_CURRENT==-1){
		printf("PLEASE ADD ATLEAST 1 FUNCTION\n");
		return;
	}
	int t;
	printf("FUNCTION TABLE------------\n");
	for(int i=0;i<=FUNCTION_CURRENT;i++){
		t= get_function_width(i);
		printf("Name: %s , width: %d\n",maintable[i].function_name,t);
	}

}

////////////////////////////////




int st_getscope(Node_ast id){
	//return 0-local,1-global
	//declaration->children->sibling(id) passed

	if((id->sibling)==NULL){

		return 0;
	}
	else{

		return 1;
	}
}

int st_gettype(Node_ast node_type){
	// int node passed
	// returns the type num: 0-int ,1-real, >1reocrd
	//return -1 if invalid type
	if(strcmp("TK_INT",node_type->ele->token)==0){

		return 0;
	}
	else if(strcmp("TK_REAL",node_type->ele->token)==0){

		return 1;
	}
	else{
		int t = findrecord(node_type->ele->value);
		
		return t;
	}
}

int st_gettype_given_id(Node_ast id,char* function_name){
	//search in global
	char name[100];
	strcpy(name,id->ele->value);
	int t = findglobalelement_type(name);
	if(t!=-1){
		return t;
	}
	Symtable fun = getfunctionNode(function_name);
	t = findsymboltable_element_type(fun,name);
	if(t!=-1){
		return t;
	}

	return -1;

}


void st_typedefinition(Node_ast typedefinitions){
	Node_ast typedefinition = typedefinitions->children;

	char temp_recordid[100];
	int temp_record_num;
	int temp_width;
	Node_ast temp_fd;
	int lineno;
	int temp_type;

	while(typedefinition!=NULL){

		temp_width=0;
		lineno = typedefinition->children->ele->lineno;
		strcpy(temp_recordid,typedefinition->children->ele->value);
		temp_record_num = insertrecordtable(temp_recordid,lineno);

		//skip record if invalid
		if(temp_record_num>1){

			temp_fd = typedefinition->children->sibling->children;
			lineno = temp_fd->children->ele->lineno;

			while(temp_fd!=NULL){
				temp_type=st_gettype(temp_fd->children);
				if(temp_type>1){
					printf("line:%d,type error in record definition of %s :field type invalid\n",lineno,temp_recordid);
				}

				//insert into recordtable
				if(insert_element_recordtable(temp_record_num,temp_type,lineno,temp_fd->children->sibling->ele->value)==1){

					//calculate width
					if(temp_type==0){
						temp_width=temp_width+INT_SIZE;
					}
					else if(temp_type==1){
						temp_width=temp_width+REAL_SIZE;
					}
					else{}
				}
				temp_fd=temp_fd->sibling;
			}
		//insert width in recordtable
			recordtable[temp_record_num].width = temp_width;
		}

		typedefinition = typedefinition->sibling;
	}
}

//function to populate local list ,pass declarations node
void st_declaration(Node_ast declarations,char* function_name,int passnum){
	//this function not for input/output list
	//passnum 1 - populate global
	//passnum 2 - populate symbol

	Node_ast declaration = declarations->children;

	int temptype;
	char tempname[100];
	int tempscope;
	int templine_num;
	int tempoffset;
	int tempinsert;
	
	while(declaration!=NULL){
		tempscope = st_getscope(declaration->children->sibling);
		//return 0-local,1-global
		temptype = st_gettype(declaration->children);
		templine_num =declaration->children->ele->lineno;
		strcpy(tempname,declaration->children->sibling->ele->value);
		
		//check if valid type
		if(temptype!=-1){
			
			
			tempoffset = VAR_OFFSET;
			tempinsert = 0;

			if(tempscope==1){
				if(passnum==1){

					tempinsert=insert_element_globaltable(temptype,tempname,tempoffset,templine_num);

				}
			}
			else{
				if(passnum==2){
					tempinsert=insert_element_symtable(function_name,2,temptype,tempname,tempoffset,templine_num);

				}
			}

			//update offset only if element inserted

			if(tempinsert==1){

			
				if(temptype==0){
					VAR_OFFSET=VAR_OFFSET+INT_SIZE;
				}
				else if(temptype==1){
					VAR_OFFSET=VAR_OFFSET+REAL_SIZE;
				}
				else{
					VAR_OFFSET=VAR_OFFSET+ recordtable[temptype].width;
				}
			}
		}
		else{
			printf("line:%d, type error,type:%s of id:%s invalid\n",templine_num,declaration->children->ele->value,tempname);
		}
		declaration = declaration->sibling;
	}
	return;
}

//pass input/output parameters node
void st_function_IOparameters_extraction(Node_ast par,char* function_name,int isinput){
	//input - 0
	//output - 1
	Node_ast parlist = par->children;

	int temptype;
	char tempname[100];
	int templine_num;

	int tempinsert;

	while(parlist!=NULL){
		temptype = st_gettype(parlist->children);
		templine_num =parlist->children->ele->lineno;
		strcpy(tempname,parlist->children->sibling->ele->value);

		if(temptype!=-1){
			tempinsert = insert_element_symtable(function_name,isinput,temptype,tempname,VAR_OFFSET,templine_num);
			if(tempinsert==1){
				if(temptype==0){
					VAR_OFFSET=VAR_OFFSET+INT_SIZE;
				}
				else if(temptype==1){
					VAR_OFFSET=VAR_OFFSET+REAL_SIZE;
				}
				else{
					VAR_OFFSET=VAR_OFFSET+ recordtable[temptype].width;
				}
			}

		}
		else{
			printf("line:%d, type error,type:%s of id:%s invalid\n",templine_num,parlist->children->ele->value,tempname);

		}


		parlist=parlist->sibling;
	}

}




/////////////////


char ast_id_array[30][20]={"atemp0","atemp1","atemp2","atemp3","atemp4","atemp5","atemp6","atemp7","atemp8","atemp9","atemp10","atemp11","atemp12","atemp13","atemp14","atemp15","atemp16","atemp17","atemp18","atemp19","atemp20","atemp21","atemp22","atemp23","atemp24","atemp25","atemp26","atemp27","atemp28","atemp29"};

typedef struct ast_help_Node{
	char record_id[100];
	char record_fieldid[100];
	char asm_id[100];
	
}ast_help_Node;
typedef ast_help_Node* Ast_help_Node;

Ast_help_Node ast_helptable;

void ast_help_create_table(){
	ast_helptable = (Ast_help_Node)malloc(30*sizeof(ast_help_Node));
}

void ast_help_insert_element_table(char* recid,char* fieldid){
	if(AST_HELP_CURRENT==29){
		printf("Max 30 records can be declared\n");
	}
	AST_HELP_CURRENT = AST_HELP_CURRENT+1;
	strcpy(ast_helptable[AST_HELP_CURRENT].record_id,recid);
	strcpy(ast_helptable[AST_HELP_CURRENT].record_fieldid,fieldid);
	strcpy(ast_helptable[AST_HELP_CURRENT].asm_id,ast_id_array[AST_HELP_CURRENT]);
}


void ast_help_tablepopulate(Node_ast root){

	Node_ast dec = root->children->children->sibling->children;
	int temptype;
	char temp_recordname[100];
	char temp_fieldname[100];

	Element2 temp_rec_list;


	while(dec!=NULL){
		temptype = st_gettype(dec->children);
		if(temptype>1){
			//then record
			strcpy(temp_recordname,dec->children->sibling->ele->value);
			temp_rec_list = recordtable[temptype].rec_list;

			while(temp_rec_list!=NULL){
				strcpy(temp_fieldname,temp_rec_list->varname);

				ast_help_insert_element_table(temp_recordname,temp_fieldname);

				temp_rec_list=temp_rec_list->next;
			}





		}

		dec=dec->sibling;
	}
}









///////////////////////////////
Node_ast st_getchild(Node_ast parent,int num){
	//num 0 - parent->children
	Node_ast child = parent->children;
	if(num>0){
		for(int i=0;i<num;i++){
			child=child->sibling;
		}
	}
}

void st_loadfunctions(Node_ast root){
	
	//insert other functions
	Node_ast funct = root->children->sibling;

	char tempname[100];
	int templineno;
	int tempinsert;
	while(funct!=NULL){
		strcpy(tempname,funct->children->sibling->sibling->sibling->ele->value);
		templineno = funct->children->sibling->sibling->sibling->ele->lineno;

		tempinsert = insertfunction(tempname,templineno);
		if(tempinsert==1){
			//if needed to do anything on sucess
		}

		funct = funct->sibling;
	}

	//insert main, its name hardcoded here "_main"
	insertfunction("_main",0);//no error will occur here so dummy line number

}

void st_loadrecords(Node_ast root){
	//load other functions typedef
	Node_ast funct = root->children->sibling;
	Node_ast temp;
	while(funct!=NULL){
		temp = funct->children->sibling->sibling->children;
		st_typedefinition(temp);

		funct = funct->sibling;
	}
	//load main - typedef
	st_typedefinition(root->children->children);

}

void st_loadglobal(Node_ast root){
	//for other functions
	Node_ast funct = root->children->sibling;
	Node_ast temp;
	while(funct!=NULL){
		temp = funct->children->sibling->sibling->children->sibling;
		st_declaration(temp,"dummy_function_name",1);

		funct = funct->sibling;
	}

	//for main
	st_declaration(root->children->children->sibling,"dummy_function_name",1);
	VAR_OFFSET = 0;

}

void st_load_IO_Local(Node_ast root){
	//for functions
	Node_ast funct = root->children->sibling;
	Node_ast temp;
	char function_name[100];
	while(funct!=NULL){
		strcpy(function_name,funct->children->sibling->sibling->sibling->ele->value);
		VAR_OFFSET=0;
		//input
		temp = funct->children;
		st_function_IOparameters_extraction(temp,function_name,0);
		//output
		temp = temp->sibling;
		st_function_IOparameters_extraction(temp,function_name,1);
		//local
		temp = temp->sibling->children->sibling;
		st_declaration(temp,function_name,2);

		funct = funct->sibling;
	}

	//for main
	//only local is there
	temp = root->children->children->sibling;
	VAR_OFFSET=0;
	st_declaration(temp,"_main",2);


}



/////////////////////

int st_isdeclared(Node_ast id ,char* function_name){
	// 1 if declared,0 if not
	char idname[100];
	strcpy(idname,id->ele->value);
	Symtable node = getfunctionNode(function_name);
	if(findglobalelement(idname)==1){
		return 1;
	}
	else if(findsymboltable_element(node,idname)==1){
		return 1;
	}
	else{
		return -1;
	}
}

int st_function_call_Pars_declaration_check(Node_ast funstmt,char* parentfun){
	// 1 - declared
	// 0 - not declared

	int templineno = funstmt->children->ele->lineno;

	//output parameters
	int tt = 1;

	Node_ast given_outpar = funstmt->children->sibling->children;
	while(given_outpar!=NULL){
		if(st_isdeclared(given_outpar,parentfun)==-1){
			tt=0;
			printf("line: %d,semantic error,the output parameter variable %s has not been declared\n",templineno,given_outpar->ele->value );

		}

		given_outpar=given_outpar->sibling;
	}

	//input parameters
	Node_ast given_inpar = funstmt->children->sibling->sibling->children;
	while(given_inpar!=NULL){
		if(st_isdeclared(given_inpar,parentfun)==-1){
			tt=0;
			printf("line: %d,semantic error,the input parameter variable %s has not been declared\n",templineno,given_inpar->ele->value );

		}

		given_inpar=given_inpar->sibling;
	}


	return tt;
}

int st_function_call_definedbeforeAndrecur_check(Node_ast funstmt,char* parentfun){
	// 1 - no error
	// 0 - error
	char childfun[100];
	strcpy(childfun,funstmt->children->ele->value);
	int templineno = funstmt->children->ele->lineno;

	int childnum = getfunctionNumber(childfun);
	int parentnum = getfunctionNumber(parentfun);
	int t=1;

	if(childnum==parentnum){
		t=0;
		printf("line:%d, semantic error, function:%s calling itself,recursion not allowed\n",templineno,parentfun);
	}
	else if(childnum>parentnum){
		t=0;
		printf("line: %d,semantic error, called function:%s not defined before calling function:%s\n",templineno,childfun,parentfun );
	}
	else if(childnum==-1){
		t=0;
		printf("line: %d,semantic error, called function:%s not defined in the program\n",templineno,childfun );
	}
	else{
		return t;
	}
}

void st_function_call_type_check(Node_ast funstmt,char* parentfun){


	char childfun[100];
	strcpy(childfun,funstmt->children->ele->value);
	
	Symtable childnode = getfunctionNode(childfun);
	int templineno = funstmt->children->ele->lineno;

	//output parameters

	Node_ast given_outpar = funstmt->children->sibling->children;//LIST OF ID
	Element2 actual_outpar = childnode->output_list;

	int given_type;
	int actual_type;

	char given_type_name[100];
	char actual_type_name[100];

	//count num of para
	while((given_outpar!=NULL)&&(actual_outpar!=NULL)){
		given_outpar = given_outpar->sibling;
		actual_outpar = actual_outpar->next;
	}
	if((given_outpar!=NULL)||(actual_outpar!=NULL)){
		printf("line: %d,semantic error,number of output parameters not matching with called function:%s definition\n",templineno,childfun );
	}
	else{

		given_outpar = funstmt->children->sibling->children;
		actual_outpar = childnode->output_list;
		//check individual types
		while((given_outpar!=NULL)&&(actual_outpar!=NULL)){
			given_type = st_gettype_given_id(given_outpar,parentfun);
			actual_type = actual_outpar->type;

		

			if(given_type!=actual_type){
				strcpy(given_type_name,recordtable[given_type].name);
				strcpy(actual_type_name,recordtable[actual_type].name);

				printf("line: %d,semantic error,the type <%s> of variable <%s> does not match with type <%s> of formal output parameter <%s>\n",templineno,given_type_name,given_outpar->ele->value,actual_type_name,actual_outpar->varname);            

			}



			given_outpar = given_outpar->sibling;
			actual_outpar = actual_outpar->next;
		}

	}


	//input parameters

	Node_ast given_inpar = funstmt->children->sibling->sibling->children;//LIST OF ID
	Element2 actual_inpar = childnode->input_list;


	//count num of para
	while((given_inpar!=NULL)&&(actual_inpar!=NULL)){
		given_inpar = given_inpar->sibling;
		actual_inpar = actual_inpar->next;
	}
	if((given_inpar!=NULL)||(actual_inpar!=NULL)){
		printf("line: %d,semantic error,number of input parameters not matching with called function:%s definition\n",templineno,childfun );
	}
	else{

		given_inpar = funstmt->children->sibling->sibling->children;
		actual_inpar = childnode->input_list;
		//check individual types
		while((given_inpar!=NULL)&&(actual_inpar!=NULL)){
			given_type = st_gettype_given_id(given_inpar,parentfun);
			actual_type = actual_inpar->type;


			if(given_type!=actual_type){
				strcpy(given_type_name,recordtable[given_type].name);
				strcpy(actual_type_name,recordtable[actual_type].name);

				printf("line: %d,semantic error,the type <%s> of variable <%s> does not match with type <%s> of formal input parameter <%s>\n",templineno,given_type_name,given_inpar->ele->value,actual_type_name,actual_inpar->varname);            

			}



			given_inpar = given_inpar->sibling;
			actual_inpar = actual_inpar->next;
		}

	}
}

void st_funtion_call_all_check(Node_ast funstmt,char* parentfun){

	int t1 = st_function_call_definedbeforeAndrecur_check(funstmt,parentfun);
	int t2 = st_function_call_Pars_declaration_check(funstmt,parentfun);

	if((t1==1)&&(t2==1)){
		st_function_call_type_check(funstmt,parentfun);
	}
}

////////////////////////////////

Node_ast declaration_list;
Node_ast declaration_list_head;

void init_dec(){
	declaration_list=NULL;
	declaration_list_head=NULL;
}
void add_dec_ele_tolist(Node_ast id){
	if(declaration_list==NULL){
		declaration_list=id;
		declaration_list_head=id;
		return;
	}
	
	declaration_list_head->sibling=id;
	declaration_list_head=id;
}

void add_dec_ele(Node_ast id){
	if(id->sibling==NULL){
		id->children=NULL;
		add_dec_ele_tolist(id);
	}
	else{
		Node_ast temp = (Node_ast)malloc(sizeof(struct node_ast));
		*temp=*id;
		temp->children=temp->sibling;
		temp->sibling=NULL;
		add_dec_ele_tolist(temp);
	}
}

void check_dec_list(char* function_name){
	Node_ast temp = declaration_list;
	int t;
	int t2;
	int lineno;
	while(temp!=NULL){
		lineno = temp->ele->lineno;
		if(temp->children==NULL){
			
			t=st_gettype_given_id(temp,function_name);
			if(t==-1){
				printf("line: %d,semantic error,variable: %s not decared\n",lineno,temp->ele->value );
			}
		}
		else{
			
			t=st_gettype_given_id(temp,function_name);
			if(t==-1){
				printf("line: %d,semantic error,record variable: %s not decared\n",lineno,temp->ele->value );
			}
			else{
				t2=find_record_element(temp->children->ele->value,t);
				if(t2==0){
					printf("line: %d,semantic error,field:%s does not exist for %s type variable\n",lineno,temp->children->ele->value,recordtable[t].name );
				}
			}


		}


		temp=temp->sibling;
	}

}

////////////////////////

void dec2(Node_ast nod){

	if(nod->tag==1)
		if(strcmp(nod->ele->token,"TK_ID")==0){
			
			add_dec_ele(nod);

		}
}


void dec1(Node_ast nod){
	if(nod->children!=NULL){
		nod = nod->children;
		dec2(nod);
		dec1(nod);
		while(nod->sibling!=NULL){
			nod = nod->sibling;
			dec2(nod);
			dec1(nod);
		}

	}
	return;
}

void declaration_ast(Node_ast node,char* function_name){
	dec2(node);
	dec1(node);
	
	check_dec_list(function_name);
}


///////////////////

////////
void st_handle_ostmt(Node_ast stmtn,char* parentfun){
	Node_ast stmt = stmtn;
	Node_ast ep;

	char stmttype[100];

	Node_ast temp;
	
		strcpy(stmttype,stmt->children->str);
		temp = stmt->children;
		if(strcmp("assignmentStmt",stmttype)==0){
			init_dec();
			declaration_ast(temp,parentfun);
		}
		else if(strcmp("iterativeStmt",stmttype)==0){
			init_dec();
			declaration_ast(temp->children,parentfun);
			st_handle_ostmt(temp->children->sibling,parentfun);
			st_handle_otherstmts(temp->children->sibling->sibling,parentfun);

		}
		else if(strcmp("conditionalStmt",stmttype)==0){
			init_dec();
			declaration_ast(temp->children,parentfun);

			st_handle_ostmt(temp->children->sibling,parentfun);
			st_handle_otherstmts(temp->children->sibling->sibling,parentfun);
			ep=temp->children->sibling->sibling->sibling;
			if(ep->children!=NULL){
				st_handle_ostmt(ep->children,parentfun);
				st_handle_otherstmts(ep->children->sibling,parentfun);
			}

		}
		else if(strcmp("ioStmt",stmttype)==0){
			init_dec();
			declaration_ast(temp,parentfun);
		}
		else if(strcmp("funCallStmt",stmttype)==0){
			//
			st_funtion_call_all_check(temp,parentfun);
		}
		else{
			printf("invalid stmt\n");
		}

}

void st_handle_otherstmts(Node_ast otherstmts,char* parentfun){

	Node_ast stmt = otherstmts->children;
	Node_ast ep;

	char stmttype[100];

	Node_ast temp;
	while(stmt!=NULL){
		strcpy(stmttype,stmt->children->str);
		temp = stmt->children;
		if(strcmp("assignmentStmt",stmttype)==0){
			init_dec();
			declaration_ast(temp,parentfun);
		}
		else if(strcmp("iterativeStmt",stmttype)==0){
			init_dec();
			declaration_ast(temp->children,parentfun);
			st_handle_ostmt(temp->children->sibling,parentfun);
			st_handle_otherstmts(temp->children->sibling->sibling,parentfun);

		}
		else if(strcmp("conditionalStmt",stmttype)==0){
			init_dec();
			declaration_ast(temp->children,parentfun);

			st_handle_ostmt(temp->children->sibling,parentfun);
			st_handle_otherstmts(temp->children->sibling->sibling,parentfun);
			ep=temp->children->sibling->sibling->sibling;
			if(ep->children!=NULL){
				st_handle_ostmt(ep->children,parentfun);
				st_handle_otherstmts(ep->children->sibling,parentfun);
			}

		}
		else if(strcmp("ioStmt",stmttype)==0){
			init_dec();
			declaration_ast(temp,parentfun);
		}
		else if(strcmp("funCallStmt",stmttype)==0){
			//
			st_funtion_call_all_check(temp,parentfun);
		}
		else{
			printf("invalid stmt\n");
		}


		stmt=stmt->sibling;

	}
	//HANDLE RETURN HERE
}


void st_otherstatements(Node_ast root){

	Node_ast funct = root->children->sibling;
	char parentname[100];
	Node_ast stmts;
	while(funct!=NULL){
		stmts = funct->children->sibling->sibling;
		strcpy(parentname,stmts->sibling->ele->value);
		st_handle_otherstmts(stmts->children->sibling->sibling,parentname);	

		funct=funct->sibling;
	} 
	//FOR MAIN
	st_handle_otherstmts(root->children->children->sibling->sibling,"_main");
	
}



/////////////////////////////




	
void option5(Node_ast root){
	printf("ERRORS WHILE GENERATING TABLE:--\n");
	init_globalvars();
	createsymtable();
	createglobaltable();
	createrecordtable();
	//load all function names to symbol table and check overloading
	st_loadfunctions(root);
	//load all records - all typedefs into recordtable
	st_loadrecords(root);
		//load all global variables into variables into global table
	st_loadglobal(root);
	//load all input/output and local var
	st_load_IO_Local(root);

	print_global_symtable();
	printf("-----------------------------------------------------\n");
	printf("SYMBOL TABLE PRINT COMPLETE. PLEASE ENTER NEXT OPTION.\n");


}
void option6(Node_ast root){
	printf("ERRORS WHILE GENERATING TABLE:--\n");
	init_globalvars();
	createsymtable();
	createglobaltable();
	createrecordtable();
	//load all function names to symbol table and check overloading
	st_loadfunctions(root);
	//load all records - all typedefs into recordtable
	st_loadrecords(root);
		//load all global variables into variables into global table
	st_loadglobal(root);
	//load all input/output and local var
	st_load_IO_Local(root);

	printglobaltable();
	printf("-----------------------------------------------------\n");
	printf("GLOBAL VARIABLES TABLE PRINT COMPLETE. PLEASE ENTER NEXT OPTION.\n");


	
}
void option7(Node_ast root){
	printf("ERRORS WHILE GENERATING TABLE:--\n");
	init_globalvars();
	createsymtable();
	createglobaltable();
	createrecordtable();
	//load all function names to symbol table and check overloading
	st_loadfunctions(root);
	//load all records - all typedefs into recordtable
	st_loadrecords(root);
		//load all global variables into variables into global table
	st_loadglobal(root);
	//load all input/output and local var
	st_load_IO_Local(root);

	print_functionwidth_table();
	printf("-----------------------------------------------------\n");
	printf("FUNCTION WIDTH TABLE PRINT COMPLETE. PLEASE ENTER NEXT OPTION.\n");
	
}
void option8(Node_ast root){
	printf("ERRORS WHILE GENERATING TABLE:--\n");
	init_globalvars();
	createsymtable();
	createglobaltable();
	createrecordtable();
	//load all function names to symbol table and check overloading
	st_loadfunctions(root);
	//load all records - all typedefs into recordtable
	st_loadrecords(root);
		//load all global variables into variables into global table
	st_loadglobal(root);
	//load all input/output and local var
	st_load_IO_Local(root);

	printrecordtable();
	printf("-----------------------------------------------------\n");
	printf("RECORD TABLE PRINT COMPLETE. PLEASE ENTER NEXT OPTION.\n");
	
}

void option9(Node_ast root){
	
	printf("-SEMANTIC ERROR LIST:-----------\n");
	init_globalvars();
	createsymtable();
	createglobaltable();
	createrecordtable();
	//load all function names to symbol table and check overloading
	st_loadfunctions(root);
	//load all records - all typedefs into recordtable
	st_loadrecords(root);
		//load all global variables into variables into global table
	st_loadglobal(root);
	//load all input/output and local var
	st_load_IO_Local(root);

	st_otherstatements(root);
	printf("----------------------------\n");
	printf("SEMANTIC ANALYSIS COMPLETED.\n");
	
	/////////////////////////////////////


}

void option10(Node_ast root){

	init_globalvars();
	createsymtable();
	createglobaltable();
	createrecordtable();
	//load all function names to symbol table and check overloading
	st_loadfunctions(root);
	//load all records - all typedefs into recordtable
	st_loadrecords(root);
		//load all global variables into variables into global table
	st_loadglobal(root);
	//load all input/output and local var
	st_load_IO_Local(root);

	ast_help_create_table();

	ast_help_tablepopulate(root);

}







