#include <parser/parser.hpp>
#include <string>

using namespace std;

Parser::Parser() {
    this->lexico = new AnalisadorLexico();
    this->sintatico = new AnalisadorSintatico(*lexico);
}

Parser::~Parser() {
    delete this->lexico;
    delete this->sintatico;
}

void Parser::parse(string cmd) {
    this->lexico->setComando(cmd);
    this->sintatico->init();
}
