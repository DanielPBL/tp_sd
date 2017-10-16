#include <parser/entercmd.hpp>

using namespace std;

EnterCmd::EnterCmd(string addr, string port) : Comando(Comando::CMD_ENTER) {
    this->ip = addr;
    this->porta = port;
}
