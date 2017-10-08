#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include <string>
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
    void parse(std::string cmd);
};

#endif
