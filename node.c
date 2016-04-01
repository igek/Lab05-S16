#include "node.h"
#include <stdlib.h>

Node * createNode(){
	return (Node*)malloc(sizeof(Node));
}

void deleteNode(Node* nodeptr){
	free(nodeptr);
}
