#include <message.hpp>
#include <msgfct.hpp>
#include "gtest/gtest.h"

namespace {
    TEST(MessageTest, ConversionMacro) {
        EXPECT_EQ("5", SSTR(5));
        EXPECT_EQ("1.1", SSTR(1.1));
        EXPECT_EQ("A", SSTR('A'));
    }

    TEST(MessageTest, Constructors) {
        // Testa construtor padrão
        MessageFactory msgFct;
        Message *msg1 = msgFct.newMessage();

        EXPECT_EQ("", msg1->getAddr());
        EXPECT_EQ("", msg1->getPort());
        EXPECT_EQ("", msg1->getText());
        EXPECT_EQ(0, msg1->getSize());
        EXPECT_EQ(Message::MSG_ERROR, msg1->getType());

        // Testa construtor de string
        Message *msg2 = msgFct.parseMessage("Id: 2\nType: ACK\nAddr: localhost\nPort: 8080\nSize: 0\n");
        EXPECT_EQ("localhost", msg2->getAddr());
        EXPECT_EQ("8080", msg2->getPort());
        EXPECT_EQ("", msg2->getText());
        EXPECT_EQ(0, msg2->getSize());
        EXPECT_EQ(Message::MSG_ACK, msg2->getType());

        delete msg1;
        delete msg2;
    }

    TEST(MessageTest, StaticMember) {
        // Obtem descrição textual (serialização)
        EXPECT_EQ("ERROR", Message::TypeDesc(Message::MSG_ERROR));
        EXPECT_EQ("PING", Message::TypeDesc(Message::MSG_PING));
        EXPECT_EQ("PONG", Message::TypeDesc(Message::MSG_PONG));
        EXPECT_EQ("FIND", Message::TypeDesc(Message::MSG_FIND));
        EXPECT_EQ("STORE", Message::TypeDesc(Message::MSG_STORE));
        EXPECT_EQ("ENTER", Message::TypeDesc(Message::MSG_ENTER));
        EXPECT_EQ("ACK", Message::TypeDesc(Message::MSG_ACK));
        EXPECT_EQ("RESPONSE", Message::TypeDesc(Message::MSG_RESP));

        // Obtem valor real (desserialização)
        EXPECT_EQ(Message::MSG_ERROR, Message::FindType("ERROR"));
        EXPECT_EQ(Message::MSG_PING, Message::FindType("PING"));
        EXPECT_EQ(Message::MSG_PONG, Message::FindType("PONG"));
        EXPECT_EQ(Message::MSG_FIND, Message::FindType("FIND"));
        EXPECT_EQ(Message::MSG_STORE, Message::FindType("STORE"));
        EXPECT_EQ(Message::MSG_ENTER, Message::FindType("ENTER"));
        EXPECT_EQ(Message::MSG_ACK, Message::FindType("ACK"));
        EXPECT_EQ(Message::MSG_RESP, Message::FindType("RESPONSE"));
    }

    TEST(MessageTest, Stringfy) {
        // Teste de serialização
        MessageFactory msgFct;
        Message *msg1 = msgFct.newMessage();
        msg1->setType(Message::MSG_FIND);
        msg1->setAddr("127.0.0.1");
        msg1->setPort("47512");
        msg1->setText("FIND(1)");

        EXPECT_EQ("Id: 1\nType: FIND\nAddr: 127.0.0.1\nPort: 47512\nSize: 7\nFIND(1)",
                    msg1->toString());

        Message *msg2 = msgFct.newMessage();
        msg2->setType(Message::MSG_STORE);
        msg2->setAddr("127.0.0.1");
        msg2->setPort("47512");
        msg2->setText("STORE(<1, \"1\")");

        EXPECT_EQ("Id: 2\nType: STORE\nAddr: 127.0.0.1\nPort: 47512\nSize: 14\nSTORE(<1, \"1\")",
                    msg2->toString());

        delete msg1;
        delete msg2;
    }
}  // namespace
