#ifndef __ENTERCMD_HPP__
#define __ENTERCMD_HPP__

#include <parser/comando.hpp>
#include <string>

class EnterCmd : public Comando {
public:
    std::string ip;
    std::string porta;

    EnterCmd(std::string addr, std::string port);
};

#endif
