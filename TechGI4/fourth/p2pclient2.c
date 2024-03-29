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
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MAX_BUFFER_LENGTH 100
void unpackData(unsigned char *buffer, char *command, short int *key, short int *value);
void packData(unsigned char *buffer, char *command, short int key, short int value, unsigned int ownAddress, short unsigned int ownPort);

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in their_addr; // connector's address information
    struct hostent *he;
    int numbytes;
    int serverPort;
    short unsigned int ownPort;
    unsigned int ownAddress;
    short int a = 0;
    short int b = 0;

    printf("TCP client example\n\n");

    if (argc != 3) {
        fprintf(stderr,"Usage: serverName, serverPort\n");
        exit(1);
    }
    ownPort= htons(0);
    inet_pton(AF_INET,"0.0.0.0",&ownAddress);
    serverPort = atoi(argv[2]);

    //Resolv hostname to IP Address
    if ((he=gethostbyname(argv[1])) == NULL) {  // get the host info
        herror("gethostbyname");
        exit(1);
    }

        /* ******************************************************************
       TO BE DONE: Create socket
     ******************************************************************* */

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    // make socket nonblocking
    //fcntl(sockfd, F_SETFL, O_NONBLOCK);
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

    unsigned char buffer[14];

    char string[4];
    // why was buffer *buffer before
    int i;
    time_t t;
    time(&t);
    srand((unsigned int)t);
    short int keys[25];
    int n;
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);


    for(i=0;i<100;i++){
        if(i<25) {
            strcpy(string ,"SET");
            a=(short int)rand();
            keys[i]=a;
            b=(short int)rand();
        }
        if((24<i&& i<50)||74<i){
            strcpy(string ,"GET");
            a=keys[i % 25];
            b=0;
        }
        if(49<i && i<75){
            strcpy(string ,"DEL");
            a=keys[i % 25];
            b=0;
        }
        packData(buffer,string, a, b,ownAddress, ownPort);
        struct timeval waittime;
        fd_set readfds;
        socklen_t their_size;

        do {
            printf("Sending values: %d, %d, operation: %s \n", a, b, string);

            if((n = sendto(sockfd, buffer, sizeof(buffer), 0, (const struct sockaddr *) &their_addr, sizeof(their_addr))) != 14) {
                fprintf(stderr, "Error sending data. expected 8, was %d \n", n);

            }
            FD_ZERO(&readfds);
            FD_SET(sockfd, &readfds);
            waittime.tv_usec = 0; waittime.tv_sec = 2;
            if(select(sockfd+1, &readfds, NULL, NULL, &waittime) == -1) {
                fprintf(stderr, "Error in select\n");
                exit(1);
            }
        } while(!FD_ISSET(sockfd,  &readfds));

        if(recvfrom(sockfd, buffer, 14, 0, (struct sockaddr *) &their_addr, &their_size) != 14) {
            fprintf(stderr, "Error receiving data.\n");
        }
        unpackData(buffer, string, &a,&b);
        if(strcmp(string,"VAL")==0) printf("Result Value %d succeed\n",b);
        if(strcmp(string,"NOF")==0) printf("Key not found\n");
        if(strcmp(string,"OK!")==0) printf("Operation succed\n");
        if(strcmp(string,"ERR")==0) printf("Operation failed\n");

    }

    clock_gettime(CLOCK_REALTIME, &end);
    long time = ((end.tv_sec-start.tv_sec)*1000000000 + end.tv_nsec - start.tv_nsec)/100;
    printf("Avarage Send-time was: %ld nanoseconds\n", time);

    /* ******************************************************************
       TO BE DONE:  Close socket
     ******************************************************************* */
    if( close(sockfd) == -1 ) {
        fprintf(stderr, "Error closing socket.\n");
        return 1;
    }

    return 0;
}

// writes 4 bytes command, 2 bytes key, 2 bytes value to buffer
void packData(unsigned char *buffer, char *command, short int key, short int value, unsigned int ownAddress, short unsigned int ownPort) {
    *((short *)(buffer+4)) = key;
    *((short *)(buffer+6)) = value;
    memcpy(buffer, command, 4);
    *((short *)(buffer+8)) = ownAddress;
    *((short *)(buffer+12))= ownPort;
}

// reads 4 bytes command, 2 bytes key, 2 bytes value from buffer
void unpackData(unsigned char *buffer, char *command, short int *key, short int *value) {
    *key = *((short *)(buffer+4));
    *value = *((short *)(buffer+6));
    memcpy(command, buffer, 4);
}

int packDataOld(unsigned char *buffer,char *string, unsigned int a, unsigned int b) {
    /* ******************************************************************
       TO BE DONE:  pack data
a: Key
b: Value
     ******************************************************************* */
    // better memcpy or strcpy
    buffer[0] = (unsigned char) string[0];
    buffer[1] = (unsigned char) string[1];
    buffer[2] = (unsigned char) string[2];
    buffer[3] = (unsigned char) '\0';
    buffer[4] = (unsigned char)((a >> 8)&255);
    buffer[5] = (unsigned char)(a&255);
    buffer[6] = (unsigned char)((b >> 8)&255);
    buffer[7] = (unsigned char)(b&255);
}
int unpackDataOld(unsigned char *buffer,char *string, unsigned int *a,unsigned int *b) {
    /* ******************************************************************
       TO BE DONE:  pack data
     ******************************************************************* */
    // better memcpy or strcpy
    buffer[0] = (unsigned char) string[0];
    string[0] = buffer[0];
    string[1] = buffer[1];
    string[2] = buffer[2];
    string[3] = buffer[3];
    *a = (buffer[4]<<8) | buffer[5];
    *b = (buffer[6]<<8) | buffer[7];
}
