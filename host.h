/* 
 * host.h 
 */

#define NAME_LENGTH 100 
#define SEND_PB 10		//the number of packets that can be sent
#define RECV_PB 10		//the number of packets that can be recieved
#define PAYLOAD_TEMP 198	//the length of the payload

#define TRUE 1
#define FALSE 0


typedef struct { /* State of host */
   int   physid;              /* physical id */
   char  maindir[NAME_LENGTH]; /* main directory name */
   int   maindirvalid;        /* indicates if the main directory is empty */
   int   netaddr;             /* host's network address */
   int   nbraddr;             /* network address of neighbor */
   int   rcvflag;		/* indicates if recv buffer has data */
   packetBuffer sendPacketBuff[SEND_PB];  /* send packet buffer */
   packetBuffer rcvPacketBuff[RECV_PB];   /* recieve packet buffer */
   managerLink manLink;       /* Connection to the manager */
   LinkInfo linkin;           /* Incoming communication link */
   LinkInfo linkout;          /* Outgoing communication link */
} hostState;

void hostMain(hostState * hstate);

void hostInit(hostState * hstate, int physid);

//returns the index of the next valid packet buffer
int nextPacket(hostState * hstate);

//cleans the packetBuffer
void cleanPacket(packetBuffer * packetBuff);

//copies from -> to
void copyPacketBuff(packetBuffer * to, packetBuffer * from);

//inserts a packet in the the recvPacketBuff
void insertPacketRcv(hostState * hstate, packetBuffer * packetBuff);