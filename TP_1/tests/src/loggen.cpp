#include <ctime>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>

using namespace std;

int main(int argc, char const *argv[]) {
    ofstream arq;
    string str;
    stringstream ss;
    unsigned int filesize, i;
    time_t _tm;
    struct tm *curtime;

    if (argc != 3) {
        cerr << "Utilização: loggen.out SERVER_ID TAMANHO_MB" << endl;
        exit(1);
    }

    str = string("maquina.") + argv[1] + ".log";
    filesize = atoi(argv[2]);
    arq.open(str.c_str());

    for (i = 1; ss.tellp() < filesize * 1024 * 1024; ++i) {
        _tm = time(NULL);
        curtime = localtime(&_tm);
        str = asctime(curtime);
        ss << "[" << str.substr(0, str.length() - 1) << "] Linha " << i << ": ";
        ss << "Lorem ipsum dolor sit amet, consectetur adipiscing elit." << endl;
    }

    arq << ss.str();
    arq.close();

    return 0;
}
