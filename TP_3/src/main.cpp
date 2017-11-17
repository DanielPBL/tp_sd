#include <peer.hpp>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <ctime>

using namespace std;

int main(int argc, char **argv) {
	string cmd;

	if (argc < 3 || argc > 4) {
		cout << "Argumentos incorretos!" << endl;
		cout << "Utilização: peer.out IP PORTA (TAXA_PERDA)" << endl;
		return EXIT_FAILURE;
	}

	srand(time(NULL));
	int taxa = argc > 3 ? atoi(argv[3]) : 0;

	if (taxa < 0 || taxa > 100) {
		cout << "A TAXA_PERDA deve estar no intervalo [0, 100]" << endl;
		return EXIT_FAILURE;
	}

	Peer p(argv[1], argv[2], taxa);
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
