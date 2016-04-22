#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "netconfig.h"
#include "utilities.h"


#define MAXNODE 30
#define MAXCONT 10
#define DIRSIZE 20


void getConfig(netconf *netconfig) 
{
	char c;
	char filename[20];
	FILE * file_ptr;
	char * file_data;
	char *dir;
	char temp[MAXNODE];
	int j = 0, i = 0, k = 0, x = 0;
	
	dir = (char*)malloc(sizeof(char)*MAXNODE*DIRSIZE);
	
	/* Read in the Configuration File */
	printf("Enter the topology config filename to read: ");
	scanf("%s", filename);
	file_ptr = fopen(filename, "r");
	while(file_ptr == NULL)
	{	printf("File was not found\n\n");
		printf("Enter the topology config filename to read: ");
		scanf("%s", filename);
		file_ptr = fopen(filename, "r");
	}
		
	// Kill whitespace
	c = fgetc(file_ptr);
	while(c == ' ' || c == '\t' || c == '\n')	
		c = fgetc(file_ptr);

	
	/* Get Host Info */
	while(c == 'H' && c != EOF)
	{
		c = fgetc(file_ptr);
		while(c == ' ' || c == '\t' || c == '\n')	
			c = fgetc(file_ptr);
			
		// sets the physical ID
		while(c != ' ')
		{	temp[j] = c;
			c = fgetc(file_ptr);
			j++;
		}j = 0;
		netconfig->hosts_phys[i]= ascii2Int(temp); 
		
		// clean up
		c = fgetc(file_ptr);
		while(c == ' ' || c == '\t' || c == '\n')	
			c = fgetc(file_ptr);
		for(k = 0; k < MAXNODE; k++)
			temp[k] = 0;
		
	
		// sets the main directory
		while(c != ' ')
		{	netconfig->hosts_dir[i][j] = c;
			c = fgetc(file_ptr);
			j++;
		} j = 0;
		
		// clean up
		c = fgetc(file_ptr);
		while(c == ' ' || c == '\t' || c == '\n')	
			c = fgetc(file_ptr);
		
		
		// sets network address
		while(c != '\n' && c != ' ' && c != EOF && j < 4)
		{	netconfig->hosts_netadd[i][j] = c;
			c = fgetc(file_ptr);
			j++;
		}
		
		// clean up
		while(c == ' ' || c == '\t' || c == '\n')	
			c = fgetc(file_ptr);
		i++;
		j = 0, k = 0;
	}
	netconfig->numHosts = i;
	i = 0, j = 0, k = 0;
			
				
	/* Get Swtich Info */
	while(c == 'S' && c != EOF)
	{
		c = fgetc(file_ptr);
		while(c == ' ' || c == '\t' || c == '\n')	
			c = fgetc(file_ptr);
		
		// sets the switch ID
		netconfig->switchid[i]= c; 
		
		c = fgetc(file_ptr);
		while(c == ' ' || c == '\t' || c == '\n')	
			c = fgetc(file_ptr);
		
		// set the switch connected hosts 
		while(c != '\n' &&  c != EOF)
		{	
			netconfig->switches[i][j] = c; 
			j++;
			c = fgetc(file_ptr);
			while(c == ' ')	
				c = fgetc(file_ptr);
		}
		netconfig->switchConnect[i] = j;
		j = 0;
		i++;
		while(c == ' ' || c == '\t' || c == '\n')	
			c = fgetc(file_ptr);
	}
	
	netconfig->numSwitch = i;
	netconfig->numNodes = netconfig->numSwitch + netconfig->numHosts;
	netconfig->numLinks = 2 * (netconfig->numNodes - 1);
}
