#include<stdio.h>
#include<stdlib.h>
#include "code19_6.h"


int no_while=0;
int no_rop =0;
int no_cond=0;

void print_num(FILE *fp){
	fprintf(fp,"_print:\n");
	fprintf(fp,"section\t.bss\n");
	fprintf(fp,".number\tresb\t10\n");
	fprintf(fp,".val\tresq\t1\n");
	fprintf(fp,".minus\tresb\t1\n");
	fprintf(fp,".newline\tresb\t1\n\n");
	fprintf(fp,"section .text\n");
	fprintf(fp,"\t\tPush rax\n");
	fprintf(fp,"\t\tpush rbx\n");
	fprintf(fp,"\t\tpush rcx\n");
	fprintf(fp,"\t\tpush rdx\n");
	fprintf(fp,"\t\tpush rsi\n");
	fprintf(fp,"\t\tpush rdi\n");
	fprintf(fp,"\t\tcmp rax, 0\n");
	fprintf(fp,"\t\tjge .X1\n");
	fprintf(fp,"\t\tneg rax\n");
	fprintf(fp,"\t\tPUSH rax\n");
	fprintf(fp,"\t\tmov rdi, .minus\n");
	fprintf(fp,"\t\tmov		 rdi, .minus\n");
	fprintf(fp,"\t\tmov byte[rdi], '-'\n");
	fprintf(fp,"\t\tmov rax,4\n");
	fprintf(fp,"\t\tmov rbx, 1\nmov rcx, rdi\n");
	fprintf(fp,"\t\tmov rcx, rdi\n");
	fprintf(fp,"\t\tmov rdx, 1\n");
	fprintf(fp,"\t\tint 0x80 \n");
	fprintf(fp,"\t\tpop rax \
\n.X1:	mov		 qword[.val],0\
\n		mov		 rdi, .number\
\n		add		 rdi,9\
\n    	mov      rdx, 0\
\n.loopzero:\
\n		mov		 rbx,10\
\n		div		 rbx	\
\n		add		 rdx,'0'     ; rdx has single digit of the number\
\n		mov		 byte[rdi],dl\
\n		dec		 rdi\
\n		inc		 qword[.val]\
\n		mov      rdx, 0\
\n		cmp		 rax,0\
\n		jne		 .loopzero\
\n		inc		 rdi\
\n		mov		 rcx, rdi\
\n		mov		 rdx, [.val]\
\n		mov		 rax, 4\
\n		mov		 rbx, 1\
\n		int		 0x80\
\n		;syscall\
\n		mov		 rdi, .newline\
\n		mov byte[rdi], 10\
\n		mov rax,4\
\n		mov rbx, 1\
\n		mov rcx, rdi\
\n		mov rdx, 1\
\n		int 0x80\
\n		;syscall\
\n		Pop rax\
\n		Pop rbx\
\n		Pop rcx\
\n		Pop rdx\
\n		Pop rsi\
\n		Pop rdi\
\n		ret\n");
}

void input_num(FILE* fp){
fprintf(fp,"\n_input:\
\nsection .bss\
\nbuff	resb	100\
\ninp 	resq	1\
\nisneg	resb	1\
\nsection .text\
\n		Push rax\
\n		push rbx\
\n		push rcx\
\n		push rdx\
\n		push rsi\
\n		push rdi\
\n		mov	rsi, buff\
\n		mov	rdi, 0	               ; number of digits stored in rdi\
\n.loop1:\
\n		mov	rax, 03\
\n		mov	rbx, 0\
\n		mov	rcx, rsi\
\n		mov	rdx, 1\
\n		int	0x80\
\n		cmp	byte[rsi], 0\
\n		je	 .parse\
\n		cmp	byte[rsi], 10\
\n  	je	 .parse\
\n		inc	rsi\
\n		inc	rdi\
\n		jmp	.loop1\
\n.parse:\
\n		mov	rsi, buff\
\n		mov	rcx, rdi\
\n		mov	qword[inp], 0\
\n		mov	byte[isneg], 0\
\n		cmp	byte[rsi], '-'            ; checking if negatve\
\n		jne	.loop\
\n		inc	rsi\
\n		inc	byte[isneg]\
\n.loop:	mov	rbx, 0\
\n		mov	bl, byte[rsi]\
\n		cmp	bl, 10\
\n		je	 .Y                  ; if end of string (new line character) is reached, then done\
\n  	mov	rdx, 10\
\n  	mov	rax, qword[inp]\
\n		mul	rdx\
\n 		sub	bl, '0'\
\n		add	rax, rbx\
\n		mov	qword[inp], rax\
\n		inc	rsi\
\n		dec rcx\
\n		jnz .loop\
\n.Y:\
\n  	cmp	byte[isneg], 0\
\n		je	.endip\
\n		neg	rax\
\n		;mov rbx, [isneg]\
\n		mov	qword [inp], rax\
\n.endip:\
\n		Pop rax\
\n		Pop rbx\
\n		Pop rcx\
\n		Pop rdx\
\n		Pop rsi\
\n		Pop rdi\
\n		mov	rax, [inp]\
\n		ret\n");
}


