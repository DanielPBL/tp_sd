#include <parser/findcmd.hpp>

FindCmd::FindCmd(unsigned int k) : Comando(Comando::CMD_FIND) {
    this->K = k;
}
