#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "stack.c"
//#include "list.c"
#include "parsetable.c"
#include "lexer.c"
#include "createparsetree.c"
#include "ast.c"
#include "symtable.c"
#include "code19_6.c"
#include <time.h>
extern int ast_no_ofnodes;
extern int parse_noofnodes;
int STACK_CAPACITY = 1000;
int flag=0;
int get_grammar_rule(char* Ter,char* nonTer,int** ParseTable){
	

	int row = find_NonTer_Num(nonTer);
	int col = find_Ter_Num(Ter);

	return ParseTable[row][col];


}

void pushTostack(int grammar_rule,char*** Grammar,Stack mainStack){

	char** arr = Grammar[grammar_rule];
	int j;
	int i=1;
	while(strcmp(Grammar[grammar_rule][i],"^")!=0){
		i=i+1;
	}
	for(j=i-1;j>0;j--){
		push(mainStack,Grammar[grammar_rule][j]);
	}

}


SeqList1* ParsingAlgo(int** ParseTable,char*** Grammar,struct iterator* iter){

	//<create a stack> name mainStack - elements string
	FILE *filewr = fopen("errorlist.txt","a");
	flag=0;
	Stack mainStack = createstack(STACK_CAPACITY);

	//<create a linked list> name finalResult - elements int
	SeqList1* finalResult = newList1();

	push(mainStack,"$");
	push(mainStack,"program"); // NOTE CHANGE HERE IN CASE YOU WILL CONSIDER Grammar NON-TERMINALS WITH <>

	char nextword[word_len];
	char stack_top[word_len];
	
	struct element* curr_ele = iter->curr;

	strcpy(nextword,gettoken(curr_ele));
	strcpy(stack_top,top(mainStack));
	int grammar_rule;
	
		
	while(strcmp(stack_top,"$")!=0){

		if(strcmp(stack_top,nextword)==0){
			pop(mainStack);
			curr_ele = getNextToken(iter);
			strcpy(nextword,gettoken(curr_ele));
		}
		else if(isTerminal(stack_top)){flag=1;
			printf("%s%d%s%s\n","Line: ",getlineno(curr_ele),": PARSING ERROR-TERMINAL NOT MATCHING-POP THE TERMINAL FROM STACK, POP : ",stack_top);
			fprintf(filewr,"%s%d%s%s\n","Line: ",getlineno(curr_ele),": PARSING ERROR-TERMINAL NOT MATCHING-POP THE TERMINAL FROM STACK, POP : ",stack_top);
			pop(mainStack);
		}
		else{
			grammar_rule = get_grammar_rule(nextword,stack_top,ParseTable);
 
			if(grammar_rule==-1){flag=1;
				printf("%s%d%s%s%s%s]\n","Line: ",getlineno(curr_ele),": PARSING ERROR-SKIP INPUT TILL SYN FOUND,SKIP [LEXEME : ",getvalue(curr_ele)," , Token - ",gettoken(curr_ele));
				fprintf(filewr,"%s%d%s%s%s%s]\n","Line: ",getlineno(curr_ele),": PARSING ERROR-SKIP INPUT TILL SYN FOUND,SKIP [LEXEME : ",getvalue(curr_ele)," , Token - ",gettoken(curr_ele));
				curr_ele = getNextToken(iter);
				strcpy(nextword,gettoken(curr_ele));
			}
			else if(grammar_rule==-2){flag=1;
				printf("%s%d%s%s\n","Line: ",getlineno(curr_ele),": PARSING ERROR-SYN FOUND-REMOVE NON-TERMINAL FROM STACK, POP : ",stack_top);
				fprintf(filewr,"%s%d%s%s\n","Line: ",getlineno(curr_ele),": PARSING ERROR-SYN FOUND-REMOVE NON-TERMINAL FROM STACK, POP : ",stack_top);
				pop(mainStack);
			}
			else{
				
				insertIntoList(finalResult,newElement1(grammar_rule));
				pop(mainStack);
				pushTostack(grammar_rule,Grammar,mainStack);
				if(strcmp(top(mainStack),"eps")==0){
					pop(mainStack);
				}
			}
		}
		
		strcpy(stack_top,top(mainStack));	
	}		

	
	fclose(filewr);

	//////////



	return finalResult;
}

int main(int argc, char** argv)
{

	
	
	double total_cpu_time;
	Node root;
	double total_cpu_time_sec;
	int option;
	SeqList final_symboltable ;
	struct iterator* iter;
	char*** gram_array;
	int** table_first;int** table_follow;int** tempTable;int** parsetable;SeqList1* finalList;
	

	
			final_symboltable = lexer(argc,argv);
			
		
			iter = create_it(final_symboltable);

	           
			gram_array = read_grammer(); 
	
			table_first = get_firsttable();

	
			table_follow = get_followtable();

			tempTable = generateParseTable(gram_array,table_first,table_follow);

			parsetable = finalParseTableGen(tempTable,table_follow);
	
			finalList = ParsingAlgo(parsetable,gram_array,iter);

			FILE* fp=fopen(argv[2],"w");
			struct iterator* iter1 = create_it(final_symboltable);
			struct iterator* iter2 = create_it(final_symboltable);	
			Tree_ast ast_temp;
			if(flag == 0){
				root = returnroot(finalList,gram_array,fp,iter1,iter2);
				ast_temp = createast(root);
			}
	FILE *fp1;
	printf("Welcome to the compiler console\n");
	printf("Group 43, Team members:-\n");
	printf("Rishabh Jain - 2015B5A70550P\n");
	printf("Akhil Agarwal - 2015B4A70631P\n");
	printf("Abhay Agarwal - 2015B3A70512P\n");
	printf("Anirudha K.- 2015B3A70626P\n");
	int choice = 0;
	printf("Enter your choices in a loop to begin the execution\n");
	while(1){
		printf("Enter your next choice: \n");
		iter1 = create_it(final_symboltable);
		scanf("%d", &choice);
		switch(choice){
			case 0: return 0;
			case 1: printlis(final_symboltable);
					break;
			case 2: if(flag==0){
						printparsetree(root, fp, iter1);
						printf("Parsing has been performed successfully\n");
					}
					else
						printf("Parsing errors are there. Stopping compilation.\n");
					break;
			case 3: if(flag==0)
					print_ast(ast_temp);
					break;
			case 4: if(flag ==0){
						printf("Number of nodes in ast = %d\n", ast_no_ofnodes);
						printf("Size of individual node of ast = 24\n");
						printf("Number of nodes in parse tree = %d\n", parse_noofnodes);
						printf("Size of individual parse tree node= 24\n");
						printf("Percentage saving in space is %f\n", (float)(parse_noofnodes-ast_no_ofnodes)/parse_noofnodes);
					}

					break;
			case 5: if(flag==0)
						option5(ast_temp->root);
					break;
			case 6: if(flag==0)
						option6(ast_temp->root);
					break;
			case 7: if(flag==0)
						option7(ast_temp->root);
					break;
			case 8: if(flag==0)
						option8(ast_temp->root);
					break;
			case 9: if(flag==0)
						option9(ast_temp->root);
					break;

			case 10: if(flag==0){
						 option10(ast_temp->root);
						 fp1 = fopen(argv[2],"w");
						 code_generation(ast_temp,fp1);
						 printf("assembly code generated successfully in file\n");
						 fclose(fp1);
					}
					break;
		}
	}
	fclose(fp);


	return 0;
}







