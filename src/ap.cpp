#include <iostream>
#include "ap.h"

ap::Opt::Opt(const std::string& nLong, char nShort, int argCount):
    nLong{nLong},
    nShort{nShort},
argCount{argCount} {
    switch(argCount) {
        case AC_NONE:
            args.resize(0);
            break;
        case AC_VARI: break; // case here; not grouped with default
        default:
            args.resize(argCount);
            break;
    }
}

ap::Parser::Parser(int argc, char **argv):
    argc{argc},
argv{argv} {
    args.resize(argc - 1);

    for(int i = 1; i < argc; i++) args[i - 1] = argv[i];
}

void ap::Parser::Parse() {
    for(const std::string& cArg: args) {
        std::cout << cArg << '\n';
    }
}
