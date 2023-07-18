#include "util.h"

int debug = 0;
int hardDebug = 0;

std::string ReadStringFromFile(std::ifstream& inf) {
    std::string tmp = "";
    char c = 1;

    while(inf.read(&c, 1) && c != 0) tmp += c;

    return tmp;
}
