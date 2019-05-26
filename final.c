#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "stack.c"
//#include "list.c"
#include "parsetable.c"
#include "lexer.c"
#include "createparsetree.c"
#include <time.h>
int STACK_CAPACITY = 1000;

int get_grammar_rule(char* Ter,char* nonTer,int** ParseTable){
	

	int row = find_NonTer_Num(nonTer);
	int col = find_Ter_Num(Ter);

	return ParseTable[row][col];


}
int flag =0;
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
		else if(isTerminal(stack_top)){
			printf("%s%d%s%s\n","Line: ",getlineno(curr_ele),": PARSING ERROR-TERMINAL NOT MATCHING-POP THE TERMINAL FROM STACK, POP : ",stack_top);
			fprintf(filewr,"%s%d%s%s\n","Line: ",getlineno(curr_ele),": PARSING ERROR-TERMINAL NOT MATCHING-POP THE TERMINAL FROM STACK, POP : ",stack_top);
			pop(mainStack);
		}
		else{
			grammar_rule = get_grammar_rule(nextword,stack_top,ParseTable);
 			
			if(grammar_rule==-1){flag = 1;
				printf("%s%d%s%s%s%s]\n","Line: ",getlineno(curr_ele),": PARSING ERROR-SKIP INPUT TILL SYN FOUND,SKIP [LEXEME : ",getvalue(curr_ele)," , Token - ",gettoken(curr_ele));
				fprintf(filewr,"%s%d%s%s%s%s]\n","Line: ",getlineno(curr_ele),": PARSING ERROR-SKIP INPUT TILL SYN FOUND,SKIP [LEXEME : ",getvalue(curr_ele)," , Token - ",gettoken(curr_ele));
				curr_ele = getNextToken(iter);
				strcpy(nextword,gettoken(curr_ele));
			}
			else if(grammar_rule==-2){flag = 1;
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
	double total_cpu_time_sec;
	int option;
	SeqList final_symboltable ;
	struct iterator* iter;
	char*** gram_array;
	int** table_first;int** table_follow;int** tempTable;int** parsetable;SeqList1* finalList;
	while(1)
	{	printf("Please enter an option to choose defined tasks:");
		scanf("%d",&option);
		if(option==0)exit(0);
		if(option==1){preproccessing(argc,argv);}
		if(option==2){final_symboltable = lexer(argc,argv);
					printlis(final_symboltable);

			     }
		if(option==3){
			clock_t starttime,endtime;
	
			starttime=clock();
	
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
			printparsetree(finalList,gram_array,fp,iter1);

			 	fclose(fp);
				endtime=clock();
			total_cpu_time=(double)(endtime-starttime);
			total_cpu_time_sec=total_cpu_time/CLOCKS_PER_SEC;
		 			}
		if(option==4){
			printf("time:%fclock cycles time:%fseconds\n",total_cpu_time,total_cpu_time_sec);
		}

	}

	return 0;
		}







