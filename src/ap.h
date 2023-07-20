#pragma once

#include <string>
#include <vector>

namespace ap {
enum ArgCount {
    AC_NONE = 0,
    AC_VARI = -1
};

class Opt {
public:
    Opt(const std::string& nLong, char nShort, int argCount);

    std::string nLong;
    char nShort;
    int argCount;
    std::vector<std::string> args;
    bool set = false;
};

class Parser {
public:
    Parser(int argc, char **argv);
    void Parse();

    std::vector<std::reference_wrapper<const Opt>> opts;
private:
    int argc;
    char **argv;
    std::vector<std::string> args;
};
}
