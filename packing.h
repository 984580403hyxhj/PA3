#ifndef __PACKING_H__
#define __PACKING_H__

typedef struct _Node{
	int value;
	int xlength;
	int ylength;
	int xcor;
	int ycor;
	bool isleaf;
	struct _Node* left;
	struct _Node* right;
}node;

typedef struct _queue{
	node *data;
	struct _queue* next;

}queue;

queue *buildlist(char *filename);

node *maketree(queue *head);

void modifytree(node *head);

void output1(node *head, char *filename);
void output2(node *head, char *filename);
void output3(node *head, char *filename);

void freetree(node *head);

#endif


