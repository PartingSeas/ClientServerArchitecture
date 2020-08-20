/*
 * Name			: Manzel Gomez
 * This File	: server.c
 *
 * Description	: server C program for a client-server architecture
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>				// for gethostbyname

#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>

int main(int argc, char *argv[]) {

	if (argc < 2) {
		fprintf(stderr, "Usage %s <port no>\n", argv[0]);
		return 1;
	}

	// STEP 0)
	// get information about the port on which this server must be running
	int listenPort = atoi(argv[1]);
	printf("          program [%s]\n", argv[0]);
	printf("       listenPort [%d]\n", listenPort);


	// STEP 1)
	// create socket:
	int sockfd;					// scoket file descriptor
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)  {
		fprintf(stderr,"**** ERROR: could not create socket\n");
		exit(2);
	}


	// STEP 2)
	// create a communication end-point, i.e., half-socket - ip address and port number:
	struct sockaddr_in srvrAddr;
	memset(&srvrAddr, 0, sizeof(srvrAddr));
	srvrAddr.sin_family = AF_INET;
	srvrAddr.sin_addr.s_addr = INADDR_ANY;
	srvrAddr.sin_port = htons(listenPort);


	// STEP 3)
	// bind socket to this address
     if ( bind(sockfd, (struct sockaddr *) &srvrAddr, sizeof(srvrAddr)) < 0 ) {
		fprintf(stderr,"**** ERROR: could not bind socket\n");
		exit(3);
	 }


	// STEP 4)
	// listen on the listening port
	int blQ = 10;	// backlog Queue size
	if ( listen(sockfd, blQ) < 0 ) {
		fprintf(stderr,"**** ERROR: could not listen\n");
		exit(4);
	}


	// STEP 5)
	// accept client connection
	struct sockaddr_in clntAddr;
	socklen_t clntAddrLen = sizeof(clntAddr);
	int clntfd = accept(sockfd, (struct sockaddr *) &clntAddr, &clntAddrLen);
	if (clntfd < 0)  {
		fprintf(stderr,"**** ERROR: could not accept client connection\n");
		exit(5);
	}


	// we are here that means none of those above 5 steps errored out.
	// socket read-write loop
	char readBuffer[8192];
	char writeBuffer[8192];
	memset(readBuffer, 0, sizeof(readBuffer));
	memset(writeBuffer, 0, sizeof(writeBuffer));
	int nBytesRead, nBytesWritten;
	while ( (nBytesRead = read(clntfd, readBuffer, 8191)) > 0 ) {
		fprintf(stdout, "READ: %s\n", readBuffer);
		// depending on what client has asked for, change the contents of writeBuffer and write it.
		strcpy(writeBuffer, "Hello Welcome");
		nBytesWritten = write(clntfd, writeBuffer, 8191);
		if(nBytesWritten < 0) {
			fprintf(stderr,"**** ERROR: socket write error\n");
			break;		// exit(6);
		}
	}

	if(nBytesRead < 0) {
		fprintf(stderr,"**** ERROR: socket read error\n");
		exit(7);
	}


	return 0;
} // end of int main(int argc, char *argv[])

