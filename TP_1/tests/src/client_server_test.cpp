#include "server.hpp"
#include "client.hpp"
#include "message.hpp"
#include "exec.hpp"
#include "gtest/gtest.h"
#include <cstring>
#include <pthread.h>

namespace {
    //Usados para simular interação cliente-servidor
    Server s("127.0.0.1", "4000", "5000");
    Client c("127.0.0.1", "5000");

    // Simula uma QUERY como cliente
    void *client_simulator(void *ptr) {
        Message *msg, resp;

        c.cconnect("127.0.0.1", "4000");
        resp.setType(Message::QUERY);
        resp.setAddr("127.0.0.1");
        resp.setPort("1717");
        resp.setText("grep \"Linha 1:\" maquina.*.log");
        c.csend(resp);
        msg = c.receive();
        delete msg;
        pthread_exit(0);
        return ptr;
    }

    // Simula uma QUERY como servidor
    void *server_simulator(void *ptr) {
        Message *msg, resp;

        s.saccept();
        msg = s.receive();
        resp.setType(Message::RESPONSE);
        resp.setAddr("127.0.0.1");
        resp.setPort("4000");
        resp.setText(exec(msg->getText().c_str()));
        s.ssend(resp);
        delete msg;
        pthread_exit(0);
        return ptr;
    }

    // Construtores da classe Server
    TEST(Constructors, Server) {
        Server s1("127.0.0.1", "4001", "5001");
        EXPECT_STREQ("127.0.0.1", s1.si.ip);
        EXPECT_EQ(4001, s1.si.port);
        EXPECT_EQ(5001, s1.si.bc_port);
    }

    // Construtores da classe Client
    TEST(Constructors, Client) {
        // Construtor IP BROADCAST
        Client c1("127.0.0.1", "5002");
        EXPECT_EQ("127.0.0.1", c1.getIp());
        EXPECT_EQ(5002, c1.si.bc_port);
        EXPECT_TRUE(c1.si.sl.empty());

        // Construtor IP BROADCAST + SERVER
        Client c2("127.0.0.1", "5003", "127.0.0.1", "4000");
        EXPECT_EQ("127.0.0.1", c2.getIp());
        EXPECT_EQ(5003, c2.si.bc_port);
        EXPECT_FALSE(c2.si.sl.empty());
        EXPECT_EQ(1, c2.si.sl.size());
        EXPECT_EQ("4000", c2.si.sl["127.0.0.1"]);
    }

    // Teste da descoberta de rede
    TEST(Network, NetworkDiscovery) {
        s.broadcast();
        EXPECT_TRUE(c.si.sl.empty());
        c.listen();
        // Espera ocupada
        while(c.si.sl.empty());

        EXPECT_FALSE(c.si.sl.empty());
        EXPECT_EQ(1, c.si.sl.size());
        EXPECT_EQ("4000", c.si.sl["127.0.0.1"]);
    }

    // Teste dos métodos para o envio de uma query (server-side)
    // Executa uma thread que fará o papel do cliente
    TEST(QueryMsg, ServerSide) {
        Message *msg, resp;
        pthread_t t;

        // Essa thread vai rodar em paralelo para simular o cliente
        ASSERT_EQ(0, pthread_create(&t, NULL, client_simulator, NULL));

        // servidor espera a conexão do cliente e recebe a mensagem
        s.saccept();
        msg = s.receive();

        // Mensagem de QUERY equivalente a da função client_simulator
        EXPECT_EQ(Message::QUERY, msg->getType());
        EXPECT_EQ("127.0.0.1", msg->getAddr());
        EXPECT_EQ("1717", msg->getPort());
        EXPECT_EQ(29, msg->getSize());
        EXPECT_EQ("grep \"Linha 1:\" maquina.*.log", msg->getText());

        // Responder a mensagem
        resp.setType(Message::RESPONSE);
        resp.setAddr("127.0.0.1");
        resp.setPort("4000");
        resp.setText(exec(msg->getText().c_str()));

        s.ssend(resp);
        delete msg;
    }

    // Teste dos métodos para o envio de uma query (client-side)
    // Executa uma thread que fará o papel do servidor
    TEST(QueryMsg, ClientSide) {
        Message *resp, msg;
        pthread_t t;

        // Essa thread vai rodar em paralelo para simular o servidor
        ASSERT_EQ(0, pthread_create(&t, NULL, server_simulator, NULL));

        // Cliente se conecta ao servidor
        c.cconnect("127.0.0.1", "4000");

        ASSERT_NE(-1, c.getSockfd());

        // Monta QUERY
        msg.setType(Message::QUERY);
        msg.setAddr("127.0.0.1");
        msg.setPort("1717");
        msg.setText("grep \"Linha 1:\" maquina.*.log");

        // Envia a mensagem e obtem resposta
        c.csend(msg);
        resp = c.receive();

        // Mensagem de RESPONSE equivalente a função server_simulator
        EXPECT_EQ(Message::RESPONSE, resp->getType());
        EXPECT_EQ("127.0.0.1", resp->getAddr());
        EXPECT_EQ("4000", resp->getPort());
        EXPECT_EQ(93, resp->getSize());
        EXPECT_STRNE(NULL, strstr(resp->getText().c_str(),
            "Linha 1: Lorem ipsum dolor sit amet, consectetur adipiscing elit."));

        delete resp;
    }

    // Teste de falha no servidor
    TEST(QueryMsg, ServerFailure) {
        Client c1("127.0.0.1", "5004", "127.0.0.1", "4004");

        // Se o server falha, o cliente deve abandonar esse servidor
        EXPECT_EQ(1, c1.si.sl.size());
        EXPECT_FALSE(c1.query("whoami"));
        EXPECT_EQ(0, c1.si.sl.size());
    }
}  // namespace
