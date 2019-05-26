/*
Akhil Agrawal 2015B4A70631P
ABHAY AGARWAL 2015B3A70512P
RISHABH JAIN 2015B5A70550P
ANIRUDHA K 2015B3A70626P
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int NonTer_Nos = 51;
int Ter_Nos = 57;
int Grammer_rows = 92;
int Grammer_columns = 20;
int word_len=30;

char* getlhs(char* line)
{
	char* lhs=(char*)malloc(sizeof(char)*word_len);
	int i=0,j=0;
	
	while(line[i]!='=')
	{	
		lhs[j]=line[i];
		j++;
		i++;
	}
	lhs[j]='\0';
	
	return lhs;
}


char* getrhs(char* line)
{
	char* rhs=(char*)malloc(sizeof(char)*200);
	int i=0,j=0;
	while(line[i]!='=')
	{
		i++;
	}
	i+=3;
	while(line[i]!='\n')
	{
		rhs[j]=line[i];
		i++;
		j++;
	}
	rhs[j]='\0';
	return rhs;
}
char*** read_grammer()
{
	FILE* fp;
	char* line = (char*)malloc(100*sizeof(char));
	char* lhs;
	char* rhs;

	int i,j=0;

	char*** gram_array= (char***)malloc(Grammer_rows*sizeof(char**));
	for(i=0;i<Grammer_rows;i++){
		gram_array[i] = (char**)malloc(Grammer_columns*sizeof(char*));

		for(j=0;j<Grammer_columns;j++){
			gram_array[i][j] = (char*)malloc(word_len*sizeof(char));
		}
	}


	i=0;
	fp=fopen("Grammar.txt","r");
	if(fp==NULL){printf("error");}

	while(fgets(line,100,fp))
	{	
		
		lhs=getlhs(line);
		rhs=getrhs(line);
		// printf("%s\n%s\n",lhs,rhs);
		strcpy(gram_array[i][0],lhs);
		char *tok=strtok(rhs," ");
		j=1;
		while(tok!=NULL)
		{        
			strcpy(gram_array[i][j],tok);
			tok=strtok(NULL," ");
			j++;
		}	
		strcpy(gram_array[i][j],"^");
		i++;
	}

	/*
	for(i=0;i<Grammer_rows;i++)
	{
		j=0;
		while(strcmp(gram_array[i][j],"^")!=0){printf("%s ",gram_array[i][j]);j++;}
		printf("\n");
	}
	*/
	fclose(fp);	
	
	return gram_array;
}

char terminal[57][30] = {"eps","$","TK_ASSIGNOP" ,"TK_COMMENT", "TK_FIELDID" ,"TK_ID" ,"TK_NUM", "TK_RNUM","TK_FUNID" ,"TK_RECORDID" ,"TK_WITH" ,"TK_PARAMETERS" ,"TK_END" ,"TK_WHILE", "TK_TYPE", "TK_MAIN", "TK_GLOBAL", "TK_PARAMETER" ,"TK_LIST" ,"TK_SQL" ,"TK_SQR", "TK_INPUT" ,"TK_OUTPUT" ,"TK_INT", "TK_REAL" ,"TK_COMMA", "TK_SEM", "TK_COLON", "TK_DOT", "TK_ENDWHILE", "TK_OP" ,"TK_CL" ,"TK_IF" ,"TK_THEN" ,"TK_ENDIF", "TK_READ", "TK_WRITE", "TK_RETURN", "TK_PLUS" ,"TK_MINUS" ,"TK_MUL" ,"TK_DIV" ,"TK_CALL" ,"TK_RECORD", "TK_ENDRECORD", "TK_ELSE" ,"TK_AND" ,"TK_OR" ,"TK_NOT" ,"TK_LT" ,"TK_LE", "TK_EQ", "TK_GT" ,"TK_NE","TK_GE"};
char nonterminal[51][30] = {"function","otherFunctions","mainFunction","program","typeDefinition","typeDefinitions","declaration","declarations","SingleOrRecId","assignmentStmt","iterativeStmt","conditionalStmt","ioStmt","outputParameters","funCallStmt","stmt","otherStmts","returnStmt","stmts","input_par","output_par","primitiveDatatype","constructedDatatype","dataType","parameter_list","remaining_list","fieldDefinition","fieldDefinitions","moreFields","global_or_not","all","factor","term","arithmeticExpression","alpha","inputParameters","idList","var","booleanExpression","elsePart","allVar","newVar","lowPrecedenceOperators","expPrime","highPrecedenceOperators","termPrime","temp","logicalOp","relationalOp"
,"optionalReturn","more_ids"};

