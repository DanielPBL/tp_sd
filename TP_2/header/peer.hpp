#ifndef __PEER_HPP__
#define __PEER_HPP__

#include <parser/parser.hpp>
#include <message.hpp>
#include <msgfct.hpp>
#include <string>
#include <map>

class Peer;

typedef struct CONEXAO {
    int connfd;
    Peer *self;
} Conexao;

typedef struct VIZINHO {
    std::string ip;
    std::string porta;
    int sockfd;
} Vizinho;

class Peer {
public:
    int cpid;
private:
    Parser parser;
    MessageFactory msgFct;
    std::string ip;
    std::string porta;

    int sockfd;
    Vizinho next;
    std::map<int, std::string> tuplas;
public:
    Peer(std::string addr, std::string port);
    ~Peer();

    void serve();
    void psend(int connfd, Message *msg);
    Message *receive(int connfd);
    void parse(std::string cmd);
    void respond(int connfd, Message *msg);

    static void* respond(void *con);
};

#endif
