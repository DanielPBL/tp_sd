#ifndef __QUITCMD_HPP__
#define __QUITCMD_HPP__

#include <parser/comando.hpp>

class QuitCmd : public Comando {
public:
    void executar();
};

#endif
