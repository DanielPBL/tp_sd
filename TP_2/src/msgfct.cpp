#include <msgfct.hpp>

MessageFactory::MessageFactory() : id(0) {
}

Message* MessageFactory::newMessage() {
    Message* msg = new Message();
    this->id++;
    msg->setId(this->id);
    return msg;
}

/*

    MessageFactory();
    Message* newMessage();
    Message* parseMessage();

*/
