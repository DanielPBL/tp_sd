#include <peer.hpp>
#include <message.hpp>
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
#include <signal.h>

#define BUF_SIZE 500
#define BACKLOG 10

using namespace std;

Peer::Peer(std::string addr, std::string port) {
    struct addrinfo hints, *servinfo, *p;
    int yes=1;
    int rv;

    this->ip = addr;
    this->porta = port;

    this->next.ip = addr;
    this->next.porta = port;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, port.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
}

Peer::~Peer() {
    close(this->sockfd);
    kill(this->cpid, SIGTERM);
}

void Peer::serve() {
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    int connfd;
    pthread_t thread;
    Conexao *con = new Conexao;

    while (true) {
        // Espera um cliente conectar
        sin_size = sizeof their_addr;
        connfd = accept(this->sockfd, (struct sockaddr *) &their_addr, &sin_size);

        if (connfd == -1) {
            throw "Erro ao aceitar conexão";
        }

        con->connfd = connfd;
        con->self = this;
        if (pthread_create(&thread, NULL, Peer::respond, con) != 0) {
            throw "Erro ao criar thread";
        }
    }
}

void Peer::psend(int connfd, Message *msg) {
    string str = msg->toString();

    // Envia uma mensagem pelo socket TCP
    if (send(connfd, str.c_str(), str.size(), 0) == -1) {
        throw "Erro ao enviar mensagem";
    }
}

Message* Peer::receive(int connfd) {
    char buf[BUF_SIZE];
    stringstream ss;
    int numBytes, size;

    if ((numBytes = recv(connfd, buf, BUF_SIZE - 1, 0)) == -1) {
        throw "Erro ao receber mensagem";
    }
    buf[numBytes] = '\0';
    ss << buf;

    /* Obtem informações da mensagem. Caso estaja incompleta, buffer < tamanho
       continua recebendo até completar a mensagem.
    */
    Message* aux = this->msgFct.parseMessage(buf);
    size = aux->getText().size();
    while (aux->getSize() > size) {
        if ((numBytes = recv(connfd, buf, BUF_SIZE - 1, 0)) == -1) {
            throw "Erro ao receber mensagem";
        }
        buf[numBytes] = '\0';
        ss << buf;
        size += numBytes;
    }

    delete aux;
    return this->msgFct.parseMessage(ss.str());
}

void Peer::respond(int connfd, Message *msg) {

}

void* Peer::respond(void *con) {
    Conexao *conexao = (Conexao*) con;
    Peer *p = (Peer*) conexao->self;
    Message *msg;

    msg = p->receive(conexao->connfd);
    p->respond(conexao->connfd, msg);
    close(conexao->connfd);

    delete conexao;
    delete msg;
    pthread_exit(NULL);
}

void Peer::parse(string cmd) {
    Comando *comando = this->parser.parse(cmd);

    switch (comando->tipo) {
        case Comando::CMD_LIST:
            cout << "Tuplas Armazenadas:" << endl;
            for (map<int, string>::iterator it = this->tuplas.begin(); it != this->tuplas.end(); ++it) {
                cout << "<" << it->first << ", \"" << it->second << "\">" << endl;
            }
            break;
        case Comando::CMD_STORE: {
            StoreCmd *store = (StoreCmd*) comando;
            this->tuplas.insert(store->tupla);
        }
            break;
        case Comando::CMD_QUIT:
            cout << "Adeus!" << endl;
            close(this->sockfd);
            kill(this->cpid, SIGTERM);
            exit(EXIT_SUCCESS);
            break;
        default:
            comando->executar();
    }

    delete comando;
}
