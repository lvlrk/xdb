#include <iostream>
#include <fmt/core.h>
#include "ap.h"
#include "util.h"

int ArgParser::OptionType(const std::string& arg) {
    int ret = OPT_NO;
    if(arg != "") {
        if(arg[0] == '-' && arg[1] != '-') {
            ret = OPT_SHORT;
        } else if(arg[0] == '-' && arg[1] == '-') {
            ret = OPT_LONG;
        }
    }

    return ret;
}

bool ArgParser::Option::IsArgTypeSpecial() {
    return (argType == ARG_NONE || argType == ARG_ANY);
}

ArgParser::Option::Option(const std::string& longName, char shortName, int argType)
{
    this->longName = longName;
    this->shortName = shortName;
    this->argType = argType;

    if(!IsArgTypeSpecial())
        args.resize(argType);
}

ArgParser::ArgParser(int argc, char **argv)
{
    this->argc = argc;
    this->argv = argv;

    if(argv[0][0] == '.' && argv[0][1] == *DIRSEP) program = argv[0] + 2; // remove './' from argv[0]
    else program = argv[0];
}

void ArgParser::AddOpt(Option& opt)
{
    opts.push_back(opt);
}

void ArgParser::ParseArgs(int i, int j) {
    if(opts[j].get().argType != Option::ARG_ANY) {
        int l = 0;
        for(int k = 0; k < opts[j].get().argType; k++) {
            if(argv[i + k + 1]) {
                opts[j].get().args[l++] = std::string(argv[i + k + 1]);
            } else {
                opts[j].get().args.push_back("");
            }
        }
    } else if(opts[j].get().argType == Option::ARG_ANY) {
        int k = 1;

        while(argv[i + k]) {
            if(OptionType(argv[i + k]) == OPT_NO) {
                opts[j].get().args.push_back(argv[i + k]);
            } else {
                break;
            }

            k++;
        }
    }
}

void ArgParser::Parse()
{
    char *carg;
    char *arga = nullptr;
    std::string sexy;
    char copt; // used for short options
    bool unrecognized = false;
    int tmp;
    int optType;
    int idx;
    int tmp2;
    std::string scarg;
    std::string scargo;

    for(int i = 1; i < argc; i++) {
        // resetting the 'temporary' variables
        unrecognized = true;
        carg = argv[i];
        tmp = 0;
        arga = nullptr;
        tmp2 = 0;

        optType = OptionType(argv[i]);
        switch(optType)
        {
            case OPT_LONG:
                carg = argv[i] + 2; // remove '--' from argv[i]

                for(int j = 0; j < opts.size(); j++)
                {
                    idx = std::string(carg).find('=');

                    if(idx != std::string::npos) {
                        scarg = carg;
                        scargo = scarg;

                        arga = const_cast<char*>(scargo.substr(idx + 1).c_str());
                        sexy = arga;

                        carg = const_cast<char*>(scargo.erase(idx).c_str());
                    }

                    if(std::string(carg) == opts[j].get().longName) {
                        opts[j].get().flag = true;
                        opts[j].get().caller = fmt::format("--{:s}", carg);

                        if(idx == std::string::npos) {
                            tmp2 = opts[j].get().argType;
                            opts[j].get().args.resize(tmp2);

                            opts[j].get().args[tmp2 - 1] = sexy;
                        }
                        unrecognized = false;

                        if(opts[j].get().argType != Option::ARG_NONE && idx != std::string::npos) {
                            ParseArgs(i, j);
                        }
                    }
                }

                if(unrecognized && arga == nullptr) {
                    std::cerr << fmt::format("{}: unrecognized option --{}\n",
                                             program, carg);
                }

                break;
            case OPT_SHORT:
                carg = argv[i] + 1; // remove '-' from argv[i]
                copt = 0;

                for(int j = 0; j < std::string(carg).size(); j++)
                {
                    copt = carg[j];

                    for(int k = 0; k < opts.size(); k++) {
                        if(copt == opts[k].get().shortName) {
                            opts[k].get().flag = true;
                            opts[k].get().caller = fmt::format("-{:c}", copt);
                            unrecognized = false;

                            if(opts[k].get().argType != Option::ARG_NONE) {
                                ParseArgs(i, k);
                            }
                        } else {
                            tmp = j;
                        }
                    }
                }

                if(unrecognized) {
                    std::cerr << fmt::format("{}: unrecognized option -{}\n",
                                             program, carg[tmp]);
                }

                break;
        }
    }
}
