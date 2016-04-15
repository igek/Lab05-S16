/*
 * switch.h
 */

extern int hosts;
extern int links;
extern int switches;

#define MAX_HOSTS 1000
typedef struct switchPacket {
    packetBuffer node;
    struct switchPacket * next;
    struct switchPacket * prev;
} switchNode;

typedef struct {
    switchNode * head;
    switchNode * tail;
    int size;
} Queue;

typedef struct {                     /* State of switch */
    int physid;                      /* Physical ID */
    int netaddr;                    /* Switch Network Address */
    int root;
    int dist;
    int parent;
    
    packetBuffer sendPacketBuff;    /* Send Packet Buffer */
    packetBuffer rcvPacketBuff;     /* Recieved Packet Buffer */
    
    packetBuffer sendSwitchInfo;
    packetBuffer rcvSwitchInfo;
    
    LinkInfo linkin;                /* Incoming Communication Links */
    LinkInfo linkout;               /* Outgoing Communication Links */
    
    Queue packetQ;                  /* Packet Queue */
    int FwdTable[MAX_HOSTS][3];      /* Forwarding Table */
    
} switchState;

void switchMain(switchState * sstate, linkArrayType *linkArray, char *fileName);
void switchInit(switchState * sstate, int physid);
void push(Queue *Q, packetBuffer data);
packetBuffer pop(Queue *Q);

/* Functions for Queue */
void switchInitQueue(Queue * switchQ) {
    switchQ->size = 0;
    switchQ->head = (switchNode*)malloc(sizeof(switchNode));
    switchQ->tail = (switchNode*)malloc(sizeof(switchNode));
    
    switchQ->head->next = switchQ->tail;
    switchQ->head->prev = NULL;
    
    switchQ->tail->prev = switchQ->head;
    switchQ->tail->next = NULL;
}

void push(Queue *Q, packetBuffer data) {
    switchNode * addNode = (switchNode*)malloc(sizeof(switchNode));
    
    /* Define the new node attributes */
    addNode->node = data;
    addNode->next = Q->head->next;
    addNode->prev = Q->head;
    
    /* Change pointers */
    Q->head->next->prev = addNode;
    Q->head->next = addNode;
    Q->size++;
}

packetBuffer pop(Queue *Q) {
    switchNode * current;
    switchNode * previous;
    packetBuffer data;
    
    if(Q->size > 0) {
        current = Q->tail->prev;
        previous = current->prev;
        
        Q->tail->prev = previous;
        previous->next = Q->tail;
        data = current->node;
        
        free(current);
        Q->size--;
        
        return data;
    }
}
