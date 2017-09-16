#include <server.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>

using namespace std;

Server::Server(const char* port) {
    struct addrinfo hints;
    struct sigaction sa;
    int rv;
    int yes = 1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, port, &hints, &(this->servinfo))) != 0) {
        throw gai_strerror(rv);
    }

    for (this->p = this->servinfo; this->p != NULL; this->p = this->p->ai_next) {
        if ((this->sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }


        if (setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
            throw "setsockopt";
        }

        if (bind(this->sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(this->sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    if (this->p == NULL) {
        throw "server: failed to bind";
    }

    if (listen(sockfd, SOMAXCONN) == -1) {
        throw "listen";
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        throw "sigaction";
    }

    cout << "server: waiting for connections..." << endl;
}

Server::~Server() {
    freeaddrinfo(this->servinfo);
}

Server::accept() {
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;

    sin_size = sizeof their_addr;
    this->connfd = accept(this->sockfd, (struct sockaddr *) &their_addr, &sin_size);
    
    if (this->connfd == -1) {
        throw "accept";
    }

    inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *) &their_addr),
            this->ip, sizeof this->ip);
    
    cout << "server: got connection from " << this->ip << endl;
}

void sigchld_handler(int s) {
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*) sa)->sin6_addr);
}