

/*
############################################################################
#                                                                          #
# Copyright TU-Berlin, 2011-2014                                           #
# Die Weitergabe, Veröffentlichung etc. auch in Teilen ist nicht gestattet #
#                                                                          #
############################################################################
*/

// Hash table server
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
#include "hash.h"

#define MAX_BUFFER_LENGTH 100

int packData(unsigned char *buffer, char *command, short int key, short int value);
void unpackData(unsigned char *buffer, char *command, short int *key, short int *value);
bool handleCommand(struct element *table, char *command, short int *key, short int *value);

int main(int argc, char *argv[])
{
    struct element *table = initTable();
    // wtf is this needed? tutor please help
    char x[1000];
    int sockfd;
    struct sockaddr_in own_addr, client_addr; // connector's address information
    struct hostent *he;
    int udpPort;

    printf("UDP server example\n\n");

    if (argc != 2) {
        fprintf(stderr,"Usage: %s udpPort\n", argv[0]);
        exit(1);
    }

    udpPort = atoi(argv[1]);

    //bring up server
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
    if(bind(sockfd, (const struct sockaddr *)&own_addr, sizeof(own_addr)) == -1) {
        fprintf(stderr, "Error creating socket\n");
        return 1;
    }

    socklen_t client_addr_size;

    char buffer[4];

    int n;
    if((n=recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_addr_size)) != 4) {
        fprintf(stderr, "Error receiving data. expected 4 bytes but got %d \n", n);
        return 1;
    }

    if( close(sockfd) == -1 ) {
        fprintf(stderr, "Error closing socket.\n");
    }

    return 0;
}

// writes 4 bytes command, 2 bytes key, 2 bytes value to buffer
int packData(unsigned char *buffer, char *command, short int key, short int value) {
    *((short *)(buffer+4)) = key;
    *((short *)(buffer+6)) = value;
    memcpy(buffer, command, 4);
}

// reads 4 bytes command, 2 bytes key, 2 bytes value from buffer
void unpackData(unsigned char *buffer, char *command, short int *key, short int *value) {
    *key = *((short *)(buffer+4));
    *value = *((short *)(buffer+6));
    memcpy(command, buffer, 4);
}
// looks which command  is in command, handles it and sets appropriate command, key and value


bool handleCommand(struct element *table, char *command, short int *key, short int *value) {
    if(strcmp(command,"GET")==0) {
        if (get(table,*key,value)){
            strcpy(command,"OK!");
        }
        else{
            strcpy(command,"ERR");
        }
        return true;
    }
    else if(strcmp(command,"DEL")==0) {
        if (delete(table,*key)){
            strcpy(command, "OK!");
        }
        else{
            strcpy(command, "ERR");
        }
        return true;
    }
    else if (strcmp(command,"SET")==0) {
        if (set(table,*key,*value)){
            strcpy(command, "VAR");
        }
        else {
            strcpy(command, "NOF");
        }
        return true;
    }
    return false;
}
