#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "packing.h"

typedef struct _list{
	struct _queue* head;
	struct _queue* tail;

}list;

queue *buildqueue(int xvalue, int yvalue, int value, bool isleaf)
{
	queue *p = malloc(sizeof(queue));
	p->data = malloc(sizeof(node));
	p->data->left = NULL;
	p->data->right = NULL;
	p->data->xlength = xvalue;
	p->data->ylength = yvalue;
	p->data->xcor = 0;
	p->data->ycor = 0;
	p->data->value = value;
	p->data->isleaf = isleaf;
	p->next = NULL;

	return p;
}

queue *insertqueue(queue *head, int xvalue, int yvalue, int value, bool isleaf)
{
	queue *p = buildqueue(xvalue, yvalue, value, isleaf);
	p->next = head;

	return p;
}

queue *buildlist(char *filename)/////use fgetc less than 10
{
	FILE *fp = fopen(filename,"r");
	if(fp == NULL)
	{
		return NULL;
	}
	int value = 0;
	//int tempvalue = 0;
	int xvalue = 0;
	int yvalue = 0;
	//node *tree;
	//node *top;
	//queue *head = NULL;

	list control;
	control.head = NULL;
	control.tail = NULL;

	while((value=fgetc(fp)) != EOF)
	{
		//value = fgetc(fp);
		if(isdigit(value) != 0)
		{
			fseek(fp, -1, SEEK_CUR);
			fscanf(fp, "%d(%d,%d)\n", &value, &xvalue, &yvalue);
			if(control.tail == NULL){//initialize
				control.head = buildqueue(xvalue,yvalue,value,1);
				control.tail = control.head;
			}else{
				control.tail->next = buildqueue(xvalue,yvalue,value,1);
				control.tail = control.tail->next;
			}
			
		}else if(isalpha(value) != 0){
			control.tail->next = buildqueue(0,0,value,0);//not a leaf
			control.tail = control.tail->next;

		}
	}
	fclose(fp);
	return control.head;
}



void merge(queue* dptr, queue* temp1, queue* temp2, queue* temp3)
{
	dptr->next = temp3;
	temp3->data->left = temp1->data;
	temp3->data->right = temp2->data;

	if(temp3->data->value == 'V')
	{
		temp3->data->xlength = temp1->data->xlength + temp2->data->xlength;
		temp3->data->ylength = temp1->data->ylength>temp2->data->ylength?temp1->data->ylength:temp2->data->ylength;
	}else{
		temp3->data->ylength = temp1->data->ylength + temp2->data->ylength;
		temp3->data->xlength = temp1->data->xlength>temp2->data->xlength?temp1->data->xlength:temp2->data->xlength;
	}
	free(temp1);
	free(temp2);
}

node *maketree(queue *head)
{
	queue *temp1 = head;
	queue *temp2 = head->next;
	queue *temp3 = temp2->next;

	queue dummy;
	dummy.data = NULL;
	dummy.next = head;
	queue *dptr = &dummy;

	while(dummy.next->next != NULL)
	{
		if((temp3->data->isleaf == 0)&&(temp3->data->left == NULL))//third one is not leaf
		{
			merge(dptr, temp1, temp2, temp3);
			temp1 = dummy.next;
			temp2 = temp1->next;
			if(temp2 == NULL) break;
			temp3 = temp2->next;
			dptr = &dummy;
		}else{
			temp1 = temp1->next;
			temp2 = temp2->next;
			temp3 = temp3->next;
			dptr = dptr->next;
		}
	}
	node *temp = dummy.next->data;
	free(dummy.next);
	//free(head);

	return temp;
}

////////////////above build tree, all worked//////////////

////////////////first output file below//////////////
typedef struct _stack{
	node *data;
	struct _stack *next;
}Stack;

void push(Stack **stack, node *address)
{
	Stack *head = malloc(sizeof(Stack));
	head->data = address;
	head->next = *stack;
	*stack = head;
}

node *top(Stack *stack)
{
	return stack->data;
}

void pop(Stack **stack)
{
	Stack *temp = *stack;
	*stack = (*stack)->next;
	free(temp);
}