void io_(Node_ast ast,FILE* fp){
	
	if(strcmp(ast->children->str,"TK_READ")==0){
		fprintf(fp,"call _input\n");
		fprintf(fp,"mov qword[%s],rax\n",ast->children->sibling->children->ele->value);
		

	}
	else{
		fprintf(fp,"mov rax,qword[%s]\n",ast->children->sibling->children->ele->value);
		fprintf(fp,"call _print\n");
		
		

	}
}

void iter_(Node_ast ast,FILE *fp){
	fprintf(fp, ".%dloop:\n",no_while );;

	bool_(ast->children,fp);
	fprintf(fp,"pop rax\n cmp rax, 0000h\n je .%dendloop\n",no_while);
	stmt_(ast->children->sibling,fp);

	if(ast->children->sibling!=NULL && ast->children->sibling->sibling!=NULL){
		Node_ast temp = ast->children->sibling->sibling->children;
		while(temp!=NULL){

			stmt_(temp, fp);
			temp = temp->sibling;
		}
	}



	fprintf(fp,"jmp .%dloop\n",no_while);
	fprintf(fp,".%dendloop:\n",no_while);
	no_while += 1;
}

void declaration_(Node_ast temp,FILE *fp){
	
	temp = temp->children;

	while(temp!=NULL){

		
		

			fprintf(fp,"%s resq 1\n",temp->children->sibling->ele->value);
		
		temp = temp->sibling;
	}


}

void assign_(Node_ast ast,FILE *fp){

	arith_(ast->children->sibling,fp);


	
	// push and pop to be included

	fprintf(fp,"pop rax\nmov qword[%s],rax\n",ast->children->children->ele->value);

}

void conditional_(Node_ast ast,FILE* fp){

	bool_(ast->children,fp);
	fprintf(fp,"pop rax\ncmp rax,0000h\nje\telselabel%d\n",no_cond);
	Node_ast temp = ast->children->sibling;
	if(temp!=NULL)
	stmt_(temp, fp);
	if(temp->sibling!=NULL && strcmp(temp->sibling->str,"otherStmts")==0){
		Node_ast temp1 = temp->sibling->children;
		while(temp1!=NULL){
			stmt_(temp1, fp);
			temp1 = temp1->sibling;
		}

	}
	fprintf(fp,"jmp endiflabel%d\nelselabel%d:\n",no_cond,no_cond);
	temp = ast->children->sibling->sibling;
	Node_ast temp1;
	if(temp!=NULL){
		if(strcmp(temp->str,"otherStmts")==0 && temp->sibling!=NULL){
			temp1 = temp->sibling->children;
		}
	}
if(temp1!=NULL){
	//printf("rn:%s",temp1->str);
	stmt_(temp1,fp);
	if(temp1->sibling!=NULL&&strcmp(temp1->sibling->str,"otherStmts")==0){
		Node_ast temp2 = temp1->sibling->children;
		while(temp2!=NULL){
			stmt_(temp2, fp);
			temp2 = temp2->sibling;
		}

	}
	}

	fprintf(fp,"endiflabel%d:\n",no_cond);
	no_cond+=1;

}

char* get_node_name(Node_ast nod){
	if(nod->tag==2)
		return nod->str;
	else if(nod->tag==1)
		return nod->ele->token;
}


