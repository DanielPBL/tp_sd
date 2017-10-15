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

#define BUF_SIZE 500
#define BACKLOG 10
#define TIMEOUT 300
#define MAXNODES 256

using namespace std;

unsigned long Peer::hash(const std::string &s) {
    unsigned long h = 0;
    char *str = (char*) s.c_str();

    while (*str) {
        h = h << 1 ^ *str++;
    }

    return (h % MAXNODES) + 1;
}

Peer::Peer(std::string addr, std::string port) {
    struct addrinfo hints, *servinfo, *p;
    int yes = 1;
    int rv;

    this->id = Peer::hash(addr + ":" + port);
    this->ip = addr;
    this->porta = port;

    this->next.id = this->id;
    this->next.ip = addr;
    this->next.porta = port;

    this->prev.id = this->id;
    this->prev.ip = addr;
    this->prev.porta = port;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, port.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        exit(1);
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((this->sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(this->sockfd, p->ai_addr, p->ai_addrlen) == -1) {
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

    if (listen(this->sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
}

Peer::~Peer() {
    close(this->sockfd);
}

void Peer::serve() {
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    int connfd;
    pthread_t thread;
    Conexao *con;

    while (true) {
        // Espera um cliente conectar
        sin_size = sizeof their_addr;
        connfd = accept(this->sockfd, (struct sockaddr *) &their_addr, &sin_size);

        if (connfd == -1) {
            throw "Erro ao aceitar conexão";
        }

        con = new Conexao;
        con->connfd = connfd;
        con->self = this;
        if (pthread_create(&thread, NULL, Peer::processa, con) != 0) {
            throw "Erro ao criar thread";
        }
    }
}

int Peer::pconnect(std::string addr, std::string port) {
    int connfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(addr.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
        throw gai_strerror(rv);
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((connfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(connfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(connfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        throw "Falha ao conectar";
    }

    return connfd;
}

void Peer::pconnect(Vizinho &peer) {
    peer.sockfd = this->pconnect(peer.ip, peer.porta);
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

void Peer::processa(int connfd, Message *msg) {
    switch (msg->getType()) {
        case Message::MSG_ENTER: {
            Message *resp;
            bool zero_entre = (this->next.id < this->id);
            Vizinho peer = {
                .ip = msg->getAddr(),
                .porta = msg->getPort(),
                .sockfd = 0,
                .id = Peer::hash(msg->getAddr() + ":" + msg->getPort())
            };

            if (this->id == this->next.id) {
                // Rede vazia
                this->next.id = peer.id;
                this->next.ip = peer.ip;
                this->next.porta = peer.porta;

                resp = this->msgFct.newMessage();
                resp->setAddr(this->ip);
                resp->setPort(this->porta);
                resp->setType(Message::MSG_RESP);
                resp->setToId(msg->getId());
                resp->setText("Addr: " + this->ip + "\n"
                            + "Port: " + this->porta + "\n"
                            + "Addr: " + this->ip + "\n"
                            + "Port: " + this->porta);
                this->pconnect(peer);
            } else if (((peer.id > this->id) || (zero_entre && this->id > peer.id))
                    && peer.id < this->next.id) {
                // Inserir entre
                resp = this->msgFct.newMessage();
                resp->setAddr(this->ip);
                resp->setPort(this->porta);
                resp->setType(Message::MSG_RESP);
                resp->setToId(msg->getId());
                resp->setText("Addr: " + this->next.ip + "\n"
                            + "Port: " + this->next.porta + "\n"
                            + "Addr: " + this->ip + "\n"
                            + "Port: " + this->porta);
                this->pconnect(peer);

                this->next.id = peer.id;
                this->next.ip = peer.ip;
                this->next.porta = peer.porta;
            } else {
                // Passa a requisição pro próximo
                peer = this->next;
                this->pconnect(peer);
                resp = msg;
            }

            this->psend(peer.sockfd, resp);
            close(peer.sockfd);
        }
            break;
        case Message::MSG_FIND: {
            FindCmd *find = (FindCmd*) this->parser.parse(msg->getText());
            bool zero_entre = (this->next.id < this->id);
            Message *resp;
            Vizinho peer = {
                .ip = msg->getAddr(),
                .porta = msg->getPort(),
                .sockfd = zero_entre //DEBUG WARNING
            };
            // Procura tupla em sua coleção, caso encontre, respode com a tupla,
            // caso contrário repassa a requisição
            if (this->tuplas.find(find->K) != this->tuplas.end()) {
                peer.sockfd = this->pconnect(msg->getAddr(), msg->getPort());
                resp = this->msgFct.newMessage();
                resp->setAddr(this->ip);
                resp->setPort(this->porta);
                resp->setType(Message::MSG_RESP);
                resp->setToId(msg->getId());
                resp->setText("<" + SSTR(find->K) + ", \"" + this->tuplas[find->K] + "\">\n");
            } else {
                // Se a própria requisição retornou ao peer, a chave não
                // foi encontrada na rede
                if (this->ip == msg->getAddr() && this->porta == msg->getPort() &&
                    this->reqs.find(msg->getId()) != this->reqs.end()) {
                    peer.sockfd = this->pconnect(msg->getAddr(), msg->getPort());
                    resp = this->msgFct.newMessage();
                    resp->setAddr(this->ip);
                    resp->setPort(this->porta);
                    resp->setType(Message::MSG_ERROR);
                    resp->setToId(msg->getId());
                } else {
                    peer = this->next;
                    this->pconnect(peer);
                    resp = msg;
                }
            }

            this->psend(peer.sockfd, resp);
            close(peer.sockfd);
        }
            break;
        case Message::MSG_STORE: {
            StoreCmd *store = (StoreCmd*) this->parser.parse(msg->getText());
            this->tuplas.insert(store->tupla);
        }
            break;
        case Message::MSG_RESP:
            if (this->reqs.find(msg->getToId()) != this->reqs.end()) {
                Message *r_msg = this->reqs[msg->getToId()]->msg;
                switch (r_msg->getType()) {
                    case Message::MSG_ENTER: {
                        stringstream ss;
                        string str;

                        ss.str(msg->getText());
                        getline(ss, str, '\n');
                        this->next.ip = str.substr(6);
                        getline(ss, str, '\n');
                        this->next.porta = str.substr(6);

                        getline(ss, str, '\n');
                        this->prev.ip = str.substr(6);
                        getline(ss, str, '\n');
                        this->prev.porta = str.substr(6);

                        this->next.id = Peer::hash(this->next.ip + ":" + this->next.porta);
                        this->prev.id = Peer::hash(this->prev.ip + ":" + this->prev.porta);

                        this->reqs[msg->getToId()]->done = true;

                        Message *ack = this->msgFct.newMessage();
                        ack->setType(Message::MSG_ACK);
                        ack->setAddr(this->ip);
                        ack->setPort(this->porta);
                        this->pconnect(this->next);
                        this->psend(this->next.sockfd, ack);
                        close(this->next.sockfd);
                    }
                        break;
                    case Message::MSG_FIND: {
                        StoreCmd *store = (StoreCmd*) this->parser.parse("STORE(" + msg->getText() + ")");

                        cout << msg->getAddr() << ":" << msg->getPort();
                        cout << " - " << store->tupla.second << endl;

                        this->reqs[msg->getToId()]->done = true;
                    }
                        break;
                    default:
                        throw "Resposta não entendida";
                }
            } else {
                throw "Resposta não esperada";
            }
            break;
        case Message::MSG_ERROR:
            if (this->ip == msg->getAddr() && this->porta == msg->getPort() &&
                this->reqs.find(msg->getToId()) != this->reqs.end()) {
                    if (this->reqs[msg->getToId()]->msg->getType() == Message::MSG_FIND) {
                        // Tupla não encontrada
                        cout << "Chave não encontrada na rede." << endl;
                        this->reqs[msg->getToId()]->done = true;
                    }
                }
        case Message::MSG_ACK:
            this->prev.ip = msg->getAddr();
            this->prev.porta = msg->getPort();
            this->prev.id = Peer::hash(this->prev.ip + ":" + this->prev.porta);

            // Enviar para o peer as tuplas que pertencem ao seu intervalo de id

            break;
        default:
            cout << "Mensagem sem tratamento" << endl;
            cout << msg->toString() << endl;
            break;
    }
}

void* Peer::processa(void *con) {
    Conexao *conexao = (Conexao*) con;
    Peer *p = (Peer*) conexao->self;
    Message *msg;

    msg = p->receive(conexao->connfd);
    p->processa(conexao->connfd, msg);
    close(conexao->connfd);

    delete conexao;
    delete msg;
    pthread_exit(NULL);
}

void* Peer::serve(void *arg) {
    Peer *peer = (Peer*) arg;
    peer->serve();
    pthread_exit(NULL);
}

void Peer::parse(string cmd) {
    Comando *comando = this->parser.parse(cmd);

    switch (comando->tipo) {
        case Comando::CMD_ENTER: {
            EnterCmd *enter = (EnterCmd*) comando;
            Message *msg = this->msgFct.newMessage();
            Requisicao *req = new Requisicao;
            int connfd;

            req->done = false;
            req->msg = msg;
            this->reqs[msg->getId()] = req;

            msg->setAddr(this->ip);
            msg->setPort(this->porta);
            msg->setType(Message::MSG_ENTER);

            connfd = this->pconnect(enter->ip, enter->porta);
            this->psend(connfd, msg);
            close(this->next.sockfd);

            // Espera TIMEOUT segundos ou até receber a resposta
            req->t = clock();
            clock_t t = (clock() - req->t) / CLOCKS_PER_SEC;
            while (t < TIMEOUT && !req->done) {
                t = (clock() - req->t) / CLOCKS_PER_SEC;
            }

            if (!req->done) {
                cerr << "Tempo limite excedido" << endl;
            } else {
                t = clock() - req->t;
                cout << "Requisição concluída em " << ((float) t) / CLOCKS_PER_SEC << " segundos." << endl;
            }

            this->reqs.erase(msg->getToId());
            delete req;
            delete msg;
        }
            break;
        case Comando::CMD_FIND: {
            FindCmd *find = (FindCmd*) comando;
            Message *msg = this->msgFct.newMessage();
            Requisicao *req = new Requisicao;

            req->done = false;
            req->msg = msg;
            this->reqs[msg->getId()] = req;

            msg->setAddr(this->ip);
            msg->setPort(this->porta);
            msg->setType(Message::MSG_FIND);
            msg->setText("FIND(" + SSTR(find->K) + ")\n");

            this->pconnect(this->next);
            this->psend(this->next.sockfd, msg);
            close(this->next.sockfd);

            // Espera TIMEOUT segundos ou até receber a resposta
            req->t = clock();
            clock_t t = (clock() - req->t) / CLOCKS_PER_SEC;
            while (t < TIMEOUT && !req->done) {
                t = (clock() - req->t) / CLOCKS_PER_SEC;
            }

            if (!req->done) {
                cerr << "Tempo limite excedido" << endl;
            } else {
                t = clock() - req->t;
                cout << "Requisição concluída em " << ((float) t) / CLOCKS_PER_SEC << " segundos." << endl;
            }

            this->reqs.erase(msg->getToId());
            delete req;
            delete msg;
        }
            break;
        case Comando::CMD_LIST:
            cout << "Vizinhos: " << endl;
            cout << "Próximo: " << this->next.ip << ":" << this->next.porta  << "#" << this->next.id << endl;
            cout << "Anterior: " << this->prev.ip << ":" << this->prev.porta  << "#" << this->prev.id << endl;
            cout << "Tuplas Armazenadas:" << endl;
            for (map<int, string>::iterator it = this->tuplas.begin(); it != this->tuplas.end(); ++it) {
                cout << "<" << it->first << ", \"" << it->second << "\">" << endl;
            }
            break;
        case Comando::CMD_STORE: {
            StoreCmd *store = (StoreCmd*) comando;
            Message *msg = this->msgFct.newMessage();

            this->tuplas.insert(store->tupla);
            // Redundância
            msg->setAddr(this->ip);
            msg->setPort(this->porta);
            msg->setType(Message::MSG_STORE);
            msg->setText("STORE(<" + SSTR(store->tupla.first) + ",\"" + store->tupla.second + "\">)\n");

            this->pconnect(this->next);
            this->psend(this->next.sockfd, msg);
            close(this->next.sockfd);
        }
            break;
        case Comando::CMD_QUIT:
            close(this->sockfd);
            cout << "Adeus!" << endl;
            exit(EXIT_SUCCESS);
            break;
        default:
            comando->executar();
    }

    delete comando;
}
