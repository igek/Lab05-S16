#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <unistd.h>
#include <fcntl.h>

#include "main.h"
#include "utilities.h"
#include "link.h"
#include "man.h"
#include "host.h"

#define EMPTY_ADDR 0xffff /* Indicates the empty / broadcase address */

#define MAXBUFFER 1000
#define PIPEWRITE 1
#define PIPEREAD 0
#define TENMILLISEC 10000 /* 10 millisecond sleep */

void switchInitState(switchState * sstate, int physid);
void switchInitRcvPacketBuff(packetBuffer * packetbuff);
void switchInitSendPacketBuff(packetBuffer * packetbuff);

void switchTransmitPacket(switchState * sstate);

void switchTransmitPacket(switchState * sstate) {
	linkSend(&(sstate->linkout), &(sstate->sendPacketBuff));
	}

void switchMain(switchState * sstate) {
	packetBuffer tmpbuff;

	while(1) {
		linkReceive(&(sstate->linkin), &tmpbuff);
		
		if(tmpbuff.length > 0) {
			switchTransmitPacket(sstate);
		}
	}
}
