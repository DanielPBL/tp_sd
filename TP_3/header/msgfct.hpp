#ifndef __MSGFCT_HPP__
#define __MSGFCT_HPP__

#include <message.hpp>
#include <string>

class MessageFactory {
private:
    unsigned int id;
public:
    MessageFactory();
    Message* newMessage();
    Message* parseMessage(std::string msg);
};

#endif
