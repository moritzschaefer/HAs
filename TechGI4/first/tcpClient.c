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
#include <string.h>
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

/* int send_timestamp(int sockfd) { */
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

    printf("TCP client example\n\n");

    if (argc != 3) {
        fprintf(stderr,"Usage: tcpClient serverName serverPort\n");
        exit(1);
    }

    serverPort = atoi(argv[2]);

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



    /* ******************************************************************
    TO BE DONE:  Send data
    ******************************************************************* */

    printf("Sending header");
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    if(!send_http_header(sockfd, argv[1])) {
        fprintf(stderr, "Error sending data.\n");
    }
    clock_gettime(CLOCK_REALTIME, &end);
    long time = (end.tv_sec-start.tv_sec)*1000000000 + end.tv_nsec - start.tv_nsec;
    printf("Send time was: %ld nanoseconds\n", time);

    if(!receive_http(sockfd)) {
        printf("Error receiving data\n");
    }

    /* ******************************************************************
    TO BE DONE:  Close socket
    ******************************************************************* */
    if( close(sockfd) == -1 ) {
        fprintf(stderr, "Error closing socket.\n");
        return 1;
    }

    return 0;
}

int send_http_header(int sockfd, char *host) {
    const char string[] = "GET / HTTP/1.0\nHost: ";
    if (send(sockfd, string, strlen(string), 0) == -1)
        return 0;
    if (send(sockfd, host, strlen(host), 0) == -1)
        return 0;
    if (send(sockfd, "\n\n", 2, 0) == -1)
        return 0;
    return 1;
}

int receive_http(int sockfd) {
    char buf[4096];
    int size;
    while((size=recv(sockfd, buf, 4096, 0)) != 0) {
        if(size == -1)
            return 0;
        printf("%.*s", size, buf);
    }
    return 1;
}
