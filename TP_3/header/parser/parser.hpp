#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <string>
#include <list>
#include <parser/lexico.hpp>
#include <parser/sintatico.hpp>
#include <parser/comando.hpp>

class Parser {
private:
    AnalisadorLexico *lexico;
    AnalisadorSintatico *sintatico;
public:
    Parser();
    ~Parser();
    Comando* parse(std::string cmd);
    std::list<Comando*> parseStores(std::string cmd);
    std::list<Comando*> parseEnters(std::string cmd);
};

#endif