void arith_(Node_ast ast,FILE* fp){

		//
	if(strcmp(get_node_name(ast),"arithmeticExpression")==0){
		arith_(ast->children,fp);

	}
	else if(strcmp(get_node_name(ast),"var")==0){
		arith_(ast->children, fp);

	}
	else if(strcmp(get_node_name(ast),"term")==0){
		arith_(ast->children,fp);
		if(ast->sibling!=NULL){
			arith_(ast->sibling->children,fp);
			arith_(ast->sibling,fp);
		}

	}
	else if(strcmp(get_node_name(ast),"factor")==0){
		arith_(ast->children,fp);
		if(ast->sibling!=NULL){
			arith_(ast->sibling->children,fp);
			arith_(ast->sibling,fp);
		}

	}
	else if(strcmp(get_node_name(ast),"TK_PLUS")==0){
		fprintf(fp,"pop rbx\n");
			fprintf(fp,"pop rax\n");
			fprintf(fp,"add rax,rbx\n");
			fprintf(fp,"push rax\n");
		


		

	}
	else if(strcmp(get_node_name(ast),"TK_MINUS")==0){
		fprintf(fp,"pop rbx\n");
			fprintf(fp,"pop rax\n");
			fprintf(fp,"sub rax,rbx\n");
			fprintf(fp,"push rax\n");

	}
	else if(strcmp(get_node_name(ast),"TK_DIV")==0){
		
			fprintf(fp,"pop rbx\n");
			fprintf(fp,"pop rax\n");
			fprintf(fp,"push edx\n");
			fprintf(fp,"mov edx,0\n");
			fprintf(fp,"idiv rbx\n");
			fprintf(fp,"pop edx\n");
			fprintf(fp, "push rax\n");

	}
	else if(strcmp(get_node_name(ast),"TK_MUL")==0){
		
			fprintf(fp, "pop rbx\n");
			fprintf(fp,"pop rax\n");
			fprintf(fp,"imul rax,rbx\n");
			fprintf(fp,"push rax\n");

	}
	else if(strcmp(get_node_name(ast),"TK_ID")==0){
		fprintf(fp,"push qword[%s]\n",ast->ele->value);

	}
	else if(strcmp(get_node_name(ast),"TK_NUM")==0){
		fprintf(fp,"mov rax,%s\n",ast->ele->value);
			fprintf(fp,"push rax\n");


	}
	else if(strcmp(get_node_name(ast),"TK_RNUM")==0){
		fprintf(fp,"mov rax,%s\n",ast->ele->value);
			fprintf(fp,"push rax\n");

	}





}
void bool_(Node_ast ast,FILE *fp){
	if(strcmp(ast->children->str,"logicalOp")==0){
		if(strcmp(ast->children->children->str,"TK_OR")==0){
		bool_(ast->children->sibling,fp);
		bool_(ast->children->sibling->sibling,fp);
		fprintf(fp, "pop rbx\n");
		fprintf(fp,"pop rax\n");
		fprintf(fp,"or rax, rbx\n");
		fprintf(fp,"push rax\n");
		}
		else if(strcmp(ast->children->children->str,"TK_AND")==0){
		bool_(ast->children->sibling,fp);
		bool_(ast->children->sibling->sibling,fp);
		fprintf(fp, "pop rbx\n");
		fprintf(fp,"pop rax\n");
		fprintf(fp,"and rax, rbx\n");
		fprintf(fp,"push rax\n");

		}

	}
	else if(strcmp(ast->children->str,"relationalOp")==0){
		
		if(strcmp(ast->children->children->str,"TK_LT")==0){
			arith_(ast->children->sibling,fp);
			arith_(ast->children->sibling->sibling,fp);

			fprintf(fp, "pop rbx\n");
		fprintf(fp,"pop rax\n");
		fprintf(fp,"push 0ffffh\n");
		fprintf(fp,"cmp rax,rbx\n");
		fprintf(fp,"jl .rel%d\n",no_rop);
		fprintf(fp,"pop rax\n");
		fprintf(fp,"push 0000h\n");
		fprintf(fp,".rel%d:\n",no_rop);
		no_rop++;
		}
		else if(strcmp(ast->children->children->str,"TK_GT")==0){
			arith_(ast->children->sibling,fp);
			arith_(ast->children->sibling->sibling,fp);
			fprintf(fp, "pop rbx\n");
		fprintf(fp,"pop rax\n");
		fprintf(fp,"push 0ffffh\n");
		fprintf(fp,"cmp rax,rbx\n");
		fprintf(fp,"jg .rel%d\n",no_rop);
		fprintf(fp,"pop rax\n");
		fprintf(fp,"push 0000h\n");
		fprintf(fp,".rel%d:\n",no_rop);
		no_rop++;
			
		}
		else if(strcmp(ast->children->children->str,"TK_GE")==0){
	arith_(ast->children->sibling,fp);
			arith_(ast->children->sibling->sibling,fp);
		fprintf(fp, "pop rbx\n");
		fprintf(fp,"pop rax\n");
		fprintf(fp,"push 0ffffh\n");
		fprintf(fp,"cmp rax,rbx\n");
		fprintf(fp,"jge .rel%d\n",no_rop);
		fprintf(fp,"pop rax\n");
		fprintf(fp,"push 0000h\n");
		fprintf(fp,".rel%d:\n",no_rop);
		no_rop++;
			
		}
		else if(strcmp(ast->children->children->str,"TK_LE")==0){
			arith_(ast->children->sibling,fp);
			arith_(ast->children->sibling->sibling,fp);

			fprintf(fp, "pop rbx\n");
		fprintf(fp,"pop rax\n");
		fprintf(fp,"push 0ffffh\n");
		fprintf(fp,"cmp rax,rbx\n");
		fprintf(fp,"jle .rel%d\n",no_rop);
		fprintf(fp,"pop rax\n");
		fprintf(fp,"push 0000h\n");
		fprintf(fp,".rel%d:\n",no_rop);
		no_rop++;
			
		}
		else if(strcmp(ast->children->children->str,"TK_EQ")==0){
			arith_(ast->children->sibling,fp);
			arith_(ast->children->sibling->sibling,fp);

			fprintf(fp, "pop rbx\n");
		fprintf(fp,"pop rax\n");
		fprintf(fp,"push 0ffffh\n");
		fprintf(fp,"cmp rax,rbx\n");
		fprintf(fp,"je .rel%d\n",no_rop);
		fprintf(fp,"pop rax\n");
		fprintf(fp,"push 0000h\n");
		fprintf(fp,".rel%d:\n",no_rop);
		no_rop++;
			
		}
		else if(strcmp(ast->children->children->str,"TK_NE")==0){
			arith_(ast->children->sibling,fp);
			arith_(ast->children->sibling->sibling,fp);

			fprintf(fp, "pop rbx\n");
		fprintf(fp,"pop rax\n");
		fprintf(fp,"push 0ffffh\n");
		fprintf(fp,"cmp rax,rbx\n");
		fprintf(fp,"jne .rel%d\n",no_rop);
		fprintf(fp,"pop rax\n");
		fprintf(fp,"push 0000h\n");
		fprintf(fp,".rel%d:\n",no_rop);
		no_rop++;
			
		}
		

	}
	else if(strcmp(ast->children->str,"TK_NOT")==0){

		bool_(ast->children->sibling,fp);
		fprintf(fp, "pop rax\n");
		fprintf(fp, "not rax\n" );
		fprintf(fp, "push rax\n" );


	}
}

