#include "Socket.h"

int Socket(int domain, int type, int protocol) {
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }
    return sock;
}

void SetSockOpt(int sockfd, int level, int optname, const void *optval, socklen_t optlen) {
    if (setsockopt(sockfd, level, optname, optval, optlen) == -1) {
        perror("Setsockopt");
        exit(1);
    }
}

void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    if (bind(sockfd, addr, sizeof(struct sockaddr)) == -1) {
        perror("Unable to bind");
        exit(1);
    }
}


void Listen(int sockfd, int backlog) {
    if (listen(sockfd, backlog) == -1) {
        perror("Listen");
        exit(1);
    }
}

int Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    if (connect(sockfd, addr, addrlen) == -1) {
        perror("Unable to connect");
    }
}
