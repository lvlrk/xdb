#define P_XDB 0
#define P_READTEST 1

#define PROG P_XDB

#if PROG == P_XDB
#define VERSION_MAX 0
#define VERSION_MID 4
#define VERSION_MIN 1

#include <iostream>
#include <string>
#include <fmt/core.h>
#include <vector>
#include "ap.h"
#include "xdb.h"
#include "util.h"

int main(int argc, char **argv)
{
    const std::string usage = "Usage: xdb [-vfct]\n"
        "\txdb -f FILE\n"
        "\txdb [--help]\n";
    const std::string help = "Usage: xdb [OPTION...] [FILE...]\n"
        "The best FOSS viewer\n\n"
        "      --help           display this help and exit\n"
        "  -v, --version        output version information and exit\n"
        "  -f, --file FILE      set operation file\n"
        "  -c, --create         create a xdb database\n"
        "  -t, --list           list files in xdb database\n\n"
        "Report bugs to https://github.com/lvlrk/xdb/issues\n";

    if(argc < 2) {
        std::cout << usage;

        return 1;
    }

    std::string fileName = "";

    ArgParser parser(argc, argv);

    ArgParser::Option helpOpt("help", 0, ArgParser::Option::ARG_NONE);
    ArgParser::Option versionOpt("version", 'v', ArgParser::Option::ARG_NONE);
    ArgParser::Option fileOpt("file", 'f', 1);
    ArgParser::Option createOpt("create", 'c', ArgParser::Option::ARG_ANY);
    ArgParser::Option debugOpt("debug", 0, ArgParser::Option::ARG_NONE);
    ArgParser::Option listOpt("list", 't', ArgParser::Option::ARG_NONE);

    parser.AddOpt(helpOpt);
    parser.AddOpt(versionOpt);
    parser.AddOpt(fileOpt);
    parser.AddOpt(createOpt);
    parser.AddOpt(debugOpt);
    parser.AddOpt(listOpt);

    parser.Parse();

    if(fileOpt.flag) {
        if(fileOpt.args.size() != fileOpt.argType) {
            std::cout << "xdb error: Missing FILE argument for -f\n";
            return 1;
        } else {
            fileName = fileOpt.args[fileOpt.argType - 1];
        }
    }
    if(debugOpt.flag) {
        debug = 1;
    }
    if(helpOpt.flag) {
        std::cout << help;

        return 0;
    }
    if(versionOpt.flag) {
        fmt::print("xdb-{}.{}.{}\n", VERSION_MAX, VERSION_MID, VERSION_MIN);

        return 0;
    }
    if(createOpt.flag) {
        if(fileName != "") {
            Xdb x;

            for(int i = 0; i < createOpt.args.size(); i++) {
                if(createOpt.args[i] != fileName) {
                    x.PushBackFilename(createOpt.args[i]);
                }
            }

            x.WriteToFile(fileName);
        } else {
            std::cout << "xdb error: Missing FILE for -c\n";
            return 1;
        }
    }
    if(listOpt.flag) {
        if(fileName != "") {
            Xdb x;

            x.ReadFromFile(fileName);
        } else {
            std::cout << "xdb error: Missing FILE for -t\n";
            return 1;
        }
    }

    return 0;
}
#elif PROG == P_READTEST
#include "xdb.h"

int main() {
    Xdb x;

    x.ReadFromFile("test.xdb");

    return 0;
}
#endif