void stmt_(Node_ast stmt,FILE* fp){
	if(stmt->children!=NULL){
	if(strcmp(stmt->children->str,"ioStmt")==0){
			io_(stmt->children,fp);

		}
		else if(strcmp(stmt->children->str,"conditionalStmt")==0){
			conditional_(stmt->children,fp);

		}
		else if(strcmp(stmt->children->str,"assignmentStmt")==0){

			assign_(stmt->children,fp);
		}
		else if(strcmp(stmt->children->str,"iterativeStmt")==0){
			iter_(stmt->children,fp);
		}
}
}

void code_generation(Tree_ast ast,FILE* fp){


	print_num(fp);
	input_num(fp);
	

	
	
	fprintf(fp,"global _start\n");
	fprintf(fp,"_start:\n");
	Node_ast main_node = ast->root->children;

	Node_ast temp = main_node->children->sibling;
	fprintf(fp,"section .bss\n");
	//printf("%s",temp->str);
	declaration_(temp,fp);

	//Node_ast stmt = main_node->children->sibling->sibling->children;
	
	fprintf(fp,"section .text\n");
	Node_ast stmt = temp->sibling->children;
	while(stmt!=NULL){

		stmt_(stmt,fp);
		stmt=stmt->sibling;

	
	arith_(temp->sibling->children->children,fp);

}

fprintf(fp, "mov rax,1\n" );
fprintf(fp,"mov rbx,0\n");
fprintf(fp, "int 0x80");


	
}

