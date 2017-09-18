#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

#include <string>
#include <sstream>

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

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
    int size;
public:
    Message();
    Message(std::string msg);

    Message::Type getType();
    std::string getText();
    std::string getAddr();
    std::string getPort();
    int getSize();
    void setType(Message::Type t);
    void setText(std::string t);
    void setAddr(std::string a);
    void setPort(std::string a);
    void setSize(int s);

    std::string toString() const;
    static std::string TypeDesc(Message::Type type);
    static Message::Type FindType(std::string type);
};

#endif
