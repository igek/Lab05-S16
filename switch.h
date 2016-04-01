/*
 * switch.h
 */

#define MAXENTRIES 100
#include "queue.h"
#include "table.h"
#include "link.h"


typedef struct{ /* state of switch */
	int switchid;			/* switch id */
	packetBuffer sendPacketBuff;	/* send packet buffer */
	packetBuffer rcvPacketBuff;	/* receive packet buffer */
	LinkInfo linkin[NUMHOSTS];		/* Incoming communication link */
	LinkInfo linkout[NUMHOSTS];		/* Outgoing communication link */
	FwdTable Table[MAXENTRIES]
} switchState;

void switchMain(switchState *sstate);

void switchInit(switchState *sstate, int physid);
