#include <stdio.h>
#include <stdlib.h>

struct Element1;
typedef struct Element1 Element1;
struct Element1{
	int data;
	Element1 *next;
};

typedef struct {
	
	Element1 *head;
	Element1 *tail;
}SeqList1;


Element1* newElement1(int i){
	Element1 *t = (Element1*)malloc(sizeof(Element1));
	t->data = i;
	t->next = NULL;
	return t;
}

SeqList1* newList1(){
	SeqList1 *t = (SeqList1*)malloc(sizeof(SeqList1));
	
	t->head=NULL;
	t->tail=NULL;
	return t;
}





void insertIntoList(SeqList1 *sl, Element1 *e){
	if(sl->head==NULL){
		sl->head=e;
		
		sl->tail=e;
	}
	else{
		sl->tail->next=e;
		sl->tail = e;
			
	}

}



