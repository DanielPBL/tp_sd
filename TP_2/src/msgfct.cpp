#include <msgfct.hpp>

using namespace std;

MessageFactory::MessageFactory() : id(0) {
}

Message* MessageFactory::newMessage() {
    Message* msg = new Message();
    this->id++;
    msg->setId(this->id);
    return msg;
}

Message* MessageFactory::parseMessage(string msg) {
    return new Message(msg);
}
