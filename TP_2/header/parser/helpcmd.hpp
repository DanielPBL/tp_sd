#ifndef __HELPCMD_HPP__
#define __HELPCMD_HPP__

#include <parser/comando.hpp>

class HelpCmd : public Comando {
public:
    HelpCmd();
    void executar();
};

#endif
