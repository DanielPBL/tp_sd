#include <parser/parser.hpp>
#include <string>
#include <list>

using namespace std;

Parser::Parser() {
    this->lexico = new AnalisadorLexico();
    this->sintatico = new AnalisadorSintatico(*lexico);
}

Parser::~Parser() {
    delete this->lexico;
    delete this->sintatico;
}

Comando* Parser::parse(string cmd) {
    this->lexico->setComando(cmd);
    return this->sintatico->init();
}

list<Comando*> Parser::parseStores(string cmd) {
    this->lexico->setComando(cmd);
    return this->sintatico->stores();
}
