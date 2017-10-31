#ifndef __SINTATICO_HPP__
#define __SINTATICO_HPP__

#include <parser/lexico.hpp>
#include <parser/comando.hpp>
#include <parser/helpcmd.hpp>
#include <parser/findcmd.hpp>
#include <parser/storecmd.hpp>
#include <parser/entercmd.hpp>
#include <string>
#include <utility>
#include <list>

class AnalisadorSintatico {
private:
	AnalisadorLexico& lexico;
	Lexema atual;
public:
	AnalisadorSintatico(AnalisadorLexico& lexico);
	void matchToken(int tipo);
	Comando* init();
	std::list<Comando*> stores();
private:
	void lancaExcessao(std::string msg);
	void lancaExcessao();

	Comando* procPrograma();
	Comando* procComando();
	EnterCmd* procEnter();
	FindCmd* procFind();
	StoreCmd* procStore();
	std::list<Comando*> procStores();
	Comando* procList();
	Comando* procQuit();
	HelpCmd* procHelp();
	std::pair<int, std::string> procPair();
	int procChave();
	std::string procValor();
};

#endif
