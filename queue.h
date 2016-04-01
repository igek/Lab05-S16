/*
 * queue.h
*/

typedef struct{
	Node* head;
	Node* tail;
} Queue;

void initQueue(Queue* q);

void add(Queue* q, Entry e);
Entry delete(Queue* q);

int empty(Queue* q);
