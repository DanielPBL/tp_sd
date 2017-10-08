#include <parser/sintatico.hpp>
#include <cstdlib>
#include <cstdio>

using namespace std;

AnalisadorSintatico::AnalisadorSintatico(AnalisadorLexico& lexico) : lexico(lexico) {
	this->atual = lexico.getLexema();
}

void AnalisadorSintatico::matchToken(int tipo) {
	if (this->atual.tipo == tipo) {
		this->atual = this->lexico.getLexema();
	} else {
		switch (this->atual.tipo) {
		case TOKEN_INVALIDO:
			this->lancaExcessao("Token invalido [" + this->atual.token + "]");
			break;
		case FIM_COMANDO_INESPERADO:
		case FIM_COMANDO_NORMAL:
			this->lancaExcessao("Fim de comando inesperado");
			break;
		default:
			this->lancaExcessao("Token nao esperado [" + this->atual.token + "]");
		}
	}
}

void AnalisadorSintatico::lancaExcessao(std::string aviso) {
	char col[10];
	string msg = "";

	sprintf(col, "%02d", this->lexico.getColuna());
	string coluna(col);
	msg = coluna + ": " + aviso;

	throw msg;
}

void AnalisadorSintatico::lancaExcessao() {
	this->lancaExcessao("Token nao esperado [" + this->atual.token + "]");
}

void AnalisadorSintatico::init() {
	this->procPrograma();
}

void AnalisadorSintatico::procPrograma() {
	this->procComando();
	this->matchToken(FIM_COMANDO_NORMAL);
}

void AnalisadorSintatico::procComando() {
	switch (this->atual.tipo) {
	case FIND:
		this->procFind();
		break;
	case STORE:
		this->procStore();
		break;
	default:
		this->lancaExcessao();
	}
}

void AnalisadorSintatico::procFind() {
	this->matchToken(FIND);
	this->matchToken(ABRE_PRNTS);
	this->procChave();
	this->matchToken(FECHA_PRNTS);
}

void AnalisadorSintatico::procStore() {
	this->matchToken(STORE);
	this->matchToken(ABRE_PRNTS);
	this->matchToken(MENOR);
	this->procChave();
	this->matchToken(VIRGULA);
	this->procValor();
	this->matchToken(MAIOR);
	this->matchToken(FECHA_PRNTS);
}

void AnalisadorSintatico::procChave() {
	switch (this->atual.tipo) {
	case INTEIRO:
		this->matchToken(INTEIRO);
		break;
	case LITERAL:
		this->matchToken(LITERAL);
		break;
	default:
		this->lancaExcessao();
	}
}

void AnalisadorSintatico::procValor() {
	switch (this->atual.tipo) {
		this->matchToken(LITERAL);
		break;
	default:
		this->lancaExcessao();
	}
}
