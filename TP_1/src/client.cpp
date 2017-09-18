#include <client.hpp>
#include <server.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sstream>
#include <iostream>
#include <vector>

#define BUF_SIZE 500

using namespace std;

void Client::init(string cip, string udp) {
    struct addrinfo hints;
    struct sockaddr_in servAddr;
    int yes = 1, rc;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((this->si.socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Erro ao criar socket de broadcast");
        throw "Erro socket";
    }

    if (setsockopt(this->si.socket, SOL_SOCKET, SO_BROADCAST, &yes, sizeof yes) == -1) {
        perror("Erro ao configurar socket de broadcast");
        throw "Erro setsockopt";
    }
    
    this->ip = string(cip);
    this->si.bc_port = atoi(udp.c_str());

    /* bind local server port */
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(this->si.bc_port);
    if ((rc = bind(this->si.socket, (struct sockaddr *) &servAddr, sizeof (servAddr))) == -1 ) {
        throw "Erro ao vincular porta";
    }
    
}

Client::Client(string cip, string udp) {
    this->init(cip, udp);
}

Client::Client(string cip, string udp, string sip, string port) {
    this->init(cip, udp);
    this->si.sl[sip] = port;
}

void Client::listen() {
    pthread_t thread;
    int rc;
    cout << "Criando thread de listen..." << endl;
    if ((rc = pthread_create(&thread, NULL, Client::listener, &this->si)) != 0) {
        throw "Erro ao criar broadcaster";
    }
    //Client::listener((void *)&this->si);
}

void* Client::listener(void* arg) {
    ServerInfo *si = (ServerInfo *) arg;
    Message *msg;
    string str;
    struct sockaddr_in their_addr;
    int numBytes;
    char buf[BUF_SIZE];
    map<string, string>::iterator it;
    socklen_t their_len = sizeof their_addr;

    while (true) {
        numBytes = recvfrom(si->socket, buf, BUF_SIZE - 1, 0,
                (struct sockaddr *) &their_addr, &their_len);

        if (numBytes == -1) {
            perror("Erro ao receber mensagem UDP");
            pthread_exit(NULL);
        }

        buf[numBytes] = '\0';
        msg = new Message(buf);

        if (msg->getType() == Message::PING) {
            si->sl.insert(pair<string, string>(msg->getAddr(), msg->getPort()));
        }
    }

    return NULL;
}

void Client::csend(int socket, Message &msg) {
    string str = msg.toString();

    if (send(socket, str.c_str(), str.size(), 0) == -1) {
        throw "Erro ao enviar mensagem";
    }
}

Message* Client::receive(int sockfd) {
    char buf[BUF_SIZE];
    stringstream ss;
    int numBytes, size;

    if ((numBytes = recv(sockfd, buf, BUF_SIZE - 1, 0)) == -1) {
        throw "Erro ao receber mensagem";
    }
    buf[numBytes] = '\0';
    ss << buf;

    Message aux(buf);
    size = aux.getText().size();
    while (aux.getSize() > size) {
        if ((numBytes = recv(sockfd, buf, BUF_SIZE - 1, 0)) == -1) {
            throw "Erro ao receber mensagem";
        }
        buf[numBytes] = '\0';
        ss << buf;
        size += numBytes;
    }

    return new Message(ss.str());
}

void Client::query(string cmd) {
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_in sin;
    socklen_t len = sizeof (sin);
    int rv;
    char s[INET6_ADDRSTRLEN], c[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (this->si.sl.empty()) {
        cout << "Aguardando algum servidor..." << endl;
        while (this->si.sl.empty());
    }

    for (std::map<string, string>::iterator it = this->si.sl.begin(); it != this->si.sl.end(); ++it) {
        if ((rv = getaddrinfo(it->first.c_str(), it->second.c_str(), &hints, &servinfo)) != 0)
            throw gai_strerror(rv);

        for (p = servinfo; p != NULL; p = p->ai_next) {
            if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                perror("Erro ao criar socket");
                continue;
            }

            if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                close(sockfd);
                perror("Erro ao conectar ao servidor");
                continue;
            }

            break;
        }

        if (p == NULL) {
            cerr << "Erro ao conectar ao servidor" << endl;
        }


        if (getsockname(sockfd, (struct sockaddr *) &sin, &len) == -1) {
            cerr << "Erro ao obter informações do socket" << endl;
        }

        inet_ntop(sin.sin_family, (void*) &sin.sin_addr, c, sizeof c);
        inet_ntop(p->ai_family, get_in_addr((struct sockaddr *) p->ai_addr), s, sizeof s);

        freeaddrinfo(servinfo);

        Message msg, *resp;
        string str;

        msg.setType(Message::QUERY);
        msg.setAddr(this->ip);
        msg.setPort(SSTR(ntohs(sin.sin_port)));
        msg.setText(cmd);

        this->csend(sockfd, msg);
        resp = this->receive(sockfd);
        cout << "Servidor - " << resp->getAddr() << ":" << resp->getPort() << endl;

        if (resp->getType() == Message::RESPONSE) {
            cout << resp->getText() << endl;
        } else {
            cout << Message::TypeDesc(resp->getType());
        }

        delete resp;
    }
}