/*
 * switch.h
 */

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
void SwitchInitQueue(Queue * switchQ);
void switchInitFwdTable(switchState * sstate);
void push(Queue *Q, packetBuffer data);
packetBuffer pop(Queue *Q);
