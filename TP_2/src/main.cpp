#include <parser/parser.hpp>
#include <iostream>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

using namespace std;

int main(int argc, char **argv) {
	string cmd;
	Parser parser;

	/*if (argc < 2) {
		cout << "Faltam argumentos!" << endl;
		return EXIT_FAILURE;
	}*/

	while (true) {
		cout << "> ";
		getline(cin, cmd);

		try {
			parser.parse(cmd);
		} catch (string msg) {
			cout << msg << endl;
		}
	}

	return EXIT_SUCCESS;
}
