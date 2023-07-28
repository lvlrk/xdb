#include <iostream>
#include <fmt/core.h>
#include "ap.h"

Opt::Opt(const std::string& nLong, char nShort, int argCount):
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

Parser::Parser(int argc, char **argv):
    argc{argc},
argv{argv} {
    args.resize(argc - 1);

    for(int i = 1; i < argc; i++) args[i - 1] = argv[i];
}

void Parser::Parse() {
    for(int i = 0; i < args.size(); i++) {
        std::string cGetOpt = GetOpt(args[i]);
        int cGetOptType = GetOptType(args[i]);

        for(Opt& cOpt: opts) {
            switch(cGetOptType) {
            case OPT_LONG:
                if(cGetOpt == cOpt.nLong) {
                    cOpt.set = true;

                    ParseOptArgs(cOpt);
                }

                break;
            case OPT_SHORT:
                if(cGetOpt[0] == cOpt.nShort) cOpt.set = true;

                break;
            case OPT_MSHORT:
                for(char c: cGetOpt) {
                    if(c == cOpt.nShort) cOpt.set = true;
                }

                break;
            }
        }
    }
}

int Parser::GetOptType(const std::string& arg) {
    if(arg[0] == '-' && arg[1] == '-') return OPT_LONG;
    else if(arg[0] == '-' && arg[1] != '-') {
        if(arg.size() == 2) return OPT_SHORT;
        else if(arg.size() > 2) return OPT_MSHORT;
    }

    return OPT_NONE;
}

std::string Parser::GetOpt(const std::string& arg, int optType) {
    int myOptType;

    if(optType == -1) myOptType = GetOptType(arg);
    else myOptType = optType;

    switch(myOptType) {
    case OPT_LONG:
        return arg.substr(2);

        break;
    case OPT_SHORT:
    case OPT_MSHORT:
        return arg.substr(1);

        break;
    default:
        return arg;
    }
}

bool Parser::ParseOptArgs(Opt& opt) {
    if(opt.argCount != AC_NONE) {
        if(opt.argCount == AC_VARI) {
            int i = 0; // args iterator
            int j = 0; // option argument iterator

            while(GetOptType(args[i]) == OPT_NONE) {
                opt.args.resize(j + 1);
                opt.args[j] = args[i];

                i++;
                j++;
            }

            if(j < 1) return false;
            else return true;
        } else {
            opt.args.resize(opt.argCount);
            int i = 0;

            for(const std::string& cArg: args) {
                if(GetOptType(cArg) == OPT_NONE) opt.args[i++] = cArg;
            }

            if(i == opt.argCount - 1) return true;
            else return false;
        }
    }

    return true;
}
