#include "lexer.h"
#define MAXCHAR 1000


void preproccessing(int argc, char** argv){
	if(argc<2)
		printf("Error in passing the argument file");
	FILE *fptr = fopen(argv[1], "r");
	if(fptr==NULL){
		printf("Cannot open file");
		exit(0);
	}

	char line[MAXCHAR];
	while(fgets(line, MAXCHAR, fptr) !=NULL){
		char data[strlen(line)+1];
		int i = 0, j=0;
		while(line[i]==' ')
			i++;
		int flag = 1;
		while(line[i]!=0){
			if(line[i]=='%'){
				data[j++] = '\n';
				data[j] = 0;
				break;
			}
			if(line[i]==' '){
				if(flag==1)
					flag = 0;
				else{
					i++;
					continue;
				}
			}
			else
				flag = 1;
			data[j++] = line[i++];
		}
		data[j] = 0;
		printf("%s\n",data);
		// fputs(data, fptrw);
	}
	fclose(fptr);
}

char* getStream(FILE *fptr){
	char* line = (char*) malloc(MAXCHAR*sizeof(char));
	return fgets(line, MAXCHAR, fptr);
}

char* substring(char* line, int st, int end){
	char* str = (char*) calloc(end-st+1, sizeof(char));
	for(int i=0; i<=end-st; i++)
		str[i] = line[st+i];
	return str;
}

void printlis(SeqList lis){
	Element e = lis->head;
	while(e!=NULL){
		printf("Token = %s, Value = %s, LineNo = %d\n", e->token, e->value, e->lineno);
		e = e->next;
	}
}

void trim(char* str){
	for(int i=0; i<strlen(str); i++){
		if(str[i]==' ')
			str[i] = 0;
	}
}

void toupp(char* str){
	for(int i=0; str[i]!=0; i++){
		str[i] = str[i]-32;
	}
}

int checkkey(char* str, char table[23][15], SeqList lis, int lineno){
	//check this statement for size of keywords
	for(int i=0; i<23; i++){
		if(strcmp(str, table[i])==0){
			char* temp2 = (char*) calloc(25,sizeof(char));
			strcpy(temp2, str);
			toupp(str);
			char* temp = (char*) calloc(25,sizeof(char));
			strcpy(temp, "TK_");
			strcat(temp, str);
			insertAtEnd(lis, newElement(temp, temp2, lineno));
			return 1;
		}
	}
	return 0;
}

