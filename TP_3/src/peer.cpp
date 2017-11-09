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
#include <list>

#define BUF_SIZE 500
#define BACKLOG 10
#define TIMEOUT 300
#define MAXNODES 1024

using namespace std;

/** Método estático para calcular o hash de uma string
* @param s string& - Referência para a string que será sofre hash
* @return unsigned long - Hash calculado
*/
unsigned long Peer::hash(const string &s) {
    unsigned long h = 0;
    char *str = (char*) s.c_str();

    while (*str) {
        h = h << 1 ^ *str++;
    }

    return (h % MAXNODES);
}

/** Construtor da classe Peer
* @param addr string - IP do peer
* @param port string - PORTA do peer
*/
Peer::Peer(string addr, string port) {
    struct addrinfo hints, *servinfo, *p;
    int yes = 1;
    int rv;

    // Inicialmente o peer aponta para ele mesmo em ambas direções.
    this->id = Peer::hash(addr + ":" + port);
    this->ip = addr;
    this->porta = port;

    this->next.id = this->id;
    this->next.ip = addr;
    this->next.porta = port;

    this->prev.id = this->id;
    this->prev.ip = addr;
    this->prev.porta = port;

    // Adiciona o próprio peer a lista de membros
    this->membros[this->id].id = this->id;
    this->membros[this->id].ip = this->ip;
    this->membros[this->id].porta = this->porta;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, port.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "Erro ao obter informações do host: %s\n", gai_strerror(rv));
        exit(1);
    }

    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((this->sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("Erro ao criar socket");
            continue;
        }

        if (setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("Erro ao setar opções do socket");
            exit(1);
        }

        if (bind(this->sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("Erro ao vincular o socket");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo);

    if (p == NULL)  {
        fprintf(stderr, "Não foi possível vincular o socket\n");
        exit(1);
    }

    if (listen(this->sockfd, BACKLOG) == -1) {
        perror("Erro ao escutar conexões");
        exit(1);
    }
}

/** Método getter do Id
* @return unsigned long
*/
unsigned long Peer::getId() const {
    return this->id;
}

/** Método getter do IP
* @return string
*/
string Peer::getIp() const {
    return this->ip;
}

/** Método getter da porta
* @return string
*/
string Peer::getPorta() const {
    return this->porta;
}

/** Método getter do peer apontado como próximo
* @return Vizinho
*/
Vizinho Peer::getNext() const {
    return this->next;
}

/** Método getter do peer apontado como anterior
* @return Vizinho
*/
Vizinho Peer::getPrev() const {
    return this->prev;
}

/** Destrutor, fecha o socket do servidor */
Peer::~Peer() {
    close(this->sockfd);
}

/** Método que cria a thread do servidor */
void Peer::start() {
    pthread_t t_id;

    if (pthread_create(&t_id, NULL, Peer::serve, this) != 0) {
        throw "Erro ao criar thread do servidor";
    }
}

/** Método que Implementa um servidor concorrente
* Para cada conexão cria uma tread para atender a requisição
*/
void Peer::serve() {
    struct sockaddr_storage their_addr;
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

/** Método para se conectar a um IP e PORTA
* @param addr string - IP para requisitar conexão
* @param port string - PORTA para requisitar conexão
* @return int - FD para o socket da conexão
*/
int Peer::pconnect(string addr, string port) {
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
            perror("Erro ao criar socket");
            continue;
        }

        if (connect(connfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(connfd);
            perror("Erro ao conectar no servidor");
            continue;
        }

        break;
    }

    if (p == NULL) {
        throw "Falha ao conectar";
    }

    return connfd;
}

/** Método para se conectar a um peer
* @param peer Vizinho& - Referência para o peer que deseja conexão
*/
void Peer::pconnect(Vizinho &peer) {
    peer.sockfd = this->pconnect(peer.ip, peer.porta);
}

/** Método para enviar uma mensagem para uma dada conexão
* @param connfd int - Socket para a conexão recebida
* @param msg Message* - Ponteiro para a mensagem recebida
*/
void Peer::psend(int connfd, Message *msg) {
    string str = msg->toString();

    // Envia uma mensagem pelo socket TCP
    if (send(connfd, str.c_str(), str.size(), 0) == -1) {
        throw "Erro ao enviar mensagem";
    }
}

/** Método que recebe uma mensagem de uma conexão informada
* @param connfd int - Socket para a conexão recebida
* @return Message* - Ponteiro para a mensagem recebida
*/
Message* Peer::receive(int connfd) {
    char buf[BUF_SIZE];
    stringstream ss;
    int numBytes, size;

    if ((numBytes = recv(connfd, buf, BUF_SIZE - 1, 0)) == -1) {
        throw "Erro ao receber mensagem";
    }
    buf[numBytes] = '\0';
    ss << buf;

    /* Obtem informações da mensagem. Caso esteja incompleta, buffer < tamanho
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

/** Método que trata as requisições que chegam ao servidor
* @param msg Message* - Ponteiro para a mensagem que deve ser atendida
*/
void Peer::processa(Message *msg) {
    switch (msg->getType()) {
        // Requisição para entrar na rede
        case Message::MSG_ENTER: {
            Message *resp;
            bool zero_entre = (this->next.id < this->id);
            Vizinho peer = {
                .ip = msg->getAddr(),
                .porta = msg->getPort(),
                .sockfd = 0,
                .id = Peer::hash(msg->getAddr() + ":" + msg->getPort())
            };

            if (this->id == this->next.id) { // Rede vazia
                // Aponta para o novo peer
                this->next.id = peer.id;
                this->next.ip = peer.ip;
                this->next.porta = peer.porta;

                // Monta uma mensagem informando que o novo peer deve apontar para
                // o peer que respondeu a requisição
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
            } else if ((peer.id > this->id && peer.id < this->next.id)
                    || (zero_entre && ((peer.id > this->id && peer.id > this->next.id)
                    || (peer.id < this->id && peer.id < this->next.id)))) { // Inseir o novo peer entre o peer atual e o próximo
                // Monta uma mensagem informando ao novo peer que seu próximo é o próximo do
                // peer atual e seu anterior é o peer atual
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

                // O próximo do peer atual se torna o novo peer
                this->next.id = peer.id;
                this->next.ip = peer.ip;
                this->next.porta = peer.porta;
            } else { // Não entra depois do peer do atual. Delega.
                // Passa a requisição pro próximo
                peer = this->next;
                this->pconnect(peer);
                resp = msg;
            }
            // Envia a mensagem para o peer que requisitou entrada ou pra quem delegou
            this->psend(peer.sockfd, resp);
            close(peer.sockfd);
        }
            break;
        // Mensagem para a sincronização das listas de membros
        case Message::MSG_SYNC: {
            cout << msg->getAddr() << endl;
            cout << msg->getPort() << endl;
            cout << msg->getText();
            unsigned int memberId, msgPId = Peer::hash(msg->getAddr() + ":" + msg->getPort());
            EnterCmd *enter;
            list<Comando*> enterCmds = this->parser.parseEnters(msg->getText());
            string members;

            for (list<Comando*>::iterator it = enterCmds.begin(); it != enterCmds.end(); ++it) {
                enter = (EnterCmd*) *it;
                memberId = Peer::hash(enter->ip + ":" + enter->porta);
                this->membros[memberId].id = memberId;
                this->membros[memberId].ip = enter->ip;
                this->membros[memberId].porta = enter->porta;
            }

            // Se a mensagem for do peer atual, é sinal que a mensagem deu a volta no
            // anel e sua propagração deve ser interrompida
            if (this->id != msgPId) {
                for (map<unsigned int, Vizinho>::iterator it = this->membros.begin(); it != this->membros.end(); ++it) {
                    members += "ENTER \"" + it->second.ip + "\" " + SSTR(it->second.porta) + "\n";
                }
                msg->setText(members);

                this->pconnect(this->next);
                this->psend(this->next.sockfd, msg);
                close(this->next.sockfd);
            }
        }
            break;
        // Requisição de busca
        case Message::MSG_FIND: {
            FindCmd *find = (FindCmd*) this->parser.parse(msg->getText());
            Message *resp;
            Vizinho peer = {
                .ip = msg->getAddr(),
                .porta = msg->getPort(),
                .sockfd = 0,
                .id = Peer::hash(msg->getAddr() + ":" + msg->getPort())
            };
            // Flag que indica se o id 0 da rede está entre o peer atual e o próximo
            int zero_entre = (this->prev.id > this->id);

            // Procura tupla em sua coleção, caso encontre, respode com a tupla,
            // caso contrário, delega para o próximo peer
            if (this->tuplas.find(find->K) != this->tuplas.end()) { // Possuo a chave
                peer.sockfd = this->pconnect(msg->getAddr(), msg->getPort());
                resp = this->msgFct.newMessage();
                resp->setAddr(this->ip);
                resp->setPort(this->porta);
                resp->setType(Message::MSG_RESP);
                resp->setToId(msg->getId());
                resp->setText("<" + SSTR(find->K) + ", \"" + this->tuplas[find->K] + "\">\n");
            } else { // Não possuo a chave
                // Verifica se deveria possuir a chave
                if ((find->K > this->prev.id && find->K <= this->id) // Caso usual
                    || (this->id == this->next.id) // Rede com somente um peer
                    || (zero_entre && ((find->K > this->prev.id && find->K > this->id) // Chave entre o peer anterior e o zero
                    || find->K <= this->id))) { // Chave entre o zero e o peer atual
                    // Se era para o peer atual possuir a chave, então esta não existe na rede
                    // Envia uma mensagem de erro para o peer que requisitou
                    peer.sockfd = this->pconnect(msg->getAddr(), msg->getPort());
                    resp = this->msgFct.newMessage();
                    resp->setAddr(this->ip);
                    resp->setPort(this->porta);
                    resp->setType(Message::MSG_ERROR);
                    resp->setToId(msg->getId());
                } else { // Chave não pertence ao peer atual
                    // Passa a requisição para o próximo peer
                    peer = this->next;
                    this->pconnect(peer);
                    resp = msg;
                }
            }
            // Envia a resposta
            this->psend(peer.sockfd, resp);
            close(peer.sockfd);
        }
            break;
        // Requisição de armazenamento
        case Message::MSG_STORE: {
            string stores = "";
            StoreCmd *store;
            list<Comando*> storeCmds;
            list<Comando*>::iterator it;
            bool zero_entre;

            storeCmds = this->parser.parseStores(msg->getText());
            for (it = storeCmds.begin(); it != storeCmds.end(); ++it) {
                store = (StoreCmd*) *it;
                zero_entre = (this->prev.id > this->id);
                // Verifica se a tupla pertence ao peer
                if ((store->tupla.first > this->prev.id && store->tupla.first <= this->id) // Caso usual
                    || (this->next.id == this->id) // Rede com somente um peer
                    || (zero_entre && ((store->tupla.first > this->prev.id && store->tupla.first > this->id)
                    || store->tupla.first <= this->id))) {
                    // Chave é responsabilidade do peer atual. Armazena
                    this->tuplas.insert(store->tupla);
                } else {
                    // Passa para o próximo chaves que não pertencem ao peer atual
                    stores += "STORE(<" + SSTR(store->tupla.first) + ",\"" + store->tupla.second + "\">)\n";
                }
            }

            // Se existe alguma chave que não pertence ao peer atual
            if (stores.length() > 0) {
                Message *repassa = this->msgFct.newMessage();
                repassa->setAddr(this->ip);
                repassa->setPort(this->porta);
                repassa->setType(Message::MSG_STORE);
                repassa->setText(stores);

                // Envia requisições de STORE para o próximo
                this->pconnect(this->next);
                this->psend(this->next.sockfd, repassa);
                close(this->next.sockfd);
                delete repassa;
            }
        }
            break;
        // Resposta a alguma requisição
        case Message::MSG_RESP:
            // Se é uma resposta para alguma requisição pendente do peer atual
            if (this->reqs.find(msg->getToId()) != this->reqs.end()) {
                Message *r_msg = this->reqs[msg->getToId()]->msg;
                switch (r_msg->getType()) {
                    // Resposta a uma requisição de ENTER
                    case Message::MSG_ENTER: {
                        stringstream ss;
                        string str;

                        // Peer que será o próximo do peer atual
                        ss.str(msg->getText());
                        getline(ss, str, '\n');
                        this->next.ip = str.substr(6);
                        getline(ss, str, '\n');
                        this->next.porta = str.substr(6);

                        // Peer que será o anterior do peer atual
                        getline(ss, str, '\n');
                        this->prev.ip = str.substr(6);
                        getline(ss, str, '\n');
                        this->prev.porta = str.substr(6);

                        // Calcula os ids
                        this->next.id = Peer::hash(this->next.ip + ":" + this->next.porta);
                        this->prev.id = Peer::hash(this->prev.ip + ":" + this->prev.porta);

                        // Seta a requisição como atendida
                        this->reqs[msg->getToId()]->done = true;

                        // Envia uma mensagem para o próximo, avisando que entrou na rede
                        Message *ack = this->msgFct.newMessage();
                        ack->setType(Message::MSG_ACK);
                        ack->setAddr(this->ip);
                        ack->setPort(this->porta);
                        this->pconnect(this->next);
                        this->psend(this->next.sockfd, ack);
                        close(this->next.sockfd);
                        delete ack;

                        // Propaga a atualização da lista de membros pelo anel
                        Message *sync = this->msgFct.newMessage();
                        sync->setType(Message::MSG_SYNC);
                        sync->setAddr(this->ip);
                        sync->setPort(this->porta);

                        map<unsigned int, Vizinho>::iterator it;
                        string members;
                        for (it = this->membros.begin(); it != this->membros.end(); ++it) {
                            members += "ENTER \"" + it->second.ip + "\" " + SSTR(it->second.porta) + "\n";
                        }

                        sync->setText(members);
                        this->pconnect(this->next);
                        this->psend(this->next.sockfd, sync);
                        close(this->next.sockfd);
                        delete sync;
                    }
                        break;
                    // Resposta a uma requisição FIND
                    case Message::MSG_FIND: {
                        StoreCmd *store = (StoreCmd*) this->parser.parse("STORE(" + msg->getText() + ")");

                        // Printa a tupla recebida
                        cout << msg->getAddr() << ":" << msg->getPort();
                        cout << " - " << store->tupla.second << endl;

                        // Seta a requisição como atendida
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
        // Alguma requisição resultou em erro
        case Message::MSG_ERROR:
            // Erro para alguma requisição pendente
            if (this->reqs.find(msg->getToId()) != this->reqs.end()) {
                // Erro para uma requisição de FIND: tupla não encontrada
                if (this->reqs[msg->getToId()]->msg->getType() == Message::MSG_FIND) {
                    // Tupla não encontrada
                    cout << "Chave não encontrada na rede." << endl;
                    this->reqs[msg->getToId()]->done = true;
                }
            } else {
                throw "Mensagem de erro não esperada";
            }
            break;
        // Informe que um peer entrou na rede, e está localizado antes do peer atual
        case Message::MSG_ACK: {
            unsigned long prev_id = this->prev.id;
            map<unsigned int, string>::iterator it;
            string stores = "";
            // Flag que indica se o peer entrou antes do zero
            bool before_zero;
            // Flag que informa se o zero da rede está entre o peer atual e seu anterior "antigo"
            bool zero_entre;

            // Passa a apontar o anterior para o novo peer que entrou na rede
            this->prev.ip = msg->getAddr();
            this->prev.porta = msg->getPort();
            this->prev.id = Peer::hash(this->prev.ip + ":" + this->prev.porta);

            // Seta flags
            zero_entre = (prev_id > this->id) || (prev_id == this->id && this->prev.id < this->id); // Inserção não usual
            before_zero = (this->prev.id > this->id);

            // Enviar para o peer as tuplas que pertencem ao seu intervalo de id
            for (it = this->tuplas.begin(); it != this->tuplas.end(); ++it) {
                /*** Esse if precisa urgentemente de otimizações ***/
                if ((!zero_entre && it->first > prev_id && it->first <= this->prev.id) // Caso usual
                    || (zero_entre && ((before_zero && it->first > prev_id && it->first <= this->prev.id) // Peer entrou antes do zero
                    || (!before_zero && !(it->first > this->prev.id && it->first <= this->id))))) { // Peer entrou depois do zero
                    stores += "STORE(<" + SSTR(it->first) + ",\"" + it->second + "\">)\n";
                }
            }

            // Se alguma chave na coleção do peer atual pertence ao peer que entrou, envia
            // uma mensagem de STORE para esse.
            if (stores.length() > 0) {
                Message *store = this->msgFct.newMessage();
                store->setAddr(this->ip);
                store->setPort(this->porta);
                store->setType(Message::MSG_STORE);
                store->setText(stores);

                this->pconnect(this->prev);
                this->psend(this->prev.sockfd, store);
                close(this->prev.sockfd);

                delete store;
            }
        }
            break;
        default:
            throw "Mensagem sem tratamento";
    }
}

/** Método estático auxiliar para a criação da PThread que processa as requisições
* @param arg void* - Ponteiro a estrutura que armazena o objeto peer e o socket da conexão
* da conexão recebida
*/
void* Peer::processa(void *con) {
    Conexao *conexao = (Conexao*) con;
    Peer *p = (Peer*) conexao->self;
    Message *msg;

    msg = p->receive(conexao->connfd);
    p->processa(msg);
    close(conexao->connfd);

    delete conexao;
    delete msg;
    pthread_exit(NULL);
}

/** Método estático auxiliar para a criação da PThread que recebe requisições
* @param arg void* - Ponteiro para o objeto da classe Peer
*/
void* Peer::serve(void *arg) {
    Peer *peer = (Peer*) arg;
    peer->serve();
    pthread_exit(NULL);
}

/** Método que interpreta os comandos do cliente
* @param cmd string - Comando informado na linha de comandos
*/
void Peer::parse(string cmd) {
    Comando *comando = this->parser.parse(cmd);

    switch (comando->tipo) {
        // Comando para entrar na rede. ENTER "IP" PORTA
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

            // Envia uma mensagem de ENTER para o peer informado
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
                cerr << "Tempo limite excedido." << endl;
            } else {
                t = clock() - req->t;
                cout << "Requisição concluída em " << ((float) t) / CLOCKS_PER_SEC << " segundos." << endl;
            }

            // Exclui a requisição
            this->reqs.erase(msg->getId());
            delete req;
            delete msg;
        }
            break;
        // Comando para buscar uma chave. FIND(K)
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

            // Envia a requisição de FIND para seu próprio SERVER
            int connfd = this->pconnect(this->ip, this->porta);
            this->psend(connfd, msg);
            close(connfd);

            // Espera TIMEOUT segundos ou até receber a resposta
            req->t = clock();
            clock_t t = (clock() - req->t) / CLOCKS_PER_SEC;
            while (t < TIMEOUT && !req->done) {
                t = (clock() - req->t) / CLOCKS_PER_SEC;
            }

            if (!req->done) {
                cerr << "Tempo limite excedido." << endl;
            } else {
                t = clock() - req->t;
                cout << "Requisição concluída em " << ((float) t) / CLOCKS_PER_SEC << " segundos." << endl;
            }

            // Exclui a requisição
            this->reqs.erase(msg->getId());
            delete req;
            delete msg;
        }
            break;
        // Comando para listar os vizinhos e as truplas. LIST
        case Comando::CMD_LIST:
            cout << "Vizinhos: " << endl;
            cout << "Anterior - " << this->prev.ip << ":" << this->prev.porta  << " (" << this->prev.id << ")" << endl;
            cout << "Próximo - " << this->next.ip << ":" << this->next.porta  << " (" << this->next.id << ")" << endl;
            cout << endl;
            cout << "Tuplas Armazenadas:" << endl;
            for (map<unsigned int, string>::iterator it = this->tuplas.begin(); it != this->tuplas.end(); ++it) {
                cout << "<" << it->first << ", \"" << it->second << "\">" << endl;
            }
            break;
        // Comando para armazenar uma trupla. STORE(<K,V>)
        case Comando::CMD_STORE: {
            StoreCmd *store = (StoreCmd*) comando;
            Message *msg = this->msgFct.newMessage();

            msg->setAddr(this->ip);
            msg->setPort(this->porta);
            msg->setType(Message::MSG_STORE);
            msg->setText("STORE(<" + SSTR(store->tupla.first) + ",\"" + store->tupla.second + "\">)\n");

            // Envia a requisição de STORE para o próprio SERVER
            int connfd = this->pconnect(this->ip, this->porta);
            this->psend(connfd, msg);
            close(connfd);

            delete msg;
        }
            break;
        // Comando para mostrar a lista de membros no grupo. MEMBERS
        case Comando::CMD_MEMBERS: {
            map<unsigned int, Vizinho>::iterator it;
            
            for (it = this->membros.begin(); it != this->membros.end(); ++it) {
                cout << it->second.ip << ":" << it->second.porta << " (" << it->first << ")" << endl;
            }
        }
            break;
        // Comando para encerrar a aplicação. QUIT
        case Comando::CMD_QUIT:
            // Encerra a aplicação
            close(this->sockfd);
            cout << "Adeus!" << endl;
            exit(EXIT_SUCCESS);
            break;
        // Demais comandos
        default:
            // Tratamento padrão: chama o método executar do comando.
            comando->executar();
    }

    delete comando;
}
