
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
    int sockfd;
    struct sockaddr_in own_addr, client_addr; // connector's address information
    struct hostent *he;
    int tcpPort, udpPort;

    printf("TCP server example\n\n");

    if (argc != 3) {
        fprintf(stderr,"Usage: tcpServer tcpPort udpPort\n");
        exit(1);
    }

    tcpPort = atoi(argv[1]);
    udpPort = atoi(argv[2]);

    // first tcp

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
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
    own_addr.sin_port = htons(tcpPort);
    own_addr.sin_addr = *((struct in_addr *)he->h_addr);

    memset(own_addr.sin_zero, '\0', sizeof own_addr.sin_zero);
    //bind socket
    if(bind(sockfd, &own_addr, sizeof(own_addr)) == -1) {
        fprintf(stderr, "Error creating socket\n");
        return 1;
    }
    if(listen(sockfd, 1) == -1)  {
        fprintf(stderr, "Error listenning\n");
        return 1;
    }


    socklen_t client_addr_size = sizeof(struct sockaddr_in);

    int clientfd = accept(sockfd, &client_addr, &client_addr_size);
    if(clientfd == -1) {
        fprintf(stderr, "Error accetping.\n");
        return 1;
    }

    char buffer[4];

    if(recv(clientfd, buffer, sizeof(buffer), 0) <= 0) {
        fprintf(stderr, "Error receiving data\n");
        return 1;
    }


    // why was buffer *buffer before
    unsigned int a,b;
    unpackData(buffer, &a, &b);
    printf("received: %d, %d; GGT: %d\n", a, b, gcd(a,b));

    if( close(sockfd) == -1 ) {
        fprintf(stderr, "Error closing socket.\n");
    }
    if( close(clientfd) == -1 ) {
        fprintf(stderr, "Error closing client socket.\n");
        return 1;
    }

    return 0;
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
