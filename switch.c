/*
 * This is the source code for the switch.
 *
 * A switch is a process spawned by the main function in main.c
 *
 */
 
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
#include "switch.h"

#define EMPTY_ADDR 0xffff /* The address for an empty or broadcase packet */

#define MAXBUFFER 1000
#define PIPEWRITE 1
#define PIPEREAD 0
#define TENMILLISEC 10000 /* 10 millisecond sleep */

//extern int hosts;
//extern int links;
//extern int switches;

/*
 * switchInit initializes the switch. It calls:
 * -switchInitQueue to initialize the switch packet queue
 * -switchInitFwdTable to initialize the switch forwarding table
 */
void switchInitFwdTable(switchState * sstate);

void switchInit(switchState *sstate, int physid) {
    sstate->physid = physid;
    sstate->netaddr = physid;
    sstate->root = physid;
    sstate->dist = 0;
    sstate->parent = 0;
    sstate->rcvPacketBuff.valid = 0;
    sstate->rcvPacketBuff.new = 0;
    
    sstate->sendSwitchInfo.srcaddr = physid;
    sstate->sendSwitchInfo.length = 4;
    sstate->rcvSwitchInfo.valid = 0;
    sstate->rcvSwitchInfo.new = 0;
    
    switchInitQueue(&sstate->packetQ);
    switchInitFwdTable(sstate);

    usleep(TENMILLISEC);
}

void switchInitFwdTable(switchState * sstate) {
    int i;
    
    for(i = 0; i < hosts; i++) {
        sstate->FwdTable[i][0] = 0;
        sstate->FwdTable[i][1] = i;
        sstate->FwdTable[i][2] = 0;
    }
}

void switchMain(switchState *sstate, linkArrayType *linkArray, char *fileName) {
    char packetData[MAXBUFFER];
    int i, j, destaddr, srcaddr;
    int n, parent;
    int routeTable[NUMLINKS];
    packetBuffer tmpbuff, out;
    LinkInfo testLink = sstate->linkin;
    
    for(i = 0; i < links; i++) {
        routeTable[i] = -1;
    }
    
    // Fill out Routing Table
    i = 0;
    char *end;
    int first, second, third;
    FILE *file = fopen(fileName, "r");
    
    if(file != NULL) {
        char line[10];
        
        while(fgets(line, sizeof(line), file) != NULL) {
            if(line[0] != '-' && i > 0) {
                first = strtol(line, &end, 10);
                second = strtol(end, &end, 10);
                third = strtol(end, &end, 10);
                
                if(first == sstate->physid) {
                    routeTable[third] = 1;
                }
                if(second == sstate->physid) {
                    routeTable[third] = 0;
                }
            }
            
            i++;
        }
        
        fclose(file);
    } else {
        perror("Data File not read.");
    }
    
    
    // Variables for main operation
    char root[2], dist[1], child[1];
    
    // Main Operation of the Network Switch
    while(1) {
        for(i = 0; i < links; i++) {
            if((linkArray->link[i].uniPipeInfo.physIdSrc < switches) && (linkArray->link[i].uniPipeInfo.physIdDst < switches)) {
                sstate->sendSwitchInfo.dstaddr = linkArray->link[i].uniPipeInfo.physIdDst;
                sprintf(root, "%d", sstate->root);
                if(root[1] == '\0') {
                    root[1] = root[0];
                    root[0] = '0';
                    root[2] = '\0';
                }
                
                dist[0] = sstate->dist;
                
                packetData[0] = root[0];
                packetData[1] = root[1];
                packetData[2] = ' ';
                packetData[3] = dist[0];
                packetData[4] = ' ';
                packetData[5] = '1';
                packetData[6] = '\0';
                
                for(n = 0; n < 6; n++) {
                sstate->sendSwitchInfo.payload[n] = packetData[n];
                }
                
                linkSend(&(sstate->linkout), &(sstate->sendSwitchInfo));
                
                for(n = 0; n < 200; n++) {
                    packetData[n] = '\0';
                    sstate->sendSwitchInfo.segmentPayload[n] = '\0';
                }
                
                if(linkReceive(&linkArray->link[i], &tmpbuff) > 0 ) {
                    if(sstate->rcvSwitchInfo.srcaddr >= 0) {
                        printf("%d : Packet received from %d.\n", sstate->physid, sstate->rcvSwitchInfo.srcaddr);
                        sstate->rcvSwitchInfo = tmpbuff;
                        findWord(root, sstate->rcvSwitchInfo.segmentPayload, 1);
                        printf("root = %d.\n", atoi(root));
                        findWord(dist, sstate->rcvSwitchInfo.segmentPayload, 2);
                        printf("distance = %d.\n", dist[0]);
                        findWord(child, sstate->rcvSwitchInfo.segmentPayload, 3);
                        printf("child = %s.\n", child);
                        
                        if(sstate->root > atoi(root)) {
                            sstate->root = atoi(root);
                            sstate->parent = sstate->rcvSwitchInfo.srcaddr;
                            sstate->dist = atoi(dist) + 1;
                        } else if (atoi(dist) + 1 < sstate->dist) {
                            sstate->parent = sstate->rcvSwitchInfo.srcaddr;
                            sstate->dist = atoi(dist) + 1;
                        }
                        
                    }
                }
            }
        if(routeTable[i] == 0) {
            if(linkReceive(&linkArray->link[i], &tmpbuff) > 0) {
                tmpbuff.rcvlink = linkArray->link[i].uniPipeInfo.physIdSrc;
                if(tmpbuff.srcaddr >= switches) {
                    push(&sstate->packetQ, tmpbuff);
                    
                    if(tmpbuff.srcaddr != 1000) {
                        srcaddr = linkArray->link[i].uniPipeInfo.physIdSrc;
                        
                        for(j = 0; j < links; j++) {
                            if(linkArray->link[j].uniPipeInfo.physIdDst == srcaddr && routeTable[j] == 1) {
                                sstate->FwdTable[tmpbuff.srcaddr][2] = j;
                                printf("%d : Switch will forward packets with destination %d through %d.\n", sstate->physid, tmpbuff.srcaddr, j);
                            }
                        }
                        sstate->FwdTable[tmpbuff.srcaddr][0] = 1;
                    }
                }
            }
        }
    }
    
        if(sstate->packetQ.size > 0) {
            out = pop(&sstate->packetQ);
            
            for(i = 0; i < links; i++) {
                if(sstate->FwdTable[out.dstaddr][0] == 1) {
                    printf("%d : sending packet to outgoing link %d.\n", sstate->physid, sstate->FwdTable[out.dstaddr][2]);
                    linkSend(&linkArray->link[sstate->FwdTable[out.dstaddr][2]], &out);
                    i = links;
                } else if (routeTable[i] == 1 && out.rcvlink != linkArray->link[i].uniPipeInfo.physIdDst) {
                    printf("%d : sending packet to outgoing link %d.\n", sstate->physid, i);
                    linkSend(&linkArray->link[i], &out);
                }
            }
        }
    }
    
    usleep(TENMILLISEC);
    i = 0;
}
