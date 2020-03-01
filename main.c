#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "packing.h"


int main(int argc, char** argv)
{
    queue *head = buildlist(argv[1]);

    head = reversequeue(head);

    queue *temp = head;
    while(temp != NULL)
    {
        printf("%c(%d,%d)\n",temp->data->value,temp->data->xlength,temp->data->ylength);
        temp = temp->next;
    }

    node *treehead = maketree(head);

    output1(treehead, argv[2]);

    output2(treehead, argv[3]);

    modifytree(treehead);

    output3(treehead, argv[4]);

    freetree(treehead);

	return EXIT_SUCCESS;
}
















