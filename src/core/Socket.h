#ifndef SOCKET_H
#define SOCKET_H

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

/**
* This is a wrapper class for socket functions, comprising error messages.
*/

int Socket(int domain, int type, int protocol);

void SetSockOpt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

void Listen(int sockfd, int backlog);

int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

#endif SOCKET_H

