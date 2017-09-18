#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <arpa/inet.h>
#include <message.hpp>
#include <map>

typedef struct SERVERINFO {
    char ip[INET6_ADDRSTRLEN];
    int port;
    int socket;
    int bc_port;
    struct sockaddr_in remoteServAddr;
    std::map<std::string, std::string> sl;
} ServerInfo;

class Server {
private:
    int sockfd, connfd;
    static void* broadcaster(void *arg);
public:
    ServerInfo si;

    Server(const char* ip, const char* port, const char* bc_port);
    ~Server();
    void saccept();
    Message* receive();
    void ssend(Message &msg);
    void broadcast();
};

void sigchld_handler(int);
void* get_in_addr(struct sockaddr*);

#endif
