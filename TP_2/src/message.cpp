#include <message.hpp>
#include <string>
#include <cstdlib>
#include <sstream>
#include <utility>

using namespace std;

Message::Message() {
    this->id = 0;
    this->addr = "";
    this->port = "";
    this->tupla = make_pair(0, "");
    this->size = 0;
    this->type = Message::ERROR;
}

// Faz o parse da string como uma mensagem (desserialização)
Message::Message(string msg) {
    stringstream ss;
    string str;

    ss.str(msg);
    getline(ss, str, '\n');
    this->type = Message::FindType(str.substr(6));
    getline(ss, str, '\n');
    this->addr = str.substr(6);
    getline(ss, str, '\n');
    this->port = str.substr(6);
    if (this->type != Message::PING) {
        getline(ss, str);
        this->size = atoi(str.substr(6).c_str());
        //this->text = ss.str().substr(ss.tellg());
    }
}

// Serializa o objeto Message em uma string
string Message::toString() const {
    stringstream ss;

    ss << "Type: " << Message::TypeDesc(this->type) << endl;
    ss << "Addr: " << this->addr << endl;
    ss << "Port: " << this->port << endl;

    if (this->type != Message::PING) {
        ss << "Size: " << this->size << endl;
        //ss << this->text;
    }

    return ss.str();
}

unsigned int Message::getId() const {
    return this->id;
}

Message::Type Message::getType() const {
    return this->type;
}

std::pair<int, string> Message::getPair() const {
    return this->tupla;
}

std::string Message::getAddr() const {
    return this->addr;
}

std::string Message::getPort() const {
    return this->port;
}

int Message::getSize() const {
    return this->size;
}

void Message::setId(unsigned int i) {
    this->id = i;
}

void Message::setType(Message::Type t) {
    this->type = t;
}

void Message::setTupla(std::pair<int, string> t) {
    this->tupla = t;
    this->setSize(0);
}

void Message::setAddr(std::string a) {
    this->addr = a;
}

void Message::setPort(std::string p) {
    this->port = p;
}

void Message::setSize(int s) {
    this->size = s;
}

string Message::TypeDesc(Message::Type type) {
    switch (type) {
        case Message::ERROR:
            return "ERROR";
        case Message::PING:
            return "PING";
        case Message::PONG:
            return "PONG";
        case Message::FIND:
            return "FIND";
        case Message::STORE:
            return "STORE";
        case Message::RESPONSE:
            return "RESPONSE";
    }

    return "ERROR";
}

Message::Type Message::FindType(std::string type) {
    if (type == "PING") {
        return Message::PING;
    } else if (type == "PONG") {
        return Message::PONG;
    } else if (type == "FIND") {
        return Message::FIND;
    } else if (type == "STORE") {
        return Message::STORE;
    } else if (type == "RESPONSE") {
        return Message::RESPONSE;
    } else {
        return Message::ERROR;
    }

    return Message::ERROR;
}
