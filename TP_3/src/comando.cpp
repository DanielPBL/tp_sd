#include <parser/comando.hpp>
#include <iostream>

using namespace std;

Comando::Comando(Type t) : tipo(t) {
}

Comando::~Comando() {
    //Destrutor virtual
}

void Comando::executar() {
    cout << "Comando ainda não implementado" << endl;
}