//function with error correction
SeqList lexer(int argc, char** argv){
	if(argc<2)
		printf("Number of argument not complete");
	FILE *fptr = fopen(argv[1], "r");
	if(fptr==NULL){
		printf("Cannot open file");
		exit(0);
	}
	FILE *filewr = fopen("errorlist.txt","w");
	char *line = getStream(fptr);
	int lineno = 1;
	char table[23][15] = {"with", "parameters", "end", "while", "type", "global", "parameter", "endwhile", "list", "input", "output", "int", "real", "if", "then", "endif", "read", "write", "return", "call", "record",
	"endrecord", "else"};
	//line = strcat(line, "  ");
	//0 is the start state of the DFA
	int state = 0;
	SeqList lis = newList();
	char* str;
	while(line !=NULL){
		int ind_st = 0, ind_en=0; //to keep track of the positions inside a line
		while(line[ind_en]!=0){
			char c = line[ind_en];
			if(c=='%'){
				//insertAtEnd(lis, newElement("TK_COMMENT", "%"));
				break;
			}
			switch(state){
				case 0: if((c>='e' && c<='z')||c=='a')
							state = 1;
						else if(c>='b'&&c<='d')
							state = 3;
						else if(c>='0'&&c<='9')
							state = 7;
						else if(c=='_')
							state = 12;
						else if(c=='#')
							state = 16;
						else if(c=='<')
							state = 19;
						else if(c=='=')
							state = 25;
						else if(c=='~')
							goto lab27;
						else if(c=='>')
							state = 28;
						else if(c=='!')
							state = 31;
						else if(c=='&')
							state = 33;
						else if(c=='@')
							state = 36;
						else if(c=='[')
							goto lab40;
						else if(c==']')
							goto lab41;
						else if(c==',')
							goto lab42;
						else if(c==';')
							goto lab43;
						else if(c==':')
							goto lab44;
						else if(c=='.')
							goto lab45;
						else if(c=='(')
							goto lab46;
						else if(c=='*')
							goto lab47;
						else if(c=='/')
							goto lab48;
						else if(c==')')
							goto lab49;
						else if(c=='+')
							goto lab50;
						else if(c=='-')
							goto lab51;
						else if(c=='%')
							goto lab39;
						else if(isspace(c)){
							ind_st++;
							state = 0;
						}
						else
							goto lab52;
						break;
				case 1: if(c>='a' && c<='z')
							state = 1;
						else
							goto lab2;
						break;
			lab2:
				case 2: str = substring(line, ind_st, ind_en-1);
						if(checkkey(str, table, lis, lineno)){
							ind_st = ind_en;
							ind_en--;
							state = 0;
							break;
						}
						insertAtEnd(lis, newElement("TK_FIELDID", str, lineno));
						ind_st = ind_en;
						ind_en--;
						state = 0;
						break;
				case 3: if(c>='a' && c<='z')
							state = 1;
						else if(c>='2' && c<='7')
							state = 4;
						else
							goto lab52;
						break;
				case 4: if(c>='b' && c<='d')
							state = 4;
						else if(c>='2' && c<='7')
							state = 5;
						else
							goto lab6;
						break;
				case 5: if(c>='2' && c<='7')
							state = 5;
						else
							goto lab6;
						break;
			lab6:
				case 6: str = substring(line, ind_st, ind_en-1);
						if(strlen(str)>20){
							fprintf(filewr, "Line: %d LEXICAL ERROR: MAXIMUM LENGTH OF ID VIOLATED\n", lineno);
							printf("Line: %d LEXICAL ERROR: MAXIMUM LENGTH OF ID VIOLATED\n", lineno);
						}
						insertAtEnd(lis, newElement("TK_ID", str, lineno));
						ind_st = ind_en;
						ind_en--;
						state = 0;
						break;
				case 7: if(c>='0' && c<='9')
							state = 7;
						else if(c=='.')
							state = 8;
						else
							goto lab11;
						break;
				case 8: if(c>='0' && c<='9')
							state = 9;
						else
							goto lab52;
						break;
				case 9: if(c>='0' && c<='9')
							goto lab10;
						else
							goto lab52;
						break;
						//NOT retracting
			lab10:
				case 10: str = substring(line, ind_st, ind_en);
						 insertAtEnd(lis, newElement("TK_RNUM", str, lineno));
						 ind_st = ind_en+1;
						 state = 0;
						 break;
						//retracting
			lab11:
				case 11: str = substring(line, ind_st, ind_en-1);
						 insertAtEnd(lis, newElement("TK_NUM", str, lineno));
						 ind_st = ind_en;
						 ind_en--;
						 state = 0;
						 break;
				case 12: if((c>='a' && c<='z')||(c>='A' && c<='Z'))
							state = 13;
						else
							goto lab52;
						 break;
				case 13: if((c>='a' && c<='z')||(c>='A' && c<='Z'))
							state = 13;
						 else if(c>='0' && c<='9')
						 	state = 14;
						 else
						 	goto lab15;
						 break;
				case 14: if(c>='0' && c<='9')
						 	state = 14;
						 else
						 	goto lab15;
						 break;
			lab15:
				case 15: str = substring(line, ind_st, ind_en-1);
						 if(strcmp("_main", str)==0){
						 	insertAtEnd(lis, newElement("TK_MAIN", str, lineno));
						 	ind_st = ind_en;
						 	ind_en--;
						 	state = 0;
						 	break;
						 }
						 insertAtEnd(lis, newElement("TK_FUNID", str, lineno));
						 ind_st = ind_en;
						 ind_en--;
						 state = 0;
						 break;
				case 16: if(c>='a' && c<='z')
							state = 17;
						else
							goto lab52;
						 break;
				case 17: if(c>='a' && c<='z')
							state = 17;
						 else
						 	goto lab18;
						 break;
			lab18:
				case 18: str = substring(line, ind_st, ind_en-1);
						 insertAtEnd(lis, newElement("TK_RECORDID", str, lineno));
						 ind_st = ind_en;
						 ind_en--;
						 state = 0;
						 break;
				case 19: if(c=='=')
						 	goto lab21;
						 else if(c=='-')
						 	state = 22;
						 else
						 	goto lab20;
						 break;
			lab20:
				case 20: str = substring(line, ind_st, ind_en-1);
						 insertAtEnd(lis, newElement("TK_LT", str, lineno));
						 ind_st = ind_en;
						 ind_en--;
						 state = 0;
						 break;
			lab21:
				case 21: str = substring(line, ind_st, ind_en);
						 insertAtEnd(lis, newElement("TK_LE", str, lineno));
						 ind_st = ind_en+1;
						 state = 0;
						 break;
				case 22: if(c=='-')
						 	state = 23;
						 else
							goto lab52;
						 break;
				case 23: if(c=='-')
							goto lab24;
						else
							goto lab52;
						 break;
			lab24:
				case 24: str = substring(line, ind_st, ind_en);
						 insertAtEnd(lis, newElement("TK_ASSIGNOP", str, lineno));
						 ind_st = ind_en+1;
						 state = 0;
						 break;
				case 25: if(c=='=')
							goto lab26;
						else
							goto lab52;
						 break;
			lab26:
				case 26: str = substring(line, ind_st, ind_en);
						 insertAtEnd(lis, newElement("TK_EQ", str, lineno));
						 ind_st = ind_en+1;
						 state = 0;
						 break;
			lab27:
				case 27: str = substring(line, ind_st, ind_en);
						 insertAtEnd(lis, newElement("TK_NOT", str, lineno));
						 ind_st = ind_en+1;
						 state = 0;
						 break;
				case 28: if(c=='=')
						 	goto lab30;
						 else
						 	goto lab29;
						 break;
			lab29:
				case 29: str = substring(line, ind_st, ind_en-1);
						 insertAtEnd(lis, newElement("TK_GT", str, lineno));
						 ind_st = ind_en;
						 ind_en--;
						 state = 0;
						 break;
			lab30:
				case 30: str = substring(line, ind_st, ind_en);
						 insertAtEnd(lis, newElement("TK_GE", str, lineno));
						 ind_st = ind_en+1;
						 state = 0;
						 break;
				case 31: if(c=='=')
						 	goto lab32;
						 else
							goto lab52;
						 break;
			lab32:
				case 32: str = substring(line, ind_st, ind_en);
						 insertAtEnd(lis, newElement("TK_NE", str, lineno));
						 ind_st = ind_en+1;
						 state = 0;
						 break;
				case 33: if(c=='&')
						 	state = 34;
						 else
							goto lab52;
						 break;
				case 34: if(c=='&')
						 	goto lab35;
						 else
							goto lab52;
			lab35:
				case 35: str = substring(line, ind_st, ind_en);
						 insertAtEnd(lis, newElement("TK_AND", str, lineno));
						 ind_st = ind_en+1;
						 state = 0;
						 break;
				case 36: if(c=='@')
						 	state = 37;
						 else
							goto lab52;
						 break;
				case 37: if(c=='@')
						 	goto lab38;
						 else
							goto lab52;
						 break;
			lab38:
				case 38: str = substring(line, ind_st, ind_en);
						 insertAtEnd(lis, newElement("TK_OR", str, lineno));
						 ind_st = ind_en+1;
						 state = 0;
						 break;
			lab39:
				case 39: str = substring(line, ind_st, ind_en);
						 insertAtEnd(lis, newElement("TK_COMMENT", str, lineno));
						 ind_st = ind_en+1;
						 state = 0;
						 break; 
			lab40:
				case 40: str = substring(line, ind_st, ind_en);
						 insertAtEnd(lis, newElement("TK_SQL", str, lineno));
						 ind_st = ind_en+1;
						 state = 0;
						 break;
			lab41:
				case 41: str = substring(line, ind_st, ind_en);
						 insertAtEnd(lis, newElement("TK_SQR", str, lineno));
						 ind_st = ind_en+1;
						 state = 0;
						 break;
			lab42:
				case 42: str = substring(line, ind_st, ind_en);
						 insertAtEnd(lis, newElement("TK_COMMA", str, lineno));
						 ind_st = ind_en+1;
						 state = 0;
						 break;
			lab43:
				case 43: str = substring(line, ind_st, ind_en);
						 insertAtEnd(lis, newElement("TK_SEM", str, lineno));
						 ind_st = ind_en+1;
						 state = 0;
						 break;
			lab44:
				case 44: str = substring(line, ind_st, ind_en);
						 insertAtEnd(lis, newElement("TK_COLON", str, lineno));
						 ind_st = ind_en+1;
						 state = 0;
						 break;
			lab45:
				case 45: str = substring(line, ind_st, ind_en);
						 insertAtEnd(lis, newElement("TK_DOT", str, lineno));
						 ind_st = ind_en+1;
						 state = 0;
						 break;
			lab46:
				case 46: str = substring(line, ind_st, ind_en);
						 insertAtEnd(lis, newElement("TK_OP", str, lineno));
						 ind_st = ind_en+1;
						 state = 0;
						 break;
			lab47:
				case 47: str = substring(line, ind_st, ind_en);
						 insertAtEnd(lis, newElement("TK_MUL", str, lineno));
						 ind_st = ind_en+1;
						 state = 0;
						 break;
			lab48:
				case 48: str = substring(line, ind_st, ind_en);
						 insertAtEnd(lis, newElement("TK_DIV", str, lineno));
						 ind_st = ind_en+1;
						 state = 0;
						 break;
			lab49:
				case 49: str = substring(line, ind_st, ind_en);
						 insertAtEnd(lis, newElement("TK_CL", str, lineno));
						 ind_st = ind_en+1;
						 state = 0;
						 break;
			lab50:
				case 50: str = substring(line, ind_st, ind_en);
						 insertAtEnd(lis, newElement("TK_PLUS", str, lineno));
						 ind_st = ind_en+1;
						 state = 0;
						 break;
			lab51:
				case 51: str = substring(line, ind_st, ind_en);
						 insertAtEnd(lis, newElement("TK_MINUS", str, lineno));
						 ind_st = ind_en+1;
						 state = 0;
						 break;
			lab52:
				case 52: fprintf(filewr,"Line: %d : LEXICAL ERROR: UNKNOWN PATTERN FOUND\n", lineno);
						 printf("Line: %d : LEXICAL ERROR: UNKNOWN PATTERN FOUND\n", lineno);
						 state = 0;
						 break;
			}
			ind_en++;
		}
		free(line);
		line = getStream(fptr);
		lineno++;
		if(line!=NULL)
			line = strcat(line, "  ");

	}
	//printlis(lis);
	fclose(fptr);
	fclose(filewr);
	
	insertAtEnd(lis, newElement("$","$", lineno));
	return lis;
}


/*
int main(int argc, char** argv){

	//preproccessing(argc, argv);
	SeqList final_symboltable = lexer(argc,argv);
	struct iterator* it = create_it(final_symboltable);

	printlis(final_symboltable);
	//printf("Token = %s, Value = %s\n", final_symboltable->tail->value, final_symboltable->tail->token);
	//
	
	

}
*/
