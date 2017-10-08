#include <parser/helpcmd.hpp>
#include <iostream>

using namespace std;

void HelpCmd::executar() {
    cout << "Comandos disponíveis: " << endl;
    cout << "STORE(<chave,valor>) - armazena a tupla na rede" << endl;
    cout << "FIND(chave) - procura a tupla identificada por 'chave' na rede" << endl;
    cout << "HELP - exibe texto de ajuda" << endl;
    cout << "QUIT - encerra a aplicação" << endl;
}
