#ifndef __STORECMD_HPP__
#define __STORECMD_HPP__

#include <parser/comando.hpp>
#include <string>
#include <utility>

class StoreCmd : public Comando {
public:
    StoreCmd(std::pair<int, std::string> pair);
    std::pair<int, std::string> tupla;
};

#endif
