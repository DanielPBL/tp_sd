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
    this->text = "";
    this->size = 0;
    this->type = Message::MSG_ERROR;
}

// Faz o parse da string como uma mensagem (desserialização)
Message::Message(string msg) {
    stringstream ss;
    string str;

    ss.str(msg);
    getline(ss, str, '\n');
    this->id = atoi(str.substr(4).c_str());
    getline(ss, str, '\n');
    this->type = Message::FindType(str.substr(6));
    getline(ss, str, '\n');
    this->addr = str.substr(6);
    getline(ss, str, '\n');
    this->port = str.substr(6);
    getline(ss, str);
    this->size = atoi(str.substr(6).c_str());

    switch (this->type) {
        case Message::MSG_RESP:
            getline(ss, str);
            this->toId = atoi(str.substr(4).c_str());
            break;
        default:
            //Nada a se fazer
            break;
    }

    this->text = ss.str().substr(ss.tellg());
}

// Serializa o objeto Message em uma string
string Message::toString() const {
    stringstream ss;

    ss << "Type: " << Message::TypeDesc(this->type) << endl;
    ss << "Addr: " << this->addr << endl;
    ss << "Port: " << this->port << endl;

    if (this->type != Message::MSG_PING) {
        ss << "Size: " << this->size << endl;
        ss << this->text;
    }

    return ss.str();
}

unsigned int Message::getId() const {
    return this->id;
}

Message::Type Message::getType() const {
    return this->type;
}

string Message::getText() const {
    return this->text;
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

int Message::getToId() const {
    return this->toId;
}

void Message::setId(unsigned int i) {
    this->id = i;
}

void Message::setType(Message::Type t) {
    this->type = t;
}

void Message::setText(std::string t) {
    this->text = t;
    this->setSize(t.size());
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

void Message::setToId(int to) {
    this->toId = to;
}

string Message::TypeDesc(Message::Type type) {
    switch (type) {
        case Message::MSG_ERROR:
            return "ERROR";
        case Message::MSG_PING:
            return "PING";
        case Message::MSG_PONG:
            return "PONG";
        case Message::MSG_FIND:
            return "FIND";
        case Message::MSG_STORE:
            return "STORE";
        case Message::MSG_RESP:
            return "RESPONSE";
        case Message::MSG_ENTER:
            return "ENTER";
    }

    return "ERROR";
}

Message::Type Message::FindType(std::string type) {
    if (type == "PING") {
        return Message::MSG_PING;
    } else if (type == "PONG") {
        return Message::MSG_PONG;
    } else if (type == "FIND") {
        return Message::MSG_FIND;
    } else if (type == "STORE") {
        return Message::MSG_STORE;
    } else if (type == "RESPONSE") {
        return Message::MSG_RESP;
    } if (type == "ENTER") {
        return Message::MSG_ENTER;
    } else {
        return Message::MSG_ERROR;
    }

    return Message::MSG_ERROR;
}
