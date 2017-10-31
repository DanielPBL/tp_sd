#include <parser/sintatico.hpp>
#include <message.hpp>
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

Comando* AnalisadorSintatico::init() {
	this->atual = lexico.getLexema();
	return this->procPrograma();
}


std::list<Comando*> AnalisadorSintatico::stores() {
	this->atual = lexico.getLexema();
	return this->procStores();
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
	case ENTER:
		cmd = this->procEnter();
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
		cmd = new Comando(Comando::CMD_NULL);
		this->lancaExcessao();
	}

	return cmd;
}

EnterCmd* AnalisadorSintatico::procEnter() {
	this->matchToken(ENTER);
	string ip = this->procValor();
	string porta = SSTR(this->procChave());

	return new EnterCmd(ip, porta);
}

FindCmd* AnalisadorSintatico::procFind() {
	this->matchToken(FIND);
	this->matchToken(ABRE_PRNTS);
	int K = this->procChave();
	this->matchToken(FECHA_PRNTS);

	return new FindCmd(K);
}

list<Comando*> AnalisadorSintatico::procStores() {
		list<Comando*> stores;

		while (this->atual.tipo != FIM_COMANDO_NORMAL) {
			stores.push_back(this->procStore());
		}

		return stores;
}

StoreCmd* AnalisadorSintatico::procStore() {
	this->matchToken(STORE);
	this->matchToken(ABRE_PRNTS);
	pair<int, string> pair = this->procPair();
	this->matchToken(FECHA_PRNTS);

	return new StoreCmd(pair);
}

Comando* AnalisadorSintatico::procList() {
	this->matchToken(LIST);

	return new Comando(Comando::CMD_LIST);
}

Comando* AnalisadorSintatico::procQuit() {
	this->matchToken(QUIT);

	return new Comando(Comando::CMD_QUIT);
}

HelpCmd* AnalisadorSintatico::procHelp() {
	this->matchToken(HELP);

	return new HelpCmd();
}

pair<int, string> AnalisadorSintatico::procPair() {
	this->matchToken(MENOR);
	int K = this->procChave();
	this->matchToken(VIRGULA);
	string V = this->procValor();
	this->matchToken(MAIOR);

	return make_pair(K, V);
}

int AnalisadorSintatico::procChave() {
	int ret = -1;

	switch (this->atual.tipo) {
	case INTEIRO:
		ret = atoi(this->atual.token.c_str());
		this->matchToken(INTEIRO);
		break;
	default:
		this->lancaExcessao();
	}

	return ret;
}

string AnalisadorSintatico::procValor() {
	string ret;

	switch (this->atual.tipo) {
	case LITERAL:
		ret = this->atual.token;
		this->matchToken(LITERAL);
		break;
	default:
		this->lancaExcessao();
	}

	return ret;
}
