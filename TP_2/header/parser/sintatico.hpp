#ifndef __SINTATICO_HPP__
#define __SINTATICO_HPP__

#include <parser/lexico.hpp>
#include <parser/comando.hpp>
#include <parser/quitcmd.hpp>
#include <parser/helpcmd.hpp>
#include <string>

class AnalisadorSintatico {
private:
	AnalisadorLexico& lexico;
	Lexema atual;
public:
	AnalisadorSintatico(AnalisadorLexico& lexico);
	void matchToken(int tipo);
	void init();
private:
	void lancaExcessao(std::string msg);
	void lancaExcessao();

	Comando* procPrograma();
	Comando* procComando();
	Comando* procFind();
	Comando* procStore();
	QuitCmd* procQuit();
	HelpCmd* procHelp();
	void procChave();
	void procValor();
};

#endif