int index_id(char* str){
	
	for(int i=0;i<57;i++){

		if (strcmp(terminal[i],str)==0){
			//printf("%d\n",i);
			return i;
		}
		
	}
	

}
int** get_firsttable(){

	FILE* fp;
	char* line = (char*)malloc(300*sizeof(char));
	int **table = (int **)malloc(NonTer_Nos * sizeof(int *)); 

    for (int i=0; i<NonTer_Nos; i++) 
         table[i] = (int *)malloc(25 * sizeof(int)); 
	fp=fopen("first_f","r");
	if(fp==NULL){printf("error");}
	int i=0;
	for(int i=0;i<NonTer_Nos;i++){
		for(int j=0;j<25;j++){
			table[i][j]=-1;
		}
		//printf("\n");
	}


	while(fgets(line,300,fp))
	{	
		int k=0;
		char*p = strtok(line, " \n");
		//strcpy(nonterminal[i],p);
		for (p = strtok(NULL," \n"); p != NULL; p = strtok(NULL, " \n"))
	{		
			table[i][k++] = index_id(p);
  			
	}
	//table[i][k]=-1;
	i++;

	}	
	/*
	//printf("\n");
	for(int i=0;i<NonTer_Nos;i++){
		for(int j=0;j<25;j++){
			printf("%d ",table[i][j]);
		}
		printf("\n");
	}
	
	*/
	return table;
}


int** get_followtable(){

	FILE* fp;
	char* line=(char*)malloc(300*sizeof(char));

	int **table = (int **)malloc(NonTer_Nos * sizeof(int *)); 
    for (int i=0; i<NonTer_Nos; i++) 
         table[i] = (int *)malloc(25 * sizeof(int)); 
	fp=fopen("follow_f","r");
	if(fp==NULL){printf("error");}
	int i=0;
	for(int i=0;i<NonTer_Nos;i++){
		for(int j=0;j<25;j++){
			table[i][j]=-1;
		}
		
	}
	

	while(fgets(line,300,fp))
	{	
		int k=0;
		char*p = strtok(line, " \n");
		//strcpy(nonterminal[i],p);
		for (p = strtok(NULL," \n"); p != NULL; p = strtok(NULL, " \n"))
	{		
			table[i][k++] = index_id(p);
  			
	}
	//table[i][k]=-1;
	i++;

	}	
	/*
	//printf("\n");
	for(int i=0;i<NonTer_Nos;i++){
		for(int j=0;j<25;j++){
			printf("%d ",table[i][j]);
		}
		printf("\n");
	}
	*/

	return table;
	
}

/*
int main(){

	//int table = get_firsttable();
	//read_grammer();
	int** table_first = get_firsttable();
	int** table_follow = get_followtable();

	for(int i=0;;i++){
		if
	}
}
*/




int isTerminal(char* str){

		/*
		2-eps
		1-other terminals
		0-non terminals
		*/

		if((strcmp(str,"$")==0)||(strcmp(str,"eps")==0)){
			return 1;
		}
	
		else{
			if(str[0]=='T'){
				if(str[1]=='K'){
					if(str[2]=='_'){
						return 1;
					}
				}
			}

		}
		return 0;
}

int* getfirst(int i, int** table_first){

	return table_first[i];

}

int* getfollow(int i,int** table_follow){

	return table_follow[i];

}

int find_NonTer_Num(char* str){

	for(int i=0;i<NonTer_Nos;i++){
		if(strcmp(str,nonterminal[i])==0){
			return i;
		}
	}	
}

int find_Ter_Num(char* str){

	for(int i= 0;i<Ter_Nos;i++){
		if(strcmp(str,terminal[i])==0){
			return i;
		}
	}	
}


