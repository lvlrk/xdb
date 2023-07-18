#pragma once

#include <string>
#include <vector>

class ArgParser
    {
    public:
        class Option
            {
            public:
                enum ArgType {
                    ARG_NONE = 0,
                    ARG_ANY = -1
                };

                Option(const std::string& longName, char shortName, int argType);
                bool IsArgTypeSpecial();

                std::string longName = "";
                char shortName = 0;
                bool flag = false;
                int argType = ARG_NONE;
                std::vector<std::string> args;
            };

        enum OptionType {
            OPT_SHORT,
            OPT_LONG,
            OPT_NO
        };

        ArgParser(int argc, char **argv);

        void AddOpt(Option& opt);
        void Parse();
    private:
        int OptionType(const std::string& arg);
        void ParseArgs(int i, int j);

        int argc;
        char **argv;
        std::vector<std::reference_wrapper<Option>> opts;
        std::string program; // argv[0]
    };
