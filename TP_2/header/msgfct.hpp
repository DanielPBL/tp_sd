#ifndef __MSGFCT_HPP__
#define __MSGFCT_HPP__

#include <message.hpp>

class MessageFactory {
private:
    unsigned int id;
public:
    MessageFactory();
    Message* newMessage();
    Message* parseMessage();
};

#endif
