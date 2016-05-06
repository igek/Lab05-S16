#ifndef MAIN_H
#define MAIN_H


#define PAYLOAD_LENGTH 200 /* Maximum payload size */
#define PAYLOAD_BUFFER 200 /* Maximum length of the processing buffer */

typedef struct {  /* Packet buffer */
   int srcaddr;      /* Source address */
   int dstaddr;      /* Destination addres */
   int length;    /* Length of packet */
   char payload[PAYLOAD_LENGTH + 1];  /* Payload section */
   int valid;     /* Indicates if the contents is valid */ 
   int new;    /* Indicates if the contents has been downloaded */
   int type;      /* Indicates type of packetbuffer */
} packetBuffer;

typedef struct {
   int type;
   int length;
   int root;
   int distance;
   int child;
} statePacket;

#endif
