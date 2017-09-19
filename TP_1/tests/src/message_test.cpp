#include "message.hpp"
#include "gtest/gtest.h"

namespace {
    TEST(MessageTest, ConversionMacro) {
        EXPECT_EQ("5", SSTR(5));
        EXPECT_EQ("1.1", SSTR(1.1));
        EXPECT_EQ("A", SSTR('A'));
    }

    TEST(MessageTest, Constructors) {
        // Testa construtor padrão
        Message msg1;
        EXPECT_EQ("", msg1.getAddr());
        EXPECT_EQ("", msg1.getPort());
        EXPECT_EQ("", msg1.getText());
        EXPECT_EQ(0, msg1.getSize());
        EXPECT_EQ(Message::ERROR, msg1.getType());

        // Testa construtor de string
        Message msg2("Type: QUERY\nAddr: localhost\nPort: 8080\nSize: 9\nOlá mundo");
        EXPECT_EQ("localhost", msg2.getAddr());
        EXPECT_EQ("8080", msg2.getPort());
        EXPECT_EQ("Olá mundo", msg2.getText());
        EXPECT_EQ(9, msg2.getSize());
        EXPECT_EQ(Message::QUERY, msg2.getType());
    }

    TEST(MessageTest, StaticMember) {
        // Obtem descrição textual (serialização)
        EXPECT_EQ("ERROR", Message::TypeDesc(Message::ERROR));
        EXPECT_EQ("PING", Message::TypeDesc(Message::PING));
        EXPECT_EQ("PONG", Message::TypeDesc(Message::PONG));
        EXPECT_EQ("QUERY", Message::TypeDesc(Message::QUERY));
        EXPECT_EQ("RESPONSE", Message::TypeDesc(Message::RESPONSE));

        // Obtem valor real (desserialização)
        EXPECT_EQ(Message::ERROR, Message::FindType("ERROR"));
        EXPECT_EQ(Message::PING, Message::FindType("PING"));
        EXPECT_EQ(Message::PONG, Message::FindType("PONG"));
        EXPECT_EQ(Message::QUERY, Message::FindType("QUERY"));
        EXPECT_EQ(Message::RESPONSE, Message::FindType("RESPONSE"));
    }
}  // namespace
