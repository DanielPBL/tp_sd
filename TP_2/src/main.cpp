#include <parser/lexico.hpp>
#include <parser/sintatico.hpp>
#include <iostream>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

using namespace std;

int main(int argc, char **argv) {
	if (argc < 2) {
		cout << "Faltam argumentos!" << endl;
		return EXIT_FAILURE;
	}

	try {
		AnalisadorLexico lexico(argv[1]);
		AnalisadorSintatico sintatico(lexico);

		sintatico.init();
	} catch (string msg) {
		cout << msg << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
