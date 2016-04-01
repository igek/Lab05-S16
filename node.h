/*
 * node.h
*/

#include "main.h"

typedef packetBuffer Entry;

typedef struct Node{
	Entry data;
	struct Node* next;
} Node;

Node* createNode();
void deleteNode(Node* nodeptr);
