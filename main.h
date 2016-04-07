
#define PAYLOAD_LENGTH 200 /* Maximum payload size */
#define FILE_LENGTH 2000	/* Maximum payload size of file */

typedef struct { /* Packet buffer */
   int srcaddr;  /* Source address */
   int dstaddr;  /* Destination addres */
   int length;   /* Length of packet */
   char payload[PAYLOAD_LENGTH + 1];  /* Payload section */
   int valid;   /* Indicates if the contents is valid */ 
   int new;     /* Indicates if the contents has been downloaded */
} packetBuffer;

   
typedef struct {
	int srcaddr;  /* Source address */
	int dstaddr;  /* Destination addres */
	int length;   /* Length of packet */
	char payload[FILE_LENGTH + 1];  /* Payload section */
	int valid;   /* Indicates if the contents is valid */
	int new;     /* Indicates if the contents has been downloaded */
} fileBuffer;
