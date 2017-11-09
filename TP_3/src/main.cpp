#include <peer.hpp>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>

using namespace std;

int main(int argc, char **argv) {
	string cmd;

	if (argc != 3) {
		cout << "Faltam argumentos!" << endl;
		cout << "Utilização: peer.out IP PORTA" << endl;
		return EXIT_FAILURE;
	}

	Peer p(argv[1], argv[2]);

	p.start();

	while (true) {
		cout << "> ";
		getline(cin, cmd);

		try {
			p.parse(cmd);
		} catch (string msg) {
			cout << msg << endl;
		}
	}

	return EXIT_SUCCESS;
}