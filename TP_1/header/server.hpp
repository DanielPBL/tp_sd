#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <arpa/inet.h>

class Server {
private:
    int sockfd, connfd;
    struct addrinfo *servinfo, *p;
    char ip[INET6_ADDRSTRLEN];
public:
    Server(const char*);
    ~Server();
    accept();
};

void sigchld_handler(int);
void* get_in_addr(struct sockaddr*);

#endif
