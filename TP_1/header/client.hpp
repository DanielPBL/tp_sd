#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <message.hpp>
#include <server.hpp>
#include <map>
#include <string>

class Client {
private:
    std::string ip;

    void csend(int socket, Message &msg);
    Message* receive(int socket);
    static void* listener(void *arg);
public:
    ServerInfo si;

    Client(std::string cip, std::string udp);
    Client(std::string cip, std::string udp, std::string sip, std::string port);
    void init(std::string cip, std::string udp);
    void query(std::string cmd);
    void listen();
};

#endif /* CLIENT_HPP */