int* getfirstall(char* arr,int** table_first){
	if(isTerminal(arr)==1){
		int* temparr = (int*)malloc(2*sizeof(int));
		temparr[0] = find_Ter_Num(arr);
		temparr[1] = -1;
		return temparr;
	}
	else{

		int* temparr= getfirst(find_NonTer_Num(arr),table_first);
		return temparr;
	}
}

int* getFirstExpression(char** arr,int** table_first){




	if((isTerminal(arr[1])==1)&&(strcmp(arr[1],"eps")!=0)){
		int* temparr = (int*)malloc(2*sizeof(int));
		temparr[0] = find_Ter_Num(arr[1]);
		temparr[1] = -1;
		return temparr;
	}
	else{

		int j=1;
		int i;
		int has_eps=1;
		int* temparr = (int*)malloc(3*Ter_Nos*sizeof(int));
		int* temparr2;
		for (i = 0; i < (3*Ter_Nos); i++)
		{
			temparr[i]=-1;
		}
		i=0;
		int k;

		


		while((strcmp(arr[j],"^")!=0)&&(has_eps==1)){



			if(strcmp(arr[j],"eps")==0){
				temparr[i]=0;
				i=i+1;
				j=j+1;
			}
			else{
				has_eps=0;

				temparr2 = getfirstall(arr[j],table_first);
				k=0;
				while(temparr2[k]!=-1){
					
					if(temparr2[k]==0){
						has_eps=1;
					}
					
					temparr[i]=temparr2[k];
					i = i+1;
					k=k+1;
					

				}
				j=j+1;

			}
			


		}		
		
		

		return temparr;
	}
}
	




int** generateParseTable(char*** gram_array,int** table_first,int** table_follow){


	int temp_var;


	int* temp_firsts;
	int* temp_follows;

	int** ParseTable = (int**)malloc(sizeof(int*)*NonTer_Nos);
	for(int i=0;i<NonTer_Nos;i++){
		ParseTable[i] = (int*)malloc(sizeof(int)*Ter_Nos);
	}



	/*
	error = -1;
	syn = -2;
	*/

	/*
	eps = 0;
	$ = 1;

	*/

	int i,j;
	for(i=0;i<NonTer_Nos;i++){
		for(j=0;j<Ter_Nos;j++){
			ParseTable[i][j]=-1;
		}
	}

	
	int has_eps,has_dollar;
	int Row_num,col_num;
	for(i=0;i<Grammer_rows;i++){
		
		has_eps=0;
		has_dollar=0;
		Row_num=find_NonTer_Num(gram_array[i][0]);
		temp_firsts = getFirstExpression(gram_array[i],table_first);

		
		j=0;
		temp_var = temp_firsts[j];


		while(temp_var!=-1){
			
			if(temp_var==0){
				has_eps=1;
			}
			else{
				if(ParseTable[Row_num][temp_var]==-1){
					ParseTable[Row_num][temp_var]=i;
				}
				else{
					ParseTable[Row_num][temp_var]=i;
				}
			}
			j=j+1;
			temp_var = temp_firsts[j];
		}
		

		if(has_eps==1){

			temp_follows = getfollow(Row_num,table_follow);
				
			j=0;
			temp_var = temp_follows[j];
			while(temp_var!=-1){
				
				if(ParseTable[Row_num][temp_var]==-1){
					ParseTable[Row_num][temp_var]=i;
				}
				else{
					ParseTable[Row_num][temp_var]=i;
				}

				j=j+1;
				temp_var = temp_follows[j];
				
			}
		}
		
	}

	return ParseTable;


}

int** finalParseTableGen(int** ParseTable,int** table_follow){



	int i,j;
	
	int temp_var;
	int* temp_follows;

	for(i=0;i<NonTer_Nos;i++){
		
		temp_follows = getfollow(i,table_follow);
		
		j=0;
		temp_var = temp_follows[j];
		while(temp_var!=-1){
			if(ParseTable[i][temp_var]==-1){
				ParseTable[i][temp_var]=-2;
			}
			j=j+1;
			temp_var=temp_follows[j];
		}


	}

	return ParseTable;


}
/*
int main(){
	char*** gram_array = read_grammer();
	
	int** table_first = get_firsttable();

	
	int** table_follow = get_followtable();

	int** ParseTable = generateParseTable(gram_array,table_first,table_follow);
	

	
}*/

