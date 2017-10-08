#include <parser/quitcmd.hpp>
#include <iostream>
#include <cstdlib>

using namespace std;

void QuitCmd::executar() {
    cout << "Adeus!" << endl;
    exit(EXIT_SUCCESS);
}