bool isempty(Stack *stack)
{
	return !stack;
}

void preorder(node *head, FILE* fp)
{
	
	Stack *stack = NULL;
	node *temp = head;
	do{
		while(temp != NULL){
			if(temp->isleaf == 1){
				fprintf(fp, "%d(%d,%d)\n",temp->value, temp->xlength, temp->ylength);
			}else{
				fprintf(fp, "%c\n", temp->value);
			}
			push(&stack, temp);
			temp = temp->left;
		}
		while(!isempty(stack) && temp == top(stack)->right){
			temp = top(stack);
			pop(&stack);
		}
		if(!isempty(stack)){
			temp = top(stack)->right;
		}
	}while(!isempty(stack));
}


void output1(node *head, char *filename)
{
	FILE *fp = fopen(filename,"w+");
	preorder(head, fp);
	fclose(fp);

}

///////////above all worked//////////
///////////second output below////////
void inorder(node *head, FILE* fp)
{
	
	Stack *stack = NULL;
	node *temp = head;
	do{
		while(temp != NULL){
			push(&stack, temp);
			temp = temp->left;
		}
		while(!isempty(stack) && temp == top(stack)->right){
			temp = top(stack);
			if(temp->isleaf == 0) {
				fprintf(fp, "%c(%d,%d)\n",temp->value, temp->xlength, temp->ylength);
			}else{
				fprintf(fp, "%d(%d,%d)\n",temp->value, temp->xlength, temp->ylength);
			}
			pop(&stack);
		}
		if(!isempty(stack)){
			temp = top(stack)->right;
		}
	}while(!isempty(stack));
}


void output2(node *head, char *filename)
{
	FILE *fp = fopen(filename,"w+");
	inorder(head,fp);
	fclose(fp);
}

/////////above all worked///////////
/////////third output below/////////

void modifytree(node *head)
{
	

	Stack *stack = NULL;
	node *temp = head;
	do{
		while(temp != NULL){
			push(&stack, temp);
			if(!(temp->isleaf)){
				if(temp->value == 'V')
				{
					temp->left->xcor = temp->xcor;
					temp->left->ycor = temp->ycor;
					temp->right->xcor = temp->xcor + temp->left->xlength;
					temp->right->ycor = temp->ycor;
				}
				if(temp->value == 'H')
				{
					temp->left->ycor = temp->ycor + temp->right->ylength;
					temp->left->xcor = temp->xcor;
					temp->right->ycor = temp->ycor;
					temp->right->xcor = temp->xcor;
				}
			}
			temp = temp->left;
		}
		while(!isempty(stack) && temp == top(stack)->right){
			temp = top(stack);
			pop(&stack);
		}
		if(!isempty(stack)){
			temp = top(stack)->right;
		}
	}while(!isempty(stack));
}

void inorder2(node *head, FILE *fp)
{
	
	Stack *stack = NULL;
	node *temp = head;
	do{
		while(temp != NULL){
			push(&stack, temp);
			temp = temp->left;
		}
		while(!isempty(stack) && temp == top(stack)->right){
			temp = top(stack);
			if(temp->isleaf == 1){
				fprintf(fp, "%d((%d,%d)(%d,%d))\n",temp->value, temp->xlength, temp->ylength, temp->xcor, temp->ycor );
			}
			pop(&stack);
		}
		if(!isempty(stack)){
			temp = top(stack)->right;
		}
	}while(!isempty(stack));
}

void output3(node *head, char *filename)
{
	FILE *fp = fopen(filename,"w+");
	inorder2(head, fp);
	fclose(fp);
}

/////above all work/////////

//////below free the tree/////
void freetree(node *head)
{
	
	Stack *stack = NULL;
	node *temp = head;
	do{
		while(temp != NULL){
			push(&stack, temp);
			temp = temp->left;
		}
		while(!isempty(stack) && temp == top(stack)->right){
			temp = top(stack);
			pop(&stack);
			free(temp);
		}
		if(!isempty(stack)){
			temp = top(stack)->right;
		}
	}while(!isempty(stack));
}





































