/*
############################################################################
# CLIENT.C 
opyright TU-Berlin, 2011-2014 #
# Die Weitergabe, Veröffentlichung etc. auch in Teilen ist nicht gestattet #
# #
############################################################################
 */
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAX_BUFFER_LENGTH 100

void unpackData(unsigned char *buffer, struct timespec *t2, struct timespec *t3);
void packData(unsigned char *buffer);

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in their_addr; // connector's address information
	struct hostent *he;
	int numbytes;
	int serverPort;

	printf("Client starts\n");

	if (argc != 3) {
		fprintf(stderr,"Usage: serverName serverPort\n");
		exit(1);
	}

	//set server port
	serverPort = atoi(argv[1]);

	//Resolv hostname to IP Address
	if ((he=gethostbyname(argv[0])) == NULL) { // get the host info
		herror("gethostbyname");
		exit(1);
	}

	/* ******************************************************************
	   TO BE DONE: Create socket
	 ******************************************************************* */
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if(sockfd == -1) {
		fprintf(stderr, "Error creating socket\n");
		return 1;
	}

	//setup transport address
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(serverPort);
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

	printf("Client socket is rdy\n");

	//ersten Zeitstempel nehmen
	struct timespec t1,t2,t3,t4;
	clock_gettime(CLOCK_REALTIME, &t1);

	printf("t1 is taken\n");
	/* ******************************************************************
	   TO BE DONE: Packing
	 ******************************************************************* */
	unsigned char buffer[20];
	// why was buffer *buffer before
	packData(buffer);

	/* ******************************************************************
	   TO BE DONE: Send data
	 ******************************************************************* */
	printf("Sending request to server...\n");

	socklen_t their_size;
	int n;
	if((n = sendto(sockfd, buffer, sizeof(buffer), 0, (const struct sockaddr *) &their_addr, sizeof(their_addr))) != 20) {
		fprintf(stderr, "Error sending data. expected 20 Byte, was %d \n", n);
	}
	printf("waitung for response");

	if(recvfrom(sockfd, buffer, 20, 0, (const struct sockaddr *) &their_addr, &their_size) != 20) {
		fprintf(stderr, "Error receiving data.\n");
	}
	printf("Answer from server comes\n");
	clock_gettime(CLOCK_REALTIME, &t4);
	printf("t4 is taken\n");
	unpackData(buffer,&t2,&t3);

	//Calc the results
	int offsetInSec = 0.5*((t4.tv_sec-t1.tv_sec)+(t3.tv_sec-t2.tv_sec));
	int delayInSec  = (t4.tv_sec-t1.tv_sec)-(t3.tv_sec-t2.tv_sec);	

	fprintf("Received result: %d", delayInSec);

	/* ******************************************************************
	   TO BE DONE: Close socket
	 ******************************************************************* */
	if( close(sockfd) == -1 ) {
		fprintf(stderr, "Error closing socket.\n");
		return 1;
	}

	return 0;
}

// writes 4 bytes command, the rest null
/* ******************************************************************
   TO BE DONE: pack data
 ******************************************************************* */
void packData(unsigned char *buffer) {
	memcpy(buffer, "REQ", 4);
}


/* ******************************************************************
   TO BE DONE: unpack data
 ******************************************************************* */
void unpackData(unsigned char *buffer, struct timespec *t2, struct timespec *t3) {
	memcpy("RES", buffer, 4);
	*t2 = *((struct timespec *)(buffer+4));
	*t3 = *((struct timespec *)(buffer+12));
}
