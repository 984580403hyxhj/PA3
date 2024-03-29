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
    printf("%d\n", root->value); 
  
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
    if(argc != 5)
    {
        return EXIT_FAILURE;
    }

    queue *head = buildlist(argv[1]);
    if(head == NULL)
    {
        return EXIT_FAILURE;
    }

    node *treehead = maketree(head);

    //print2D(treehead);

    output1(treehead, argv[2]);

    output2(treehead, argv[3]);

    modifytree(treehead);

    output3(treehead, argv[4]);

    freetree(treehead);

	return EXIT_SUCCESS;
}
















