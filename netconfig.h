#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities.h"

#ifndef NETCONFIG
#define NETCONFIG

#define MAXNODE 30
#define MAXCONT 10
#define DIRSIZE 20
#define ADDSIZE 4

// Host Network Configuration
typedef struct{
	int physid;
	char hosts_dir[DIRSIZE];
} host_conf;


// Switch Network Configuration
typedef struct{
	int numConnect;
	int switches[MAXNODE];
} switch_conf;


// Global structure Network Configuration
typedef struct{
	int numNodes;
	int numHosts;
	int numSwitch;
	int numLinks;
	int hosts_phys[MAXNODE];
	char hosts_dir[MAXNODE][DIRSIZE];
	char hosts_netadd[MAXNODE][ADDSIZE];
	char switchid[MAXNODE];
	char switches[MAXNODE][MAXCONT];
	int switchConnect[MAXCONT];
} netconf;



void getConfig(netconf *netconfig);


#endif