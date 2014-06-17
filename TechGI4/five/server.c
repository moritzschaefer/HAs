/*
############################################################################
# SERVER.C
opyright TU-Berlin, 2011-2014 #
# Die Weitergabe, Veröffentlichung etc. auch in Teilen ist nicht gestattet #
# #
############################################################################
*/

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
#include <time.h>

#define false 0
#define true  1

#define MAX_BUFFER_LENGTH 100

int  unpackData(unsigned char *buffer);
void packData(unsigned char *buffer, struct timespec *t2, struct timespec *t3);

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in own_addr, client_addr; // connector's address information
    struct hostent *he;
    int udpPort;
    struct timespec t2, t3;

    printf("Server starts \n");

    if (argc != 2) {
        fprintf(stderr,"Usage: udpPort\n");
        exit(1);
    }
	printf("server port reading\n");
    //Port setzen
    udpPort = atoi(argv[1]); //atoi holt aus einem String eine Zahl
	printf("server port is readed\n");	
	printf("server create a socket\n");
	
    //set a udp Server
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if(sockfd == -1) {
        fprintf(stderr, "Error creating socket\n");
        return 1;
    }
	printf("Server socked created\n");	

    //Resolv hostname to IP Address
    if ((he=gethostbyname("localhost")) == NULL) { // get the host info
        herror("gethostbyname");
	printf("servername couldn't set");
        exit(1);
    }
	printf("Servername localhost is set");

    own_addr.sin_family = AF_INET;
    own_addr.sin_port = htons(udpPort);
    own_addr.sin_addr = *((struct in_addr *)he->h_addr);

    memset(own_addr.sin_zero, '\0', sizeof own_addr.sin_zero);
    //bind socket
    if(bind(sockfd, (const struct sockaddr*)&own_addr, sizeof(own_addr)) == -1) {
        fprintf(stderr, "Error creating socket\n");
        return 1;
    }

    socklen_t client_addr_size;

    char buffer[20];

    int n;
    while(1) {
		printf("server ist waiting for Request");
        if((n=recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_addr_size)) != 20) {
            fprintf(stderr, "Error receiving data. expected 20 bytes but got %d \n", n);
            return 1;
        }

		if(unpackData(buffer)==false){
			printf("Error, unknown command");
		}
		
        //Zweiter Zeitstempel
        clock_gettime(CLOCK_REALTIME, &t2);
        printf("Request received");
        if(!checkMessage(buffer)) {
            fprintf(stderr, "bad message received");
            return 1;
        }
        //Dritter Zeitstempel
        clock_gettime(CLOCK_REALTIME, &t3);
        //don't print but send
        //define command
        packData(buffer, &t2, &t3);
        if((n=sendto(sockfd, buffer, 20, 0, (struct sockaddr*)&client_addr, client_addr_size)) != 20) {
            fprintf(stderr, "Error sending back data. expected 2 bytes but got %d \n", n);
            return 1;
        }
    }

    if( close(sockfd) == -1 ) {
        fprintf(stderr, "Error closing socket.\n");
    }

    return 0;
}


/* ******************************************************************
   TO BE DONE: unpack data
 ******************************************************************* */
int unpackData(unsigned char *buffer){
    return(memcmp("REQ", buffer,4));
}

// writes 4 bytes command, the rest null
void packData(unsigned char *buffer, struct timespec *t2, struct timespec *t3) {
    /* ******************************************************************
       TO BE DONE: pack data
     ******************************************************************* */
    memcpy(buffer, "RES", 4);
    *((short *)(buffer+4)) 	= t2->tv_sec;
    *((short *)(buffer+8))	= t2->tv_nsec;
    *((short *)(buffer+12)) = t3->tv_sec;
    *((short *)(buffer+16)) = t3->tv_nsec;
}

int checkMessage(unsigned char *buffer) {
    return true;
}

