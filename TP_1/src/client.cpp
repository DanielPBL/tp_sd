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
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sstream>
#include <iostream>
#include <vector>
#include <time.h>

#define BUF_SIZE 500
#define TIMEOUT  300

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
    if ((rc = bind(this->si.socket, (struct sockaddr *) &servAddr, sizeof (servAddr))) == -1) {
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
    //cout << "Criando thread de listen..." << endl;
    if ((rc = pthread_create(&thread, NULL, Client::listener, &this->si)) != 0) {
        throw "Erro ao criar broadcaster";
    }
}

void* Client::listener(void* arg) {
    ServerInfo *si = (ServerInfo *) arg;
    Message *msg;
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
    struct timeval timeout;
    struct sockaddr_in sin;
    socklen_t len = sizeof (sin);
    int rv;
    clock_t t;
    int *pids, status;
    unsigned int i;
    vector<string> ips;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    memset(&timeout, 0, sizeof timeout);
    timeout.tv_sec = TIMEOUT;
    timeout.tv_usec = 0;

    t = clock();
    if (this->si.sl.empty()) {
        cout << "Aguardando algum servidor..." << endl;
        while (this->si.sl.empty() && ((clock() - t) / CLOCKS_PER_SEC) < TIMEOUT);
    }

    if (this->si.sl.empty()) {
        cout << "Tempo limite atingido, tente novamente mais tarde." << endl;
        return;
    }

    pids = new int[this->si.sl.size()];
    ips.resize(this->si.sl.size());
    i = 0;

    t = clock();
    for (std::map<string, string>::iterator it = this->si.sl.begin(); it != this->si.sl.end(); ++it) {
        ips[i] = it->first;
        pids[i] = fork();

        if (pids[i] == -1) {
            cerr << "Erro ao criar processo filho" << endl;
            exit(1);
        }

        if (pids[i++] == 0) {
            if ((rv = getaddrinfo(it->first.c_str(), it->second.c_str(), &hints, &servinfo)) != 0) {
                perror(gai_strerror(rv));
                exit(1);
            }

            for (p = servinfo; p != NULL; p = p->ai_next) {
                if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
                    perror("Erro ao criar socket");
                    continue;
                }

                if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof timeout) == -1) {
                    perror("Erro ao configurar socket");
                    continue;
                }

                if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof timeout) == -1) {
                    perror("Erro ao configurar socket");
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
                cerr << "Erro ao conectar ao servidor " << it->first << ":" << it->second << endl;
                exit(1);
            }

            if (getsockname(sockfd, (struct sockaddr *) &sin, &len) == -1) {
                cerr << "Erro ao obter informações do socket" << endl;
                exit(1);
            }

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
            exit(0);
        }
    }

    for (i = 0; i < this->si.sl.size(); ++i) {
        if (waitpid(pids[i], &status, 0) < 0) {
            cerr << "Erro ao obter status do processo filho" << endl;
            exit(1);
        }

        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) != 0) {
                this->si.sl.erase(this->si.sl.find(ips[i]));
            }
        }

        if (WIFSIGNALED(status)) {
            this->si.sl.erase(this->si.sl.find(ips[i]));
        }
    }

    delete pids;

    t = clock() - t;
    cout << "Tarefa concluída em " << ((float) t) / CLOCKS_PER_SEC << " segundos." << endl;
}
