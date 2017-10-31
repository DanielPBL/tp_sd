#ifndef __FINDCMD_HPP__
#define __FINDCMD_HPP__

#include <parser/comando.hpp>

class FindCmd : public Comando {
public:
    unsigned int K;

    FindCmd(unsigned int k);
};

#endif
