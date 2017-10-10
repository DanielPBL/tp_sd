#ifndef __LEXICO_HPP__
#define __LEXICO_HPP__

#include <sstream>
#include <string>
#include <map>

#define FIM_COMANDO_INESPERADO -2
#define TOKEN_INVALIDO         -1
#define FIM_COMANDO_NORMAL      0
#define FIND                    1
#define STORE                   2
#define LITERAL                 3
#define INTEIRO                 4
#define ABRE_PRNTS              5
#define FECHA_PRNTS             6
#define MAIOR                   7
#define MENOR                   8
#define VIRGULA                 9
#define QUIT                    10
#define HELP                    11
#define SERVER                  12
#define LIST                    13

typedef struct LEXEMA {
	std::string token;
	int tipo;
} Lexema;

class AnalisadorLexico {
private:
	std::stringstream *comando;
	std::map<std::string, int> tabelaSimbolos;

public:
	AnalisadorLexico();
	AnalisadorLexico(std::string cmd);
	~AnalisadorLexico();
	Lexema getLexema();
	void setComando(std::string cmd);

private:
	int consultarTabelaSimbolos(std::string token);
	void criarTabelaSimbolos();
	void ungetChar();
	static bool isDigit(char c);
	static bool isLetter(char c);
	static bool isBlank(char c);
	static bool isSimbolo(char c);
};

#endif
