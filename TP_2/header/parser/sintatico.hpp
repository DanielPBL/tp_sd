#ifndef __SINTATICO_HPP__
#define __SINTATICO_HPP__

#include "lexico.hpp"
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
	void lancaExcessao(std::string aviso);
	void lancaExcessao();

	void procPrograma();
	void procComando();
	void procFind();
	void procStore();
	void procChave();
	void procValor();
};

#endif