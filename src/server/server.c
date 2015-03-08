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
#include <Parser.h>

#include "../core/Socket.h"
#include "../core/Db.h"
#include "../core/Parser.h"

#define CLIENT_PORT 9060


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

Db_t * database;


/**
* Called to perform login entry to DB.
*/
int callbackJoin(int sockfd, Msg_t * msg, char * ip, int port) {
    puts("");
    printf("nick=%s ip=%s port=%d\n", msg->payload, ip, port);
    Db_insert(database, DbEntry_create(msg->payload, ip, CLIENT_PORT));
    Db_show(database);
    return 0;
}


/**
* Called when logout needed.
*/
int callbackLeave(int sockfd, char * nickname) {
    Db_deletebyId(database, nickname);
    Db_show(database);
    return 0;
}


/**
* Called when list performed.
*/
int callbackList(int sockfd, char * nickname) {
    puts("List database called.");
    char * buffer = Db_serialize(database);
    send(sockfd, buffer, 1024, 0);
    return 0;
}


int main(int argc, const char *argv[]) {
    printf("Starting server..");
    int sock, connected, recvBytes, _true = 1;
    char reply[1024], recvData[1024];

    struct sockaddr_in serverAddr, clientAddr;
    unsigned int sinSize;

    database = Db_create();

    // FOR MOCKING, REMOVE IN PRODUCTION  ------------------
    Db_insert(database, DbEntry_create("test1", "127.0.0.1", 8000));
    Db_insert(database, DbEntry_create("test2", "127.0.0.1", 8001));
    // /FOR MOCKING, REMOVE IN PRODUCTION  -----------------


    sock = Socket(AF_INET, SOCK_STREAM, 0);
    SetSockOpt(sock, SOL_SOCKET, SO_REUSEADDR, &_true, sizeof(int));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(serverAddr.sin_zero), 8);

    Bind(sock, (struct sockaddr *) &serverAddr, sizeof(struct sockaddr));
    Listen(sock, 5);

    printf("\nMy TCP ECHO Server is Waiting for client on port 9000\n");
    fflush(stdout);

    char *ip;
    int port;

    while (1) {
        sinSize = sizeof(struct sockaddr_in);
        connected = accept(sock, (struct sockaddr *) &clientAddr, &sinSize);
        ip = inet_ntoa(clientAddr.sin_addr);
        port = ntohs(clientAddr.sin_port);

        printf("\n I got a connection from (%s , %d)", ip, port);
        fflush(stdout);

        Msg_t *msg;
        char buffer[1024];
        recvBytes = recv(connected, buffer, 1024, 0);
        msg = msg_fromString(buffer);
        msg_display(msg);
        fflush(stdout);

        if (strcmp(msg->command, "JOIN") == 0) {
            callbackJoin(connected, msg, ip, port);

        } else if (strcmp(msg->command, "LIST") == 0) {
            callbackList(connected, msg->payload);

        } else if (strcmp(msg->command, "BYE") == 0) {
            callbackLeave(connected, msg->payload);
        }

        msg_free(msg);
        close(connected);

    }

    Db_free(database);
    return 0;
}

#pragma clang diagnostic pop