#ifndef __FINDCMD_HPP__
#define __FINDCMD_HPP__

#include <parser/comando.hpp>

class FindCmd : public Comando {
public:
    int K;

    FindCmd(int k);
};

#endif
