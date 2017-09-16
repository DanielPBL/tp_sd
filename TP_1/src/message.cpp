#include <message.hpp>
#include <string>
#include <sstream>

using namespace std;

Message::Message(string msg) {
    stringstream ss;
    string str;

    ss.str(msg);
    //str = getline(ss, str, endl);
    this->type = Message::FindType(str);
    this->addr = str;
    this->port = str;
    this->text = str;
}

string Message::toString() const {
    stringstream ss;

    ss << "Type: " << Message::TypeDesc(this->type) << endl;
    ss << "Addr: " << this->addr << endl;
    ss << "Port: " << this->port << endl;
    ss << "Size: " << this->text.size() << endl;
    ss << this->text;

    return ss.str();
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
