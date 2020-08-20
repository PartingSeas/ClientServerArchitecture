/*
 * Name			: Manzel Gomez
 * This File	: client.c
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

	if (argc < 3) {
		fprintf(stderr, "Usage %s <ip address> <port no>\n", argv[0]);
		return 1;
	}

	char *hostName = argv[1];
	int port = atoi(argv[2]);
	printf("        program [%s]\n", argv[0]);
	printf("       hostName [%s]\n", hostName);
	printf("           port [%d]\n", port);


	// STEP 0)
	// get information about the host
	struct hostent *serverHost;
	serverHost = gethostbyname(hostName);
	if (serverHost == NULL) {
		fprintf(stderr,"**** ERROR: could not find host entry for host %s\n", hostName);
		exit(1);
	}


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
	memset(&srvrAddr, 0, sizeof srvrAddr);
	srvrAddr.sin_family = AF_INET;
	memcpy(&srvrAddr.sin_addr.s_addr, serverHost->h_addr, serverHost->h_length);
	srvrAddr.sin_port = htons(port);


	// STEP 3)
	// connect with the server
     if ( connect(sockfd, (struct sockaddr *) &srvrAddr, sizeof(srvrAddr)) < 0 ) {
		fprintf(stderr,"**** ERROR: could not connect socket\n");
		exit(3);
	 }


	// STEP 4)
	// read-write loop - communication
	char readBuffer[8192];
	char writeBuffer[8192];
	memset(readBuffer, 0, sizeof(readBuffer));
	memset(writeBuffer, 0, sizeof(writeBuffer));
	int nBytesRead, nBytesWritten;
	while ( fgets(writeBuffer, 8191, stdin) != NULL) {

		nBytesWritten = write(sockfd, writeBuffer, 8191);
		if(nBytesWritten < 0) {
			fprintf(stderr,"**** ERROR: socket write error\n");
			break;		// exit(6);
		}

		nBytesRead = read(sockfd, readBuffer, 8191);

		fprintf(stdout, "%s\n", readBuffer);
		fprintf(stdout, "_> ");
	}

	if(nBytesRead < 0) {
		fprintf(stderr,"**** ERROR: socket read error\n");
		exit(4);
	}


	return 0;
} // end of int main(int argc, char *argv[])

