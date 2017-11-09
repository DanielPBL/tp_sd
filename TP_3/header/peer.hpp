#ifndef __PEER_HPP__
#define __PEER_HPP__

#include <parser/parser.hpp>
#include <message.hpp>
#include <msgfct.hpp>
#include <string>
#include <map>
#include <utility>
#include <ctime>

class Peer;

typedef struct CONEXAO {
    int connfd;
    Peer *self;
} Conexao;

typedef struct VIZINHO {
    std::string ip;
    std::string porta;
    int sockfd;
    unsigned long id;
} Vizinho;

typedef struct REQUISICAO {
    clock_t t;
    bool done;
    Message *msg;
} Requisicao;

class Peer {
private:
    Parser parser;
    MessageFactory msgFct;
    unsigned long id;
    std::string ip;
    std::string porta;
    int sockfd;
    Vizinho next, prev;
    std::map<unsigned int, std::string> tuplas;
    std::map<unsigned int, Requisicao*> reqs;
    std::map<unsigned int, Vizinho> membros;
public:
    Peer(std::string addr, std::string port);
    ~Peer();

    void start();
    void serve();
    void pconnect(Vizinho &peer);
    int pconnect(std::string addr, std::string port);
    void psend(int connfd, Message *msg);
    Message *receive(int connfd);
    void parse(std::string cmd);
    void processa(Message *msg);

    unsigned long getId() const;
    std::string getIp() const;
    std::string getPorta() const;
    Vizinho getNext() const;
    Vizinho getPrev() const;

    static unsigned long hash(const std::string &s);
    static void* processa(void *con);
    static void* serve(void *arg);
};

#endif