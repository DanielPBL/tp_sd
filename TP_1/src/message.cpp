#include <message.hpp>
#include <string>
#include <cstdlib>
#include <sstream>

using namespace std;

Message::Message() {
    this->addr = "";
    this->port = "";
    this->text = "";
    this->size = 0;
    this->type = Message::ERROR;
}

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
        getline(ss, str); //Tamanho é importante?
        this->size = atoi(str.substr(6).c_str());
        this->text = ss.str().substr(ss.tellg()); //
    }
}

string Message::toString() const {
    stringstream ss;

    ss << "Type: " << Message::TypeDesc(this->type) << endl;
    ss << "Addr: " << this->addr << endl;
    ss << "Port: " << this->port << endl;

    if (this->type != Message::PING) {
        ss << "Size: " << this->size << endl;
        ss << this->text;
    }

    return ss.str();
}

Message::Type Message::getType() {
    return this->type;
}

std::string Message::getText() {
    return this->text;
}

std::string Message::getAddr() {
    return this->addr;
}

std::string Message::getPort() {
    return this->port;
}

int Message::getSize() {
    return this->size;
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

string Message::TypeDesc(Message::Type type) {
    switch (type) {
        case Message::ERROR:
            return "ERROR";
        case Message::PING:
            return "PING";
        case Message::PONG:
            return "PONG";
        case Message::QUERY:
            return "QUERY";
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
    } else if (type == "QUERY") {
        return Message::QUERY;
    } else if (type == "RESPONSE") {
        return Message::RESPONSE;
    } else {
        return Message::ERROR;
    }

    return Message::ERROR;
}
