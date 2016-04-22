/* Create nonblocking connections from the manager to all hosts */
void netCreateConnections(manLinkArrayType * manLinkArray); 

/* Create all nonblocking links */
void netCreateLinks(linkArrayType * linkArray);

/* 
 * Close all connections except the outgoing connection
 * from the host to manager and the incoming connection from
 * the manager to host
 */ 
void netCloseConnections(manLinkArrayType *  manLinkArray, int hostid);

/* 
 * Set up the end nodes of the links -- essentially creating 
 * network topology
 */
void netSetNetworkTopology(linkArrayType * linkArray, netconf *netconfig);

/* Find host's outgoing link and return its index from the link array */
int netHostOutLink(linkArrayType * linkArray, int hostid); 

/* Find host's incoming link and return its index from the link array */
int netHostInLink(linkArrayType * linkArray, int hostid); 

/* Find switch's incoming/outgoing link and return its index from link array */
int netSwitchInLink(linkArrayType * linkArray, int switchid, int skip); 
int netSwitchOutLink(linkArrayType * linkArray, int switchid, int skip); 

/* Close links not used by the host */
void netCloseHostOtherLinks(linkArrayType * linkArray, int hostid);

/* Close links not connected to the switch */
//void netCloseSwitchOtherLinks(linkArrayType * linkArray, switchState *switchstate, int switchid);

/* Close all links */
void netCloseLinks(linkArrayType * linkArray); 

/* Close the host's side of a connection between a host and manager */
void netCloseManConnections(manLinkArrayType * manLinkArray);

