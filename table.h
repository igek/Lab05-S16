/*
 * table.h
*/

typedef struct{
	int valid; //1 or 0 valid or empty
	int destaddr; //destination address
	int outlink; //outgoing link
} FwdTable;
