#ifndef __COMANDO_HPP__
#define __COMANDO_HPP__

class Comando {
public:
    enum Type {
        CMD_NULL,
        CMD_FIND,
        CMD_STORE,
        CMD_LIST,
        CMD_ENTER,
        CMD_QUIT,
        CMD_HELP,
        CMD_MEMBERS
    };
    Type tipo;

    Comando(Type t);
    virtual ~Comando();
    virtual void executar();
};

#endif
