#include <parser/storecmd.hpp>
#include <cstdlib>
#include <string>

using namespace std;

StoreCmd::StoreCmd(std::pair<unsigned int, std::string> pair) : Comando(Comando::CMD_STORE) {
    this->tupla = pair;
}
