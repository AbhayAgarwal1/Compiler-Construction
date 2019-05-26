#ifndef var_x1
#define var_x1

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>


typedef struct element *Element;
typedef struct seqList *SeqList;

struct seqList
{
	int count;
	struct element *head ;
	struct element *tail ;
};

struct element
{
	char* token;
	char* value;
	int lineno;
	struct element *next ;
};


SeqList newList();
Element newElement();
SeqList insertAtEnd(SeqList sl, Element e);

SeqList newList()
{
	return (SeqList) calloc (1,sizeof(struct seqList));
};

Element newElement(char* tok, char* val, int lin)
{
	Element e = (Element) calloc (1,sizeof(struct element));
	e->lineno = lin;
	e->token = tok;
	e->value = val;
	return e;
};

SeqList insertAtEnd(SeqList sl, Element e)
{
	if(sl->tail == NULL)
	{	
		sl->tail = e;
		sl->head = e;
		sl->count = 1;
		return sl;
	}
	sl->tail->next = e;
	sl->tail       = e;
	sl->count++;
	return sl;
};

struct iterator{
	struct seqList* lt;
	struct element* curr;

};

struct iterator* create_it(struct seqList* lt){

	struct iterator* it = malloc(sizeof(struct iterator));
	it->lt = lt;
	it->curr = lt->head;
	return it;
}

struct element* getNextToken(struct iterator* it){

	it->curr = it->curr->next;
	return it->curr;
}




char* getStream(FILE *fp);
Element getNextToken();

#endif
