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
#include <sstream>
#include <pthread.h>

#define BUF_SIZE 500

using namespace std;

Server::Server(const char* ip, const char* port, const char* bc_port) {
    struct addrinfo *servinfo, *p;
    struct addrinfo hints;
    struct sigaction sa;
    struct sockaddr_in sin, cliAddr;
    struct hostent *h;
    socklen_t len = sizeof (sin);
    int rv;
    int yes = 1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((h = gethostbyname("255.255.255.255")) == NULL) {
        perror("Erro ao obter host de broadcast");
        throw "Error gethostbyname";
    }

    si.remoteServAddr.sin_family = h->h_addrtype;
    memcpy((char *) &si.remoteServAddr.sin_addr.s_addr,
            h->h_addr_list[0], h->h_length);
    si.remoteServAddr.sin_port = htons(atoi(bc_port));

    if ((this->si.socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Erro ao criar socket de broadcast");
        throw "Erro socket";
    }

    if (setsockopt(this->si.socket, SOL_SOCKET, SO_BROADCAST, &yes, sizeof yes) == -1) {
        perror("Erro ao configurar socket de broadcast");
        throw "Erro setsockopt";
    }


    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    cliAddr.sin_port = htons(0);

    if ((rv = bind(this->si.socket, (struct sockaddr *) &cliAddr, sizeof (cliAddr))) == -1) {
        throw "Erro ao vincular socket";
    }

    if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
        throw gai_strerror(rv);
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((this->sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("Erro ao criar socket");
            continue;
        }

        if (setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
            throw "Erro ao configurar socket";
        }

        if (bind(this->sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(this->sockfd);
            perror("Erro ao vincular socket");
            continue;
        }

        break;
    }

    if (p == NULL) {
        throw "Erro ao vincular socket";
    }

    freeaddrinfo(servinfo);

    if (getsockname(this->sockfd, (struct sockaddr *) &sin, &len) == -1) {
        throw "Erro ao obter informações do socket";
    }

    strncpy(this->si.ip, ip, sizeof this->si.ip);
    this->si.port = ntohs(sin.sin_port);
    this->si.bc_port = atoi(bc_port);

    if (listen(sockfd, SOMAXCONN) == -1) {
        throw "Erro ao escutar conexões";
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        throw "Erro ao configurar sinalização";
    }
}

void Server::broadcast() {
    pthread_t thread;
    int rc;
    // Cria thread de broadcast
    if ((rc = pthread_create(&thread, NULL, Server::broadcaster, &this->si)) != 0) {
        throw "Erro ao criar broadcaster";
    }
}

void* Server::broadcaster(void *arg) {
    ServerInfo *si = (ServerInfo *) arg;
    Message msg;
    string str;
    int numBytes;

    // Cria mensagem de PING
    msg.setType(Message::PING);
    msg.setAddr(string(si->ip)); // IP do servidor
    msg.setPort(SSTR(si->port)); // Porta TCP do servidor
    str = msg.toString();

    // Envia uma mensagem de broadcast a cada 5 segundos
    while (true) {
        numBytes = sendto(si->socket, str.c_str(), str.size(), 0,
                (struct sockaddr *) &si->remoteServAddr, sizeof si->remoteServAddr);

        if (numBytes == -1) {
            perror("Erro ao enviar mensagem UDP");
            pthread_exit(NULL);
        }

        sleep(5);
    }

    return NULL;
}

// Fecha sockets caso estejam abertos
Server::~Server() {
    close(this->sockfd);
    close(this->connfd);
}

void Server::saccept() {
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    char s[INET6_ADDRSTRLEN];

    // Espera um cliente conectar
    sin_size = sizeof their_addr;
    this->connfd = accept(this->sockfd, (struct sockaddr *) &their_addr, &sin_size);

    if (this->connfd == -1) {
        throw "Erro ao aceitar conexão";
    }

    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *) &their_addr), s, sizeof s);

    cout << "Conexão recebida de " << s << endl;
}

Message* Server::receive() {
    char buf[BUF_SIZE];
    stringstream ss;
    int numBytes, size;

    if ((numBytes = recv(this->connfd, buf, BUF_SIZE - 1, 0)) == -1) {
        throw "Erro ao receber mensagem";
    }
    buf[numBytes] = '\0';
    ss << buf;

    /* Obtem informações da mensagem. Caso estaja incompleta, buffet < tamanho
       continua recebendo até completar a mensagem.
    */
    Message aux(buf);
    size = aux.getText().size();
    while (aux.getSize() > size) {
        if ((numBytes = recv(this->connfd, buf, BUF_SIZE - 1, 0)) == -1) {
            throw "Erro ao receber mensagem";
        }
        buf[numBytes] = '\0';
        ss << buf;
        size += numBytes;
    }

    return new Message(ss.str());
}

void Server::ssend(Message &msg) {
    string str = msg.toString();

    // Envia uma mensagem pelo socket TCP
    if (send(this->connfd, str.c_str(), str.size(), 0) == -1) {
        throw "Erro ao enviar mensagem";
    }
}

void sigchld_handler(int s) {
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while (waitpid(-1, &s, WNOHANG) > 0);

    errno = saved_errno;
}

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*) sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*) sa)->sin6_addr);
}
