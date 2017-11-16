#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

#include <string>
#include <sstream>

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

class MessageFactory;

class Message {
    friend class MessageFactory;
public:
    enum Type {
        MSG_ERROR,
        MSG_PING,
        MSG_PONG,
        MSG_FIND,
        MSG_STORE,
        MSG_RESP,
        MSG_ENTER,
        MSG_ACK,
        MSG_HEARTB,
        MSG_SYNC,
        MSG_DIED,
        MSG_REPL
    };
private:
    unsigned int id;
    Message::Type type;
    std::string text;
    std::string addr;
    std::string port;
    int size;
    int toId;
public:
    unsigned int getId() const;
    Message::Type getType() const;
    std::string getText() const;
    std::string getAddr() const;
    std::string getPort() const;
    int getSize() const;
    int getToId() const;
    void setId(unsigned int i);
    void setType(Message::Type t);
    void setText(std::string t);
    void setAddr(std::string a);
    void setPort(std::string a);
    void setSize(int s);
    void setToId(int to);

    std::string toString() const;
    static std::string TypeDesc(Message::Type type);
    static Message::Type FindType(std::string type);
private:
    Message();
    Message(std::string msg);
};

#endif
