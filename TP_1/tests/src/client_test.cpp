#include "client.hpp"
#include "gtest/gtest.h"

namespace {
  TEST(ClientTest, Constructors) {
    // Construtor de 2 argumentos
    Client c1("127.0.0.1", "5000");
    EXPECT_EQ("127.0.0.1", c1.getIp());
    EXPECT_EQ(5000, c1.si.bc_port);
    EXPECT_EQ(0, c1.si.sl.size());

    // Construtor de 4 argumentos
    Client c2("127.0.0.1", "5000", "127.0.0.1", "4001");
    EXPECT_EQ("127.0.0.1", c2.getIp());
    EXPECT_EQ(4001, c2.si.bc_port);
    EXPECT_EQ(1, c1.si.sl.size());
    EXPECT_EQ("4001", c1.si.sl["127.0.0.1"]);
  }

  TEST(ClientTest, FindServer) {
    // Cliente inicialmente sem servidores
    Client c1("127.0.0.1", "5000");
    // Cria thread de listen
    c1.listen();
    // Realiza uma query como garantia que algum server foi encontrado
    c1.query("whoami");
    EXPECT_EQ(1, c1.si.sl.size());
    EXPECT_EQ("4001", c1.si.sl["127.0.0.1"]);
  }

  TEST(ClientTest, SuccessfulQuery) {
    Client c1("127.0.0.1", "5000");
    Message qry("Type: QUERY\nAddr: 127.0.0.1\nPort: 41045\nSize: 9\necho -n \"Olá mundo\"");
    Message *resp;

    c1.listen();
    //Espera ocupada
    while (c1.si.sl.empty());
    c1.csend(0, qry);
  }
}  // namespace
