#ifndef __STORECMD_HPP__
#define __STORECMD_HPP__

#include <parser/comando.hpp>
#include <string>
#include <utility>

class StoreCmd : public Comando {
public:
    std::pair<unsigned int, std::string> tupla;

    StoreCmd(std::pair<unsigned int, std::string> pair);
};

#endif
