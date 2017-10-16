#include "parser/parser.hpp"
#include "parser/findcmd.hpp"
#include "parser/storecmd.hpp"
#include "parser/entercmd.hpp"
#include "gtest/gtest.h"

namespace {
    Parser parser;

    TEST(ParserTest, FindComand) {
        FindCmd *find = (FindCmd*) parser.parse("FIND(1)");

        EXPECT_EQ(Comando::CMD_FIND, find->tipo);
        EXPECT_EQ(1, find->K);

        delete find;
    }

    TEST(ParserTest, StoreComand) {
        StoreCmd *store = (StoreCmd*) parser.parse("STORE(<1,\"teste\">)");

        EXPECT_EQ(Comando::CMD_STORE, store->tipo);
        EXPECT_EQ(1, store->tupla.first);
        EXPECT_EQ("teste", store->tupla.second);

        delete store;
    }

    TEST(ParserTest, EnterComand) {
        EnterCmd *enter = (EnterCmd*) parser.parse("ENTER \"localhost\" 5000");

        EXPECT_EQ(Comando::CMD_ENTER, enter->tipo);
        EXPECT_EQ("localhost", enter->ip);
        EXPECT_EQ("5000", enter->porta);

        delete enter;
    }

    TEST(ParserTest, OtherComands) {
        Comando *list = parser.parse("LIST");
        Comando *quit = parser.parse("QUIT");
        Comando *help = parser.parse("HELP");

        EXPECT_EQ(Comando::CMD_LIST, list->tipo);
        EXPECT_EQ(Comando::CMD_QUIT, quit->tipo);
        EXPECT_EQ(Comando::CMD_HELP, help->tipo);

        delete list;
        delete quit;
        delete help;
    }

    TEST(ParserTest, ErrorComand) {
        EXPECT_ANY_THROW(parser.parse("TESTE"));
    }
}  // namespace
