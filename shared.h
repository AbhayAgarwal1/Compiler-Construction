#ifndef var_x5
#define var_x5

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


#endif