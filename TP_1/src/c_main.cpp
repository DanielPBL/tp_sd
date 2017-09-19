#include <client.hpp>
#include <cstdlib>
#include <cstdio>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    Client *c;
    string cmd;

    if (argc != 5 && argc != 3) {
        cerr << "Utilização: client.out IP UDP_PORT (IP_SERVER PORTA_SERVER)" << endl;
        return EXIT_FAILURE;
    }

    if (argc == 3) {
        c = new Client(argv[1], argv[2]);
    } else {
        c = new Client(argv[1], argv[2], argv[3], argv[4]);
    }

    c->listen();

    while (true) {
        cout << "> ";
        getline(cin, cmd);

        if (cmd == "quit") {
            break;
        }

        c->query(cmd);
    }

    return EXIT_SUCCESS;
}
