#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <server.hpp>
#include <message.hpp>
#include <sstream>
#include <iostream>
#include <exec.hpp>

using namespace std;

int main(int argc, char** argv) {
    Server *s;
    Message *msg, resp;

    if (argc != 4) {
        cerr << "Utilização: server.out IP TCP_PORT UDP_PORT";
        exit(EXIT_FAILURE);
    }

    s = new Server(argv[1], argv[2], argv[3]);

    s->broadcast();

    while (true) {
        s->saccept();

        if (!fork()) {
            msg = s->receive();

            resp.setType(Message::RESPONSE);
            resp.setAddr(argv[1]);
            resp.setPort(argv[2]);
            resp.setText(exec(msg->getText().c_str()));

            s->ssend(resp);
            delete msg;
            break;
        }
    }

    delete s;
    return EXIT_SUCCESS;
}

