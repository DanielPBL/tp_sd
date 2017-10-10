#include <parser/sintatico.hpp>
#include <cstdlib>
#include <cstdio>

using namespace std;

AnalisadorSintatico::AnalisadorSintatico(AnalisadorLexico& lexico) : lexico(lexico) {
}

void AnalisadorSintatico::matchToken(int tipo) {
	if (this->atual.tipo == tipo) {
		this->atual = this->lexico.getLexema();
	} else {
		this->lancaExcessao();
	}
}

void AnalisadorSintatico::lancaExcessao(std::string msg) {
	msg += "\nDigite 'help' para mais informações";
	throw msg;
}

void AnalisadorSintatico::lancaExcessao() {
	switch (this->atual.tipo) {
	case TOKEN_INVALIDO:
		this->lancaExcessao("Token inválido [" + this->atual.token + "]");
		break;
	case FIM_COMANDO_INESPERADO:
	case FIM_COMANDO_NORMAL:
		this->lancaExcessao("Fim de comando inesperado");
		break;
	default:
		this->lancaExcessao("Token não esperado [" + this->atual.token + "]");
	}
}

void AnalisadorSintatico::init() {
	Comando *cmd;

	this->atual = lexico.getLexema();
	cmd = this->procPrograma();
	cmd->executar();
	delete cmd;
}

Comando* AnalisadorSintatico::procPrograma() {
	Comando *cmd;

	cmd = this->procComando();
	this->matchToken(FIM_COMANDO_NORMAL);

	return cmd;
}

Comando* AnalisadorSintatico::procComando() {
	Comando *cmd;

	switch (this->atual.tipo) {
	case SERVER:
		cmd = this->procServer();
		break;
	case FIND:
		cmd = this->procFind();
		break;
	case STORE:
		cmd = this->procStore();
		break;
	case LIST:
		cmd = this->procList();
		break;
	case QUIT:
		cmd = this->procQuit();
		break;
	case HELP:
		cmd = this->procHelp();
		break;
	default:
		cmd = new Comando();
		this->lancaExcessao();
	}

	return cmd;
}

Comando* AnalisadorSintatico::procServer() {
	this->matchToken(SERVER);
	this->procValor();
	this->procChave();

	return new Comando();
}

Comando* AnalisadorSintatico::procFind() {
	this->matchToken(FIND);
	this->matchToken(ABRE_PRNTS);
	this->procChave();
	this->matchToken(FECHA_PRNTS);

	return new Comando();
}

Comando* AnalisadorSintatico::procStore() {
	this->matchToken(STORE);
	this->matchToken(ABRE_PRNTS);
	this->procPair();
	this->matchToken(FECHA_PRNTS);

	return new Comando();
}

Comando* AnalisadorSintatico::procList() {
	this->matchToken(LIST);

	return new Comando();
}

QuitCmd* AnalisadorSintatico::procQuit() {
	this->matchToken(QUIT);

	return new QuitCmd();
}

HelpCmd* AnalisadorSintatico::procHelp() {
	this->matchToken(HELP);

	return new HelpCmd();
}
void AnalisadorSintatico::procPair() {
	this->matchToken(MENOR);
	this->procChave();
	this->matchToken(VIRGULA);
	this->procValor();
	this->matchToken(MAIOR);
}

void AnalisadorSintatico::procChave() {
	switch (this->atual.tipo) {
	case INTEIRO:
		this->matchToken(INTEIRO);
		break;
	default:
		this->lancaExcessao();
	}
}

void AnalisadorSintatico::procValor() {
	switch (this->atual.tipo) {
	case LITERAL:
		this->matchToken(LITERAL);
		break;
	default:
		this->lancaExcessao();
	}
}
