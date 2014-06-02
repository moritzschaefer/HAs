
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

void packData(unsigned char *buffer, char *command, short int key, short int value,unsigned short int port, unsigned int IP);
void unpackData(unsigned char *buffer, char *command, short int *key, short int *value, unsigned short int *port, unsigned int *IP);
bool handleCommand(struct element *table, char *command, short int *key, short int *value);

int main(int argc, char *argv[])
{
    struct element *table = initTable();
    int sockfd;
    struct sockaddr_in own_addr, client_addr, follow_addr, pre_addr; // connector's address information
    struct hostent *he;
    int udpPort;
    int followPort;
    int prePort;
    int ownID, preID, followID, overflow;
    printf("UDP Hash server example\n\n");

    if (argc != 9) {
        fprintf(stderr,"Usage: %s udpPort NodeID PreNodeID PreNodeIP PreNodePort  FollowNodeID FollowNodeIP FollowNodePort \n", argv[0]);
        exit(1);
    }

    udpPort = atoi(argv[1]);
    followPort = atoi(argv[8]);
    prePort   =atoi(argv[5]);
    ownID = atoi(argv[2]);
    preID = atoi(argv[3]);
    followID = atoi(argv[6]);
    overflow=0;
    if(preID>ownID){
        overflow=1;
    }
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


    //Resolv FollowNode to IP Address


    follow_addr.sin_family = AF_INET;
    follow_addr.sin_port = htons(followPort);
    inet_pton(AF_INET, argv[7], &(follow_addr.sin_addr));
    memset(follow_addr.sin_zero, '\0', sizeof follow_addr.sin_zero);


    //bind socket
    if(bind(sockfd, (const struct sockaddr *)&own_addr, sizeof(own_addr)) == -1) {
        fprintf(stderr, "Error creating socket\n");
        return 1;
    }

    socklen_t client_addr_size = sizeof(struct sockaddr);

    char buffer[14];

    while(1) {

        int n;

        if((n=recvfrom(sockfd, (void *)buffer, 14, 0, (struct sockaddr*)&client_addr, &client_addr_size)) != 14) {
            fprintf(stderr, "Error receiving data. expected 14 bytes but got %d \n", n);
            return 1;
        }

        short int key, value;
        unsigned short int port;
        unsigned int IP;
        char command[4];
        unpackData(buffer, command, &key, &value, &port, &IP);
        int hashedkey = hash(key);
        if(port==0){
            port=ntohs(client_addr.sin_port);
            IP= ntohl(client_addr.sin_addr.s_addr);
        }

        if((overflow && (hashedkey<ownID || hashedkey>preID)) || (hashedkey<ownID && hashedkey>preID)){
            if(!handleCommand(table, command, &key, &value)) {
                fprintf(stderr, "Unknown command from client. exiting: %s", command);
                return 1;
            }
            client_addr.sin_family = AF_INET;
            client_addr.sin_port = htons(port);
            client_addr.sin_addr.s_addr = htonl(IP);

            memset(client_addr.sin_zero, '\0', sizeof client_addr.sin_zero);


            packData(buffer, command, key, value, port, IP);

            if((n=sendto(sockfd, (void *)buffer, 14, 0, (struct sockaddr*)&client_addr, sizeof(client_addr))) != 14) {
                char debugbuf[100];
                inet_ntop(AF_INET, &client_addr.sin_addr, debugbuf, 100);
                fprintf(stderr, "Error sending back data. expected 14 bytes but got %d \n", n);
                fprintf(stderr, "Debug info: port: %hd port(ntohs): %hd address: %d address inet_ntop: %s\n", client_addr.sin_port, ntohs(client_addr.sin_port), client_addr.sin_addr.s_addr, debugbuf);
                return 1;
            }
        }
        else{
            if((n=sendto(sockfd, (void *)buffer, 14, 0, (struct sockaddr*)&follow_addr, sizeof(follow_addr))) != 14) {
                fprintf(stderr, "Error sending data to next Peer. expected 14 bytes but got %d \n", n);
                return 1;
            }

        }




    }

    if( close(sockfd) == -1 ) {
        fprintf(stderr, "Error closing socket.\n");
    }

    return 0;
}

// writes 4 bytes command, 2 bytes key, 2 bytes value to buffer
void packData(unsigned char *buffer, char *command, short int key, short int value,unsigned short int port, unsigned int IP) {
    *((short *)(buffer+4)) = key;
    *((short *)(buffer+6)) = value;
    memcpy(buffer, command, 4);
    *((unsigned *)(buffer+8)) = htonl(IP);
    *((short *)(buffer+12)) = htons(port);
}

// reads 4 bytes command, 2 bytes key, 2 bytes value from buffer
void unpackData(unsigned char *buffer, char *command, short int *key, short int *value, unsigned short int *port, unsigned int *IP) {
    *key = *((short *)(buffer+4));
    *value = *((short *)(buffer+6));
    memcpy(command, buffer, 4);
    *IP =ntohl( *((unsigned *)(buffer+8)));
    *port = ntohs( *((short *)(buffer+12)));
}
// looks which command  is in command, handles it and sets appropriate command, key and value


bool handleCommand(struct element *table, char *command, short int *key, short int *value) {
    if(strcmp(command,"GET")==0) {
        if (get(table,*key,value)){
            strcpy(command,"VAL");
        }
        else{
            strcpy(command,"NOF");
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
            strcpy(command, "OK!");
        }
        else {
            strcpy(command, "ERR");
        }
        return true;
    }
    return false;
}
