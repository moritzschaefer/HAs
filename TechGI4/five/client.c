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

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in their_addr; // connector's address information
    struct hostent *he;
    int numbytes;
    int serverPort;
	char[] command; //keine ahnung ob das so richtig ist!

    if (argc != 3) {
        fprintf(stderr,"Usage: tcpClient serverName serverPort\n");
        exit(1);
    }
	
	//set server port
    serverPort = atoi(argv[2]);

    //Resolv hostname to IP Address
    if ((he=gethostbyname(argv[1])) == NULL) { // get the host info
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

	
	//ersten Zeitstempel nehmen
	struct timespec t1,t2,t3,t4;
    clock_gettime(CLOCK_REALTIME, &t1);

/* ******************************************************************
TO BE DONE: Packing
******************************************************************* */

    //connect(sockfd, &their_addr, sizeof(their_addr));

    unsigned char buffer[20];

    // why was buffer *buffer before
    packData(buffer,"REQ");

/* ******************************************************************
TO BE DONE: Send data
******************************************************************* */

    printf("Sending request to server...");

    socklen_t their_size;
    int n;
    if((n = sendto(sockfd, buffer, sizeof(buffer), 0, (const struct sockaddr *) &their_addr, sizeof(their_addr))) != 20) {
        fprintf(stderr, "Error sending data. expected 20 Byte, was %d \n", n);
    }

    if(recvfrom(sockfd, buffer, 2, 0, (const struct sockaddr *) &their_addr, &their_size) != 2) {
        fprintf(stderr, "Error receiving data.\n");
    }
	clock_gettime(CLOCK_REALTIME, &t4);
    unpackData(buffer, &a);
	
	//Calc the results
	int offsetInSec = 0.5*((t4.tv_spec-t1.tv_spec)+(t3.tv_spec-2t.v_spec));
	int delayInSec   = (t4.tv_spec-t1.tv_spec)-(t3.tv_spec-t2.tv_spec);	
	
    printf("Received result: %d", delayInSec);

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
void packData(unsigned char *buffer, char *command) {
/* ******************************************************************
TO BE DONE: pack data
******************************************************************* */
	memcpy(buffer, command, 4);
}


int unpackData(unsigned char *buffer, *command) {
/* ******************************************************************
TO BE DONE: unpack data
******************************************************************* */
	void unpackData(unsigned char *buffer, char *command, short int *t2, short int *t3) {
	memcpy(command, buffer, 4);
    *t2 = *((timespec *)(buffer+4));
    *t3 = *((timespec *)(buffer+12));
}
