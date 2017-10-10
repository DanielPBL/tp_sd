#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

#include <string>
#include <sstream>
#include <utility>

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

class MessageFactory;

class Message {
    friend class MessageFactory;
public:
    enum Type {
        ERROR,
        PING,
        PONG,
        FIND,
        STORE,
        RESPONSE
    };
private:
    unsigned int id;
    Message::Type type;
    std::pair<int, std::string> tupla;
    std::string addr;
    std::string port;
    int size;
public:
    unsigned int getId() const;
    Message::Type getType() const;
    std::pair<int, std::string> getPair() const;
    std::string getAddr() const;
    std::string getPort() const;
    int getSize() const;
    void setId(unsigned int i);
    void setType(Message::Type t);
    void setTupla(std::pair<int, std::string> t);
    void setAddr(std::string a);
    void setPort(std::string a);
    void setSize(int s);

    std::string toString() const;
    static std::string TypeDesc(Message::Type type);
    static Message::Type FindType(std::string type);
private:
    Message();
    Message(std::string msg);
};

#endif
