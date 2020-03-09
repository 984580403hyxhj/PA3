#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "packing.h"


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
	queue *head = NULL;

	while(!feof(fp))
	{
		value = fgetc(fp);
		if(isdigit(value) != 0)
		{
			fseek(fp, -1, SEEK_CUR);
			fscanf(fp, "%d(%d,%d)\n", &value, &xvalue, &yvalue);
			//printf("%d(%d,%d)\n",value,xvalue,yvalue);
			//value = tempvalue;
			head = insertqueue(head, xvalue, yvalue, value, 1); // 1, is a leaf
		}else if(isalpha(value) != 0){

			head = insertqueue(head, 0, 0, value, 0);//0 not a leaf
			//printf("%c\n", value);
		}
	}
	fclose(fp);
	return head;
}

/*queue *buildlist(char *filename)
{
	FILE *fp = fopen(filename,"r");
	int value = 0;
	//char tempvalue = 0;
	int xvalue = 0;
	int yvalue = 0;
	//node *tree;
	//node *top;
	queue *head = NULL;

	while(!feof(fp))
	{
		fscanf(fp, "%d(%d,%d)\n",&value,&xvalue,&yvalue);
		if(isdigit(value) != 0)
		{
			head = insertqueue(head, xvalue, yvalue, value);
		}else if(isalpha(value) != 0){

			head = insertqueue(head, 0, 0, value);
		}
	}
	fclose(fp);
	return head;
}*/

queue *reversequeue(queue *head)
{
	queue *prev = NULL;
	queue *cur = head;
	queue *next = head->next;
	while(cur != NULL)
	{
		cur->next = prev;
		prev = cur;
		cur = next;
		if(cur == NULL) break;
		next = next->next;
	}
	return prev;
}

////////above build queue, all work////////////

queue *merge(queue *head, queue *temp1, queue *temp2, queue *temp3)
{
	temp3->data->left = temp1->data;
	temp3->data->right = temp2->data;

	if(temp3->data->value == 'V')
	{
		temp3->data->xlength = temp1->data->xlength + temp2->data->xlength;
		if(temp1->data->ylength >= temp2->data->ylength)
		{
			temp3->data->ylength = temp1->data->ylength;
		}else{
			temp3->data->ylength = temp2->data->ylength;
		}
	}else{
		temp3->data->ylength = temp1->data->ylength + temp2->data->ylength;
		if(temp1->data->xlength >= temp2->data->xlength)
		{
			temp3->data->xlength = temp1->data->xlength;
		}else{
			temp3->data->xlength = temp2->data->xlength;
		}
	}
	if(head == temp1)
	{
		head = temp3;
		free(temp1);
		free(temp2);
	}else{
		queue *p = head;
		while(p->next != temp1)
		{
			p = p->next;
		}
		p->next = temp3;
		free(temp1);
		free(temp2);
	}



	return head;

}

node *maketree(queue *head)
{
	queue *temp1 = head;
	queue *temp2 = head->next;
	queue *temp3 = temp2->next;

	while(head->next != NULL)
	{
		if((temp3->data->isleaf == 0)&&(temp3->data->left == NULL))//third one is not leaf
		{
			head = merge(head, temp1, temp2, temp3);
			if(head->next == NULL) break;
			temp1 = head;
			temp2 = head->next;
			temp3 = temp2->next;
		}else{
			temp1 = temp1->next;
			temp2 = temp2->next;
			temp3 = temp3->next;
		}
	}
	node *temp = head->data;
	free(head);

	return temp;
}

////////////////above build tree, all worked//////////////

////////////////first output file below//////////////
void preorder(node *head, FILE* fp)
{
	if(head == NULL) return;

	if(head->isleaf == 1)
	{
		fprintf(fp, "%d(%d,%d)\n",head->value, head->xlength, head->ylength);
	}else{
		fprintf(fp, "%c\n", head->value);
	}
	preorder(head->left,fp);
	preorder(head->right,fp);
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
	if(head == NULL) return;

	inorder(head->left,fp);
	inorder(head->right,fp);

	if(head->isleaf == 0) {
		fprintf(fp, "%c(%d,%d)\n",head->value, head->xlength, head->ylength);
	}else{
		fprintf(fp, "%d(%d,%d)\n",head->value, head->xlength, head->ylength);
	}
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
	if(head->isleaf) return;

	if(head->value == 'V')
	{
		head->left->xcor = head->xcor;
		head->left->ycor = head->ycor;
		head->right->xcor = head->xcor + head->left->xlength;
		head->right->ycor = head->ycor;
	}
	if(head->value == 'H')
	{
		head->left->ycor = head->ycor + head->right->ylength;
		head->left->xcor = head->xcor;
		head->right->ycor = head->ycor;
		head->right->xcor = head->xcor;
	}

	modifytree(head->left);
	modifytree(head->right);
}

void inorder2(node *head, FILE *fp)
{
	if(head == NULL) return;

	if(head->isleaf == 1)
	{
		//printf("111\n");
		//printf("%d((%d,%d)(%d,%d))\n",head->value, head->xlength, head->ylength, head->xcor, head->ycor  );
		fprintf(fp, "%d((%d,%d)(%d,%d))\n",head->value, head->xlength, head->ylength, head->xcor, head->ycor );
	}

	inorder2(head->left,fp);
	inorder2(head->right,fp);
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
	if(!(head->left && head->right))
	{
		free(head);
		return;
	}

	freetree(head->left);
	freetree(head->right);

	free(head);
}





































