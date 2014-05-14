
/*
############################################################################
#                                                                          #
# Copyright TU-Berlin, 2011-2014                                           #
# Die Weitergabe, Veröffentlichung etc. auch in Teilen ist nicht gestattet #
#                                                                          #
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

#define MAX_BUFFER_LENGTH 100

void unpackData(unsigned char *buffer, unsigned int *a, unsigned int *b);
int gcd ( int a, int b );
int main(int argc, char *argv[])
{
    int response_result = 1;
    int sockfd;
    struct sockaddr_in own_addr, client_addr; // connector's address information
    struct hostent *he;
    int udpPort;

    printf("UDP server example\n\n");

    if (argc != 2) {
        fprintf(stderr,"Usage: tcpServer udpPort\n");
        exit(1);
    }

    udpPort = atoi(argv[1]);

    // first udp

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if(sockfd == -1) {
        fprintf(stderr, "Error creating socket\n");
        return 1;
    }

    //Resolv hostname to IP Address
    if ((he=gethostbyname("localhost")) == NULL) {  // get the host info
        herror("gethostbyname");
        exit(1);
    }

    own_addr.sin_family = AF_INET;
    own_addr.sin_port = htons(udpPort);
    own_addr.sin_addr = *((struct in_addr *)he->h_addr);

    memset(own_addr.sin_zero, '\0', sizeof own_addr.sin_zero);
    //bind socket
    if(bind(sockfd, &own_addr, sizeof(own_addr)) == -1) {
        fprintf(stderr, "Error creating socket\n");
        return 1;
    }

    socklen_t client_addr_size;;

    char buffer[4];

    int n;
    if((n=recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_addr_size)) != 4) {
        fprintf(stderr, "Error receiving data. expected 4 bytes but got %d \n", n);
        return 1;
    }

    // why was buffer *buffer before
    unsigned int a,b;
    unpackData(buffer, &a, &b);
    int result = gcd(a,b);
    //don't print but send
    if(response_result) {
        packData(buffer, a,b);
        if((n=sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, client_addr_size)) != 4) {
            fprintf(stderr, "Error sending back data. expected 4 bytes but got %d \n", n);
            return 1;
        }
    } else {
        printf("received: %d, %d; GGT: %d\n", a, b, result);
    }

    if( close(sockfd) == -1 ) {
        fprintf(stderr, "Error closing socket.\n");
    }

    return 0;
}

int packData(unsigned char *buffer, unsigned int a, unsigned int b) {
    /* ******************************************************************
TO BE DONE: pack data
******************************************************************* */
    buffer[0] = (unsigned char)((a >> 8)&255);
    buffer[1] = (unsigned char)(a&255);
    buffer[2] = (unsigned char)((b >> 8)&255);
    buffer[3] = (unsigned char)(b&255);
}

void unpackData(unsigned char *buffer, unsigned int *a, unsigned int *b) {
    /* ******************************************************************
       TO BE DONE:  pack data
     ******************************************************************* */
    *a = (buffer[0]<<8) | buffer[1];
    *b = (buffer[2]<<8) | buffer[3];
}

int gcd ( int a, int b ) {
    int c;
    while ( a != 0 ) {
        c = a; a = b%a;  b = c;
    }
    return b;
}
