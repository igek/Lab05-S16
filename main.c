#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>
#include <fcntl.h>

#include "main.h"
#include "utilities.h"
#include "link.h"
#include "man.h"
#include "host.h"
#include "switch.h"
#include "net.h"
#include "netconfig.h"

#define TENMILLISEC 10000   /* 10 millisecond sleep */
#define EMPTY_ADDR  0xffff  /* Indicates the empty address */
                             /* It also indicates that the broadcast address */
#define MAXBUFFER 1000
#define PIPEWRITE 1 
#define PIPEREAD  0

#define DIRSIZE 20

int main()
{
	hostState hstate;           /* The host's state */
	switchState switchstate;		/* switch state */
	linkArrayType linkArray;
	manLinkArrayType manLinkArray;
	netconf netconfig;
	pid_t pid;					/* Process id */
	int physid;					/* Physical ID of host */
	int switchid;				/* Switch ID */
	int i, j, k, x, skip;
	char c;
	char charc[MAXNODE];
	int NUMHOSTS, NUMSWITCH, NUMLINKS, NUMNODE;
	
	/* Get the network configuration */	
	getConfig(&netconfig);
			
	printf("\nHOST INFORMATION:\n");
	for(k = 0; k < netconfig.numHosts; k++)
	{	printf("Host%d: Phys_ID: %d\t Dir: %s\t", 
		  k, netconfig.hosts_phys[k], netconfig.hosts_dir[k]);
		printf("Netadd: ");
		for(x = 0; x < 4; x++)
			printf("%c", netconfig.hosts_netadd[k][x]);
		printf("\n");
	} k = 0;		
	printf("\nSWITCH INFORMATION:\n");
	for(i = 0; i < netconfig.numSwitch; i++)
	{	printf("Switch %c:    %d: ", netconfig.switchid[i], netconfig.switchConnect[i]);
		for(j = 0; j < netconfig.switchConnect[i]; j++)
			printf("%c ", netconfig.switches[i][j]);
		printf("\n");
	} j = 0;
	NUMHOSTS = netconfig.numHosts;
	NUMSWITCH = netconfig.numSwitch;
	NUMLINKS = netconfig.numLinks;
	NUMNODE = netconfig.numNodes;
	printf("\nNETWORK INFORMATION:\n");
	printf("Number of Nodes: %d\n", NUMNODE);
	printf("Number of Hosts: %d\n", NUMHOSTS);
	printf("Number of Switch: %d\n", NUMSWITCH);
	printf("Number of Links: %d\n\n", NUMLINKS);
	
	
	/* Create nonblocking (pipes) between manager and hosts 
	 * assuming that hosts have physical IDs 0, 1, to 9 */
	manLinkArray.numlinks = NUMHOSTS;
	netCreateConnections(& manLinkArray);

	/* Create links between nodes but not setting their end nodes */
	linkArray.numlinks = NUMLINKS;
	netCreateLinks(& linkArray);

	/* Set the end nodes of the links */
	netSetNetworkTopology(&linkArray, &netconfig);
	
	printf("PIPE INFORMATION:\n");
		
	/* Create nodes and spawn their own processes, one process per node */ 
	for (physid = 0; physid < NUMHOSTS; physid++) 
	{
		/* Set and Initialize the host state */
		strcpy(hstate.maindir, netconfig.hosts_dir[physid]);
		hstate.maindirvalid = 1;
		
		for(j = 0; j < ADDSIZE; j++)
			charc[j] = netconfig.hosts_netadd[physid][j];
		hstate.netaddr =  ascii2Int(charc);

		
		/* fork and create a child process */
		pid = fork();
		if (pid == -1) {
			printf("Error:  the fork() failed\n");
			//return;
		}
		
		/* The child process -- a host node */
		else if (pid == 0) 
		{ 
			/* Initialize host's state */
			hostInit(&hstate, physid);

			/* Initialize the connection to the manager */ 
			hstate.manLink = manLinkArray.link[physid];

			/* Close all connections not connect to the host */
			/* Also close the manager's side of connections to host */
			netCloseConnections(& manLinkArray, physid);

			/* Initialize the host's incident communication links */
			k = netHostOutLink(&linkArray, physid);		/* Host's outgoing link */
			hstate.linkout = linkArray.link[k];
			printf("Host[%d] Transmit: %d\n",physid, linkArray.link[k].linkID);
				
			k = netHostInLink(&linkArray, physid);		/* Host's incoming link */
			hstate.linkin = linkArray.link[k];
			printf("Host[%d] Receive:  %d\n", physid, linkArray.link[k].linkID);
				
			/* Close all other links -- not connected to the host */
			netCloseHostOtherLinks(& linkArray, physid);
			
			/* Go to the main loop of the host node */
			hostMain(&hstate);
		}  
	}
	/* wait for 10ms */
	usleep(TENMILLISEC);

	/* Create nodes and spawn their own processes, one process per node */ 
	/* Assume that switches are labeled A, B, C ....*/
	
	for(i = 0; i < NUMSWITCH; i++) 
	{
		/* Set and Initialize the switch state */
		switchid = 65 + i;
		switchstate.switchid = switchid;
		switchstate.numConnects = netconfig.switchConnect[i];
		
		/* fork and create a child process */
		pid = fork();
		if (pid == -1) {
			printf("Error:  the fork() failed\n");
			//return;
		}
		
		/* The child process -- a host node */
		else if (pid == 0) 
		{ 
			/* Initialize swtiches state */
			switchInit(&switchstate);
			
			skip = 0;
			for(j = 0; j < switchstate.numConnects; j++)
			{
				/* Convert the switch ID to an int */
				c = netconfig.switches[i][j];
				if(c >= 48 && c <= 57)
					physid = c - 48;
				else physid = c;
				
				// Initialize the host's incident communication links
				k = netSwitchInLink(&linkArray, switchid, skip);		// Switch's outgoing link
				switchstate.linkin[j] = linkArray.link[k];
				printf("Switch[%d] Receive:  %d\n", i, linkArray.link[k].linkID);
			 
				k = netSwitchOutLink(&linkArray, switchid, skip);		// Switch's incoming link
				switchstate.linkout[j] = linkArray.link[k];
				printf("Switch[%d] Transmit: %d\n", i, linkArray.link[k].linkID);
			 
				// Initialize the routing table
				switchstate.physidConnect[j] = physid;
				switchstate.table[j][0] = physid;
				
				skip++;
			}
		
			/* Close all other links -- not connected to the switch */
			//netCloseSwitchOtherLinks(&linkArray, &switchstate, physid);
			
			/* Go to the main loop of the host node */
			SwitchMain(&switchstate);
		}  
	}
	
	/* wait for 20ms */
	usleep(TENMILLISEC);
	usleep(TENMILLISEC);

	/* Connected to the hosts and doesn't need the links between nodes */

	/* Close all links between nodes */
	netCloseLinks(&linkArray);

	/* Close the host's side of connections between a host and manager */
	netCloseManConnections(&manLinkArray);
	
	/* Go to main loop for the manager */
	manMain(& manLinkArray);
	
	/* Kill all processes, including children */
	kill(0, SIGKILL); 
}
