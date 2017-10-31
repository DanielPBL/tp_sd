#include <parser/helpcmd.hpp>
#include <parser/comando.hpp>
#include <iostream>

using namespace std;

HelpCmd::HelpCmd() : Comando(Comando::CMD_HELP) {
}

void HelpCmd::executar() {
    cout << "Comandos disponíveis: " << endl;
    cout << "ENTER \"IP\" PORTA - entra na rede P2P do peer informado" << endl;
    cout << "STORE(<K,\"V\">) - armazena a tupla na rede" << endl;
    cout << "FIND(K) - busca a tupla identificada por K na rede" << endl;
    cout << "LIST - lista os peers vizinhos e as tuplas armazenadas neste peer" << endl;
    cout << "HELP - exibe texto de ajuda" << endl;
    cout << "QUIT - encerra a aplicação" << endl;
}
