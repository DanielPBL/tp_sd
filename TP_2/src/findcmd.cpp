#include <parser/findcmd.hpp>

FindCmd::FindCmd(int k) : Comando(Comando::CMD_FIND) {
    this->K = k;
}
