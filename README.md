# Lab05-S16
UH Manoa EE367L  Lab 05 Spring 2016

2.1 Links
The links can transport a stream of bytes and will be implemented by pipes (e.g., pipe or pipe2). It can
transport a packet which is essentially a file with the following components:

Header: Control information to process the packet. It is comprised of

Destination address (2 bytes): This is the address of the destination host of the packet.

Source address (2 bytes): This is the address of the host of where the packet originated.

Length (1 byte), which is an unsigned integer: This is the total length of the payload in bytes.

Payload: The data that is carried by the packet (size ranges from 1 and 200 bytes)
The addresses are called network addresses since they are used to get the packet through the network. Each
host will have a distinct network address. The network address is 16 bits.
One particular address 1000 is reserved for broadcast, which means this packet is meant for all hosts. No
node should have this network address.

2.2 Host
Each host will have its own ID, which we refer to as its physical ID. This is fixed. Initially, this is its network
address, but it can be changed by the system manager.
The host will be able to send and receive packets through the link. The host has the following:

A main directory, which it can access files. This must be a subdirectory in the directory of the network
simulator.

Send packet buffer, which is a buffer that holds a packet to be sent on the outgoing link. More about
this below.

Receive packet buffer, which is buffer that holds a packet that is received on the incoming link. More
about this below.

The following are the state values of a host.

physical ID: Physical ID, which is unique among nodes. It is assigned at the start of the program, and
doesn’t change thereafter. This is implemented as an integer data type.

maindir: Name of its main directory. This can be changed, and initially, it’s blank. This is implemented
as a character string.

netaddr: The network address for the host. This can be changed. Initially, it’s blank. Implemented as
an integer data type.

nbraddr: This is the address of the node the host is connected to, i.e., it’s neighbor node. Initially, the
host will not know it’s neighbor’s address. This state is initialized to 0xffff to indicate that the neighbor
is unknown. The host then will learn its neighbor’s address whenever it receives a packet and checks
the source address field. This is implemented as an integer.

rcvflag: This flag indicates if a new packet has been received. It is set to 1 whenever a new packet is
received. A user may clear this flag to 0.

The host can send and receive packets. When it receives a packet, it stores it in a receive packet buffer. The receive
buffer should be a struct which has members for the source address, destination address, length, and payload.
Whenever a packet is received
If its destination address is the same as the host’s network address then flags are set to indicate that a
new packet has been downloaded into the receive packet buffer
Otherwise, the packet is discarded
When a host sends a packet, it comes from its send packet buffer. The send packet buffer should have enough
space for the payload as well as for other things like source address, destination address, and length. But
that’s an implementation decision.
Whenever a host sends a packet (from the send packet buffer) on the outgoing link, the source address of the
packet is the host’s address.

Besides sending and receiving packets, a host can

Upload a file (from its main directory) into the payload of the send packet buffer.

Download the payload from the receive packet buffer into a file in its main directory.

Display the state of the host

Change the state of the host

SYSMAN

This is connected to each host through pipes as shown in Figure 2. It allows a user to send commands and
receives replies to one host at a time. The host that the SYSMAN is currently connected to is the active host,
and the connection is the active connection. The following is what the user can do with the SYSMAN through
the console:

List all hosts (physical IDs) with an indicator of active connection

Select host (physical ID) to be an active connection

Display state of host: physical ID, main directory, network address, neighbor address, receive flag.

Set host’s main directory

Set host’s network address

Clear host’s receive packet buffer

Download the payload of the host’s receive packet buffer into a file in the main directory. The user
specifies the file’s name. The download overwrites any existing file.

Upload a file in the main directory into the send packet buffer’s payload. The user specifies the file’s
name. If the file is too big for a packet ( i.e., > 200 bytes) then the file is not uploaded and the user gets
an error message.

Send packet in the send packet buffer.

Instructions

Your boss asks you to make three improvements to your co-worker’s software.

Improvement 1: Simulate switching nodes.

Improvement 2: Increase maximum transport size from 200 bytes to 2000 bytes.

Improvement 3: Read in a data file and simulate various network topologies.
