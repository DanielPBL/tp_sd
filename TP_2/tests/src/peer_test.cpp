#include "peer.hpp"
#include "gtest/gtest.h"
#include <unistd.h>

using namespace std;

namespace {
    // Somente 2 peers devido o limite de criação de thread por processo
    Peer p1("localhost", "5000"); //856
    Peer p2("localhost", "6000"); //832

    // Teste do custrutor da classe Peer
    TEST(PeerTest, Constructor) {
        EXPECT_EQ(856, p1.getId());
        EXPECT_EQ("localhost", p1.getIp());
        EXPECT_EQ("5000", p1.getPorta());
        EXPECT_EQ(856, p1.getNext().id);
        EXPECT_EQ("localhost", p1.getNext().ip);
        EXPECT_EQ("5000", p1.getNext().porta);
        EXPECT_EQ(856, p1.getPrev().id);
        EXPECT_EQ("localhost", p1.getPrev().ip);
        EXPECT_EQ("5000", p1.getPrev().porta);

        EXPECT_EQ(832, p2.getId());
        EXPECT_EQ("localhost", p2.getIp());
        EXPECT_EQ("6000", p2.getPorta());
    }

    // Teste da criação das threads dos servidores dos peers
    TEST(PeerTest, StartServer) {
        EXPECT_NO_THROW(p1.start());
        EXPECT_NO_THROW(p2.start());
    }

    /**
    A PARTIR DAQUI, SLEEPS FORAM USADOS DEVIDO A SOBRECARGA DE THREADS
    E (PORCAMENTE) PARA SINCRONIZAÇÂO =)
    **/

    // Teste da inserção dos peers
    TEST(PeerTest, EnterCmd) {
        testing::internal::CaptureStdout();
        EXPECT_NO_THROW(p2.parse("ENTER \"localhost\" 5000"));
        usleep(200);
        string output = testing::internal::GetCapturedStdout();
        EXPECT_NE(output.find("Requisição concluída"), string::npos);

        EXPECT_EQ(832, p1.getNext().id);
        EXPECT_EQ(832, p1.getPrev().id);
        EXPECT_EQ(856, p2.getNext().id);
        EXPECT_EQ(856, p2.getPrev().id);
    }

    // Teste da inserção de tuplas na rede
    TEST(PeerTest, StoreCmd) {
        usleep(200);
        EXPECT_NO_THROW(p2.parse("STORE(<856, \"856\">)"));
        usleep(200);
        EXPECT_NO_THROW(p1.parse("STORE(<832, \"832\">)"));
        usleep(200);
    }

    // Testa da busca de tuplas
    TEST(PeerTest, FindCmd) {
        testing::internal::CaptureStdout();
        EXPECT_NO_THROW(p1.parse("FIND(1)"));
        usleep(200);
        string output = testing::internal::GetCapturedStdout();
        EXPECT_NE(output.find("Chave não encontrada na rede."), string::npos);

        testing::internal::CaptureStdout();
        EXPECT_NO_THROW(p1.parse("FIND(856)"));
        usleep(200);
        output = testing::internal::GetCapturedStdout();
        EXPECT_NE(output.find("856"), string::npos);

        testing::internal::CaptureStdout();
        EXPECT_NO_THROW(p1.parse("FIND(832)"));
        usleep(200);
        output = testing::internal::GetCapturedStdout();
        EXPECT_NE(output.find("832"), string::npos);
    }

    // Testa o comando LIST
    TEST(PeerTest, ListCmd) {
        testing::internal::CaptureStdout();
        EXPECT_NO_THROW(p1.parse("LIST"));
        string output = testing::internal::GetCapturedStdout();
        EXPECT_NE(output.find("<856, \"856\">"), string::npos);

        testing::internal::CaptureStdout();
        EXPECT_NO_THROW(p2.parse("LIST"));
        output = testing::internal::GetCapturedStdout();
        EXPECT_NE(output.find("<832, \"832\">"), string::npos);
    }
}  // namespace
