#include <parser/lexico.hpp>
#include <string>
#include <locale>
#include <sstream>
#include <map>

using namespace std;

AnalisadorLexico::AnalisadorLexico() {
	this->comando = new stringstream();
	this->criarTabelaSimbolos();
}

AnalisadorLexico::AnalisadorLexico(string cmd) {
	this->comando = new stringstream(cmd + "\n");
	this->criarTabelaSimbolos();
}

AnalisadorLexico::~AnalisadorLexico() {
	delete this->comando;
}

void AnalisadorLexico::setComando(string cmd) {
	delete this->comando;
	this->comando = new stringstream(cmd + "\n");
}

bool AnalisadorLexico::isDigit(char c) {
	int ch = (int)c;

	return ch >= 48 && ch <= 57;
}

bool AnalisadorLexico::isLetter(char c) {
	int ch = (int)c;

	return (ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122);
}

bool AnalisadorLexico::isSimbolo(char c) {
	return c == '(' || c == ')' || c == '<' || c == '>' || c == ',';
}

bool AnalisadorLexico::isBlank(char c) {
	return c == '\n' || c == '\r' || c == '\t' || c == ' ';
}

int AnalisadorLexico::consultarTabelaSimbolos(string token) {
	locale loc;
	unsigned int i;

	for (i = 0; i < token.length(); i++) {
		token[i] = tolower(token[i], loc);
	}

	if (this->tabelaSimbolos.find(token) != this->tabelaSimbolos.end()) {
		return this->tabelaSimbolos[token];
	}

	return TOKEN_INVALIDO;
}

void AnalisadorLexico::criarTabelaSimbolos() {
	this->tabelaSimbolos["find"]  = FIND;
	this->tabelaSimbolos["store"] = STORE;
	this->tabelaSimbolos["("]     = ABRE_PRNTS;
	this->tabelaSimbolos[")"]     = FECHA_PRNTS;
	this->tabelaSimbolos[">"]     = MAIOR;
	this->tabelaSimbolos["<"]     = MENOR;
	this->tabelaSimbolos[","]     = VIRGULA;
	this->tabelaSimbolos["quit"]  = QUIT;
	this->tabelaSimbolos["help"]  = HELP;
}

void AnalisadorLexico::ungetChar() {
	this->comando->unget();
}

Lexema AnalisadorLexico::getLexema() {
	Lexema lexema;
	char ch = -1;
	int estado;

	lexema.tipo = FIM_COMANDO_NORMAL;
	lexema.token = "";

	estado = 1;
	while (estado != 5) {
		*(this->comando) >> noskipws >> ch;

		if (this->comando->eof() || this->comando->fail()) {
			if (estado != 1) {
				lexema.tipo = FIM_COMANDO_INESPERADO;
			}
			break;
		}

		switch (estado) {
		case 1:
			if (AnalisadorLexico::isBlank(ch)) {
				continue;
			} else if (AnalisadorLexico::isSimbolo(ch)) {
				lexema.token += ch;
				lexema.tipo = this->consultarTabelaSimbolos(lexema.token);
				estado = 5;
			} else if (AnalisadorLexico::isLetter(ch)) {
				lexema.token += ch;
				estado = 2;
			} else if (AnalisadorLexico::isDigit(ch)) {
				lexema.token += ch;
				estado = 3;
			} else if (ch == '"') {
				estado = 4;
			} else {
				lexema.token += ch;
				lexema.tipo = TOKEN_INVALIDO;
				estado = 5;
			}
			break;
		case 2:
			if (AnalisadorLexico::isLetter(ch)) {
				lexema.token += ch;
			} else {
				lexema.tipo = this->consultarTabelaSimbolos(lexema.token);
				this->ungetChar();
				estado = 5;
			}
			break;
		case 3:
			if (AnalisadorLexico::isDigit(ch)) {
				lexema.token += ch;
			} else {
				lexema.tipo = INTEIRO;
				this->ungetChar();
				estado = 5;
			}
			break;
		case 4:
			if (ch != '"') {
				lexema.token += ch;
			} else {
				lexema.tipo = LITERAL;
				estado = 5;
			}
			break;
		}
	}

	return lexema;
}
