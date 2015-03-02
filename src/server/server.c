#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include "EventListener.h"

#include "../core/Socket.h"


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"


int callbackRegister(int connection, char * opr, char * payload) {
    return 0;
}

int callbackList(int connection, char * opr, char * payload) {
    return 0;
}

int callbackDeregister(int connection, char * opr, char * payload) {
    return 0;
}


int main(int argc, const char *argv[]) {
    printf("Starting server..");
    int sock, connected, recvBytes, _true = 1;
    char reply[1024], recvData[1024];

    struct sockaddr_in serverAddr, clientAddr;
    unsigned int sinSize;

    // for the event listener
    EventListener_t * ell = EventListener_create();
    Event_t eventRegister, eventList, eventDeregister;
    Event_fill(&eventRegister, "REG", callbackRegister);
    Event_fill(&eventList, "LIST", callbackList);
    Event_fill(&eventDeregister, "DEREG", callbackDeregister);
    EventListener_addEvent(ell, eventRegister);
    EventListener_addEvent(ell, eventList);
    EventListener_addEvent(ell, eventDeregister);


    sock = Socket(AF_INET, SOCK_STREAM, 0);
    SetSockOpt(sock, SOL_SOCKET, SO_REUSEADDR, &_true, sizeof(int));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(serverAddr.sin_zero), 8);

    Bind(sock, (struct sockaddr *) &serverAddr, sizeof(struct sockaddr));
    Listen(sock, 5);

    printf("\nMy TCP ECHO Server is Waiting for client on port 5000\n");
    fflush(stdout);

    char * ip;
    int port;

    while(1) {
        sinSize = sizeof(struct sockaddr_in);
        connected = accept(sock, (struct sockaddr *) &clientAddr, &sinSize);
        ip = inet_ntoa(clientAddr.sin_addr);
        port = ntohs(clientAddr.sin_port);

        printf("\n I got a connection from (%s , %d)", ip, port);
        fflush(stdout);

        // receives client data
        while(1) {
            recvBytes = recv(connected, recvData, 1024, 0);
            recvData[recvBytes] = '\0';
            printf("RECV=%s", recvData);
            fflush(stdout);
//            EventListener_yield(ell, connected, recvData);

            strcpy(reply, "");
            strcat(reply, "ECHO FROM SERVER: ");
            strcat(reply, recvData);

            //send reply to client
            send(connected, reply, strlen(reply), 0);
        }
    }

    EventListener_free(ell);
    return 0;
}
#pragma clang diagnostic pop