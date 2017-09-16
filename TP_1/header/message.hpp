#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

#include <string>

class Message {
public:

    enum Type {
        ERROR,
        PING,
        PONG,
        QUERY,
        RESPONSE
    };
private:
    Message::Type type;
    std::string text;
    std::string addr;
    std::string port;
public:
    Message(std::string);
    std::string toString() const;
    static std::string TypeDesc(Message::Type);
    static Message::Type FindType(std::string);
};

#endif
