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

#define MAX_BUFFER_LENGTH 1000000
char *buffer;

int main(int argc, char *argv[])
{
    buffer = calloc(1, MAX_BUFFER_LENGTH);
    char *host = "weather.yahooapis.com";
    char *request = "/forecastrss?w=638242&u=c";
    int sockfd;
    struct sockaddr_in their_addr; // connector's address information
    struct hostent *he;
    int numbytes;
    int serverPort;

    printf("fetch temperature from yahoo");

    //Resolv hostname to IP Address
    if ((he=gethostbyname(host)) == NULL) {  // get the host info
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
    serverPort = 80;




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

    if(!send_http_header(sockfd, host, request)) {
        fprintf(stderr, "Error sending data.\n");
    }
    clock_gettime(CLOCK_REALTIME, &end);
    long time = (end.tv_sec-start.tv_sec)*1000000000 + end.tv_nsec - start.tv_nsec;
    printf("Send time was: %ld nanoseconds\n", time);

    int temperature;
    if(!receive_http(sockfd, &temperature)) {
        printf("Error receiving data\n");
    }
    printf("Current temperature in Berlin is %d", temperature);

    /* ******************************************************************
    TO BE DONE:  Close socket
    ******************************************************************* */
    if( close(sockfd) == -1 ) {
        fprintf(stderr, "Error closing socket.\n");
        return 1;
    }

    return 0;
}

int send_http_header(int sockfd, char *host, char *request) {
    if (send(sockfd, "GET ", strlen("GET "), 0) == -1)
        return 0;
    if (send(sockfd, request, strlen(request), 0) == -1)
        return 0;
    if (send(sockfd, " HTTP/1.0\nHost: ", strlen( " HTTP/1.0\nHost: "), 0) == -1)
        return 0;
    if (send(sockfd, host, strlen(host), 0) == -1)
        return 0;
    if (send(sockfd, "\n\n", 2, 0) == -1)
        return 0;
    return 1;
}

int receive_http(int sockfd, int *temperature) {
    int size=0;
    int current_pos=0;
    while((size=recv(sockfd, buffer+current_pos, MAX_BUFFER_LENGTH-current_pos, 0)) != 0 ) {
        if(size == -1)
            return 0;
    }
    char *pos = strstr(buffer, "yweather:condition");
    if(pos==NULL) {
        printf( "where is it");
        return 0;
    }
    pos = strstr(pos, "temp=\"")+strlen("temp=\"");
    char *secondpos = strstr(pos, "\"");
    secondpos[0] = '\0';
    *temperature = atoi(pos);
    return 1;
}
