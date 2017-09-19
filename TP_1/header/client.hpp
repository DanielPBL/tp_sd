#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <message.hpp>
#include <server.hpp>
#include <map>
#include <string>
#include <netinet/in.h>

class Client {
private:
    std::string ip;
    int sockfd;
    struct sockaddr_in sin;

    void init(std::string cip, std::string udp);
    static void* listener(void *arg);
public:
    ServerInfo si;

    Client(std::string cip, std::string udp);
    Client(std::string cip, std::string udp, std::string sip, std::string port);
    std::string getIp() const;
    int getSockfd() const;
    void cconnect(std::string sip, std::string porta);
    void csend(Message &msg);
    Message* receive();
    bool query(std::string cmd);
    void listen();
};

#endif /* CLIENT_HPP */
