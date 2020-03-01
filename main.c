#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "packing.h"


#define COUNT 10

void print2DUtil(node *root, int space) 
{ 
    // Base case 
    if (root == NULL) 
        return; 
  
    // Increase distance between levels 
    space += COUNT; 
  
    // Process right child first 
    print2DUtil(root->right, space); 
  
    // Print current node after space 
    // count 
    printf("\n"); 
    for (int i = COUNT; i < space; i++) 
        printf(" "); 
    printf("%c, (%d, %d)(%d, %d)\n", root->value, root->xlength, root->ylength, root->xcor, root->ycor); 
  
    // Process left child 
    print2DUtil(root->left, space); 
} 
  
// Wrapper over print2DUtil() 
void print2D(node *root) 
{ 
   // Pass initial space count as 0 
   print2DUtil(root, 0); 
} 







int main(int argc, char** argv)
{
    queue *head = buildlist(argv[1]);

    head = reversequeue(head);
    queue *temp = head;

    while(temp != NULL)
    {
        printf("%c, (%d, %d)\n",temp->data->value, temp->data->xlength, temp->data->ylength);
        temp = temp->next;
    }

    node *treehead = maketree(head);

    output1(treehead, argv[2]);

    output2(treehead, argv[3]);

    modifytree(treehead);

    print2D(treehead);

    output3(treehead, argv[4]);

    freetree(treehead);

	return EXIT_SUCCESS;
}
















