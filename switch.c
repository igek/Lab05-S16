/*
 * This is the source code for the switch.
 *
 * A switch is a process spawned by the main function in main.c
 *
 */
 
#include "switch.h"

/* Main Switch Function */
void SwitchMain(switchState * sw)
{
	int check_host, host, j;
	int packet_length;
	int head = 0;
	int tail = 0;
	packetBuffer pbuff;
	packetBuffer queue[1000];
	packetBuffer q_buffer;
	
	/* Switch Node While Loop */
	while(1)
	{		
		/* Receive packets */
		for(host = 0; host < sw->numConnects; host++)
		{
			linkReceive(&(sw->linkin[host]), &pbuff);
			
			if(pbuff.valid)
			{	
				sw->RecvID = sw->linkin[host].uniPipeInfo.physIdSrc;
				update_table(sw, &pbuff, host);
				insertFIFO(&queue, &pbuff, sizeof(packetBuffer), &head, &tail, 1000);
				break;
			}
		}
		
		/* Transmit first packet of queue */
		if(head != tail)	
		{
			removeFIFO(&queue, &q_buffer, sizeof(packetBuffer), &head, &tail, 1000);
			if(q_buffer.dstaddr < 1000) 
				transmit(sw, &q_buffer, BROADCAST);
			else 
			{	for(host = 0; host < sw->numConnects; host++)
				{	
					if(sw->table[host][1] == q_buffer.dstaddr)
					{	transmit(sw, &q_buffer, host);
						break;
					}
				}
				if(host == sw->numConnects) transmit(sw, &q_buffer, BROADCAST);
			}
		}
		/* The switch goes to sleep for 20 ms */
		usleep(TENMILLISEC);
		usleep(TENMILLISEC);
	}
}


/* Update the routing table */
void update_table(switchState * sw, packetBuffer *pbuff, int host)
{
	int i;
	for(i = 0; i < sw->numConnects; i++)
	{
		if(sw->table[i][0] == sw->physidConnect[host])
		{	sw->table[i][1] = pbuff->srcaddr;
			break;
		}
	}
	sw->tableIndex++;
}


/* Insert a packetBuffer into the FIFO */
void insertFIFO(void *buffer, void *item, unsigned itemSize, int * head, int * tail, int bufferSize)
{
	memmove(buffer + ((*tail)*itemSize), item, itemSize);
	*tail = ((*tail)+1) % bufferSize;
	if(*tail == *head) *head = ((*head)++ % bufferSize);
}


/* Remove a packetBuffer into the FIFO */
void removeFIFO(void *buffer, void *item, unsigned itemSize, int * head, int * tail, int bufferSize)
{
	if(*head == *tail) return;
	
	memmove(item, buffer+((*head)*itemSize), itemSize);
	*head = ((*head) + 1 % bufferSize);
}



/* Transmit a packetBuffer */
void transmit(switchState * sw, packetBuffer * q_buffer, int sendval)
{	
	LinkInfo SendLink;
	int i, j;
	
	if(sendval != BROADCAST)
	{	SendLink = sw->linkout[sendval];
		linkSend(&SendLink, q_buffer);
	}
	else
	{	for(i = 0; i < sw->numConnects; i++)
		{	if(sw->physidConnect[i] != sw->RecvID)
			{	SendLink = sw->linkout[i];
			 	linkSend(&SendLink, q_buffer);
			}
		}
	}
}


/* Initializes the host */
void switchInit(switchState * switchstate)
{	
	/* Initialize the receive and send packet buffers */
	hostInitRcvPacketBuff(&(switchstate->rcvPacketBuff));  
	hostInitSendPacketBuff(&(switchstate->rcvPacketBuff)); 
	
	/* Initialize the network packet */
	switchstate->state.srcadd = switchstate->physid;
	switchstate->state.root = switchstate->physid;
	switchstate->state.distance = 2147483647;		/* Infinity but maximum value for an int will have to suffice */
}

/* Initialize send packet buffer */
void switchInitSendPacketBuff(packetBuffer * packetbuff)
{
	packetbuff->valid = 0;
	packetbuff->new = 0;
}

/* Initialize receive packet buffer  */ 
void switchInitRcvPacketBuff(packetBuffer * packetbuff)
{
	packetbuff->valid = 0;
	packetbuff->new = 0;
}

