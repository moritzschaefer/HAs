/*
############################################################################
#                                                                          #
# Copyright TU-Berlin, 2011-2014                                           #
# Die Weitergabe, Veröffentlichung etc. auch in Teilen ist nicht gestattet #
#                                                                          #
############################################################################
*/

#include <stdio.h>
#include <time.h>
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

/* bool send_timestamp(int sockfd) { */
/*  */
/*     struct timespec tp; */
/*     clock_gettime(CLOCK_REALTIME, &tp); */
/*     if(send(sockfd, buffer, sizeof(buffer), 0) == -1) { */
/*         return false; */
/*     } */
/*  */
/*     return true; */
/* } */

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in their_addr; // connector's address information
    struct hostent *he;
    int numbytes;
    int serverPort;
    int a = 0;
    int b = 0;

    printf("TCP client example\n\n");

    if (argc != 5) {
        fprintf(stderr,"Usage: tcpClient serverName serverPort int1 int2\n");
        exit(1);
    }

    serverPort = atoi(argv[2]);
    a = atoi(argv[3]);
    b = atoi(argv[4]);

    //Resolv hostname to IP Address
    if ((he=gethostbyname(argv[1])) == NULL) {  // get the host info
        herror("gethostbyname");
        exit(1);
    }

    /* ******************************************************************
    TO BE DONE: Create socket
    ******************************************************************* */

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if(sockfd == -1) {
        fprintf(stderr, "Error creating socket\n");
        return 1;
    }

    //setup transport address
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(serverPort);
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);


    /* ******************************************************************
    TO BE DONE:  Binding
    ******************************************************************* */

    connect(sockfd, &their_addr, sizeof(their_addr));

    unsigned char buffer[4];

    // why was buffer *buffer before
    packData(buffer, a, b);

    /* ******************************************************************
    TO BE DONE:  Send data
    ******************************************************************* */

    printf("Sending values: %d, %d\n", a, b);
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    if(send(sockfd, buffer, sizeof(buffer), 0) == -1) {
        fprintf(stderr, "Error sending data.\n");
    }
    clock_gettime(CLOCK_REALTIME, &end);
    long time = (end.tv_sec-start.tv_sec)*1000000000 + end.tv_nsec - start.tv_nsec;
    printf("Send time was: %ld nanoseconds", time);


    /* ******************************************************************
    TO BE DONE:  Close socket
    ******************************************************************* */
    if( close(sockfd) == -1 ) {
        fprintf(stderr, "Error closing socket.\n");
        return 1;
    }

    return 0;
}

int packData(unsigned char *buffer, unsigned int a, unsigned int b) {
    /* ******************************************************************
    TO BE DONE:  pack data
    ******************************************************************* */
    buffer[0] = (unsigned char)((a >> 8)&255);
    buffer[1] = (unsigned char)(a&255);
    buffer[2] = (unsigned char)((b >> 8)&255);
    buffer[3] = (unsigned char)(b&255);
}
