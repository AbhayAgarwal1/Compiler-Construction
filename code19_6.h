int get_grammar_rule(char* Ter,char* nonTer,int** ParseTable);
void printNewline(FILE*fp);
void printDecimalNumber(FILE* fp);
void printNegPos(FILE*fp);
void read1(FILE* fp);
void io_(Node_ast ast,FILE* fp);
void iter_(Node_ast ast,FILE *fp);
void declaration_(Node_ast temp,FILE *fp);
void assign_(Node_ast ast,FILE *fp);
void conditional_(Node_ast ast,FILE* fp);
char* get_node_name(Node_ast nod);
void arith_(Node_ast ast,FILE* fp);
void bool_(Node_ast ast,FILE *fp);
void stmt_(Node_ast stmt,FILE* fp);
void code_generation(Tree_ast ast,FILE* fp);