#include <parser/comando.hpp>
#include <iostream>

using namespace std;

Comando::~Comando() {
    //Destrutor virtual
}

void Comando::executar() {
    cout << "Comando ainda não implementado" << endl;
}
