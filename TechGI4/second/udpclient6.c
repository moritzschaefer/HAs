/*
############################################################################
#                                                                          #
# Copyright TU-Berlin, 2011-2014                                           #
# Die Weitergabe, Veröffentlichung etc. auch in Teilen ist nicht gestattet #
#                                                                          #
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
    int a = 0;
    int b = 0;

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

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    /* ******************************************************************
       TO BE DONE: Create socket
     ******************************************************************* */

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if(sockfd == -1) {
        fprintf(stderr, "Error creating socket\n");
        return 1;}
    //setup transport address
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(serverPort);
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);


    /* ******************************************************************
       TO BE DONE:   Packing
     ******************************************************************* */

    //connect(sockfd, &their_addr, sizeof(their_addr));

    unsigned char buffer[8];

    char string[4];
    // why was buffer *buffer before
    int i;
    time_t t;
    time(&t);
    srand((unsigned int)t);
    int keys[25];
    int n;
    for(i=0;i<100;i++){
        if(i<25) {
            strcpy(string ,"SET");
            a=rand();
            keys[i]=a;
            b=rand();
        }
        if(24<i<50||74<i){
            strcpy(string ,"GET");
            a=keys[i % 25];
            b=0;
        }
        if(49<i<75){
            strcpy(string ,"DEL");
            a=0;
            b=0;
        }
        packData(buffer,string, a, b);

        /* ******************************************************************
           TO BE DONE:  Send data
         ******************************************************************* */

        printf("Sending values: %d, %d\n", a, b);

        socklen_t their_size;
        if((n = sendto(sockfd, buffer, sizeof(buffer), 0, (const struct sockaddr *) &their_addr, sizeof(their_addr))) != 8) {
            fprintf(stderr, "Error sending data. expected 4, was %d \n", n);
        }
        if(recvfrom(sockfd, buffer, 2, 0, (const struct sockaddr *) &their_addr, &their_size) != 8) {
            fprintf(stderr, "Error receiving data.\n");
        } 
        unpackData(buffer,&string, &a,&b);
        if(strcmp(string,"VAL\0")==0) printf("Result Value %d succeed",b);
        if(strcmp(string,"NOF\0")==0) printf("Key not found");
        if(strcmp(string,"OK!\0")==0) printf("Insertion succed");
        if(strcmp(string,"ERR\0")==0) printf("Insertion failed");

    }

    clock_gettime(CLOCK_REALTIME, &end);
    long time = (end.tv_sec-start.tv_sec)*1000000000 + end.tv_nsec - start.tv_nsec;
    printf("Send time was: %ld nanoseconds\n", time);

    /* ******************************************************************
       TO BE DONE:  Close socket
     ******************************************************************* */
    if( close(sockfd) == -1 ) {
        fprintf(stderr, "Error closing socket.\n");
        return 1;
    }

    return 0;
}

int packData(unsigned char *buffer,char string[], unsigned int a, unsigned int b) {
    /* ******************************************************************
       TO BE DONE:  pack data
a: Key
b: Value
     ******************************************************************* */
    buffer[0] = (unsigned char) string[0];
    buffer[1] = (unsigned char) string[1];
    buffer[2] = (unsigned char) string[2];
    buffer[3] = (unsigned char) '\0';
    buffer[4] = (unsigned char)((a >> 8)&255);
    buffer[5] = (unsigned char)(a&255);
    buffer[6] = (unsigned char)((b >> 8)&255);
    buffer[7] = (unsigned char)(b&255);
}
int unpackData(unsigned char *buffer,char *string[], unsigned int *a,unsigned int *b) {
    /* ******************************************************************
       TO BE DONE:  pack data
     ******************************************************************* */
    *string[0] = buffer[0];
    *string[1] = buffer[1];
    *string[2] = buffer[2];
    *string[3] = buffer[3];
    *a = (buffer[4]<<8) | buffer[5];
    *b = (buffer[6]<<8) | buffer[7];
}
