/* Enum of different programs */
#define XDB_P_RELEASE 0
#define XDP_P_READTEST 1

#define XDB_PROGRAM XDB_P_RELEASE // program enum number

#if XDB_PROGRAM == XDB_P_RELEASE
#include "ap.h"       // ArgParser
#include "util.h"     // VERBOSE
#include "xdb.h"      // Xdb
#include <fmt/core.h> // fmt::format
#include <iostream>   // std::cout, std::cerr
#include <string>     // std::string
#include <vector>     // std::vector

class App {
    public:
        App(int argc, char **argv);
        int Run();

    private:
        int Usage();
        int Help();
        int Version();
        int Verbose();
        int File();
        int Create();
        int List();

    private:
        struct Version {
            int max;
            int mid;
            int min;
        };

        /* Arguments-related */
        int argc;
        char **argv;
        ArgParser argParser;

        /* Program information */
        std::string program = "xdb";
        struct Version version = {0, 1, 3};

        /* Option-objects-related */
        ArgParser::Option optHelp;
        ArgParser::Option optVersion;
        ArgParser::Option optVerbose;
        ArgParser::Option optFile;
        ArgParser::Option optCreate;
        ArgParser::Option optList;
        std::vector<std::reference_wrapper<ArgParser::Option>> opts;
};

int App::Usage() {
    std::string text;

    text = fmt::format("Usage: {:s} [-", program);

    for (ArgParser::Option &opt : opts) {
        if (opt.shortName != 0) {
            text += opt.shortName;
        }
    }

    text += fmt::format("]\n       {:s} [--help]\n", program);

    std::cerr << text;

    return 1;
}

int App::Help() {
    std::cout << fmt::format(
            "Usage: {:s} [OPTION...] [FILE]...\n"
            "The best FOSS porn viewer\n\n"

            "Examples:\n"
            "# Verbosely create archive.xdb from image1, image2, and video\n"
            "{:s} -cvf archive.xdb image1.png image2.jpeg video.mp4\n\n"

            "      --help               display this help and exit\n"
            "      --version            output version information and exit\n"
            "  -v, --verbose            verbosely list files processed\n"
            "  -f, --file=FILE          use archive FILE\n"
            "  -c, --create=<FILE(s)>   create archive file from <FILE(s)>\n"
            "  -t, --list               list archive\n\n"

            "Report bugs to https://github.com/lvlrk/xdb/issues\n",
            program, program);

    return 0;
}

int App::Version() {
    std::cout << fmt::format("{:s}-{:d}.{:d}.{:d}", program, version.max,
            version.mid, version.min)
        << std::endl;

    return 0;
}

int App::Verbose() {
    verbose = 1;

    return 0;
}

int App::File() {
    if (optFile.args.size() != optFile.argType) {
        std::cerr << fmt::format("{:s} error: Missing FILE argument for {:s}\n",
                program, optFile.caller);

        return 1;
    }

    return 0;
}

int App::Create() {
    if (optFile.args.size() == optFile.argType) {
        if (optCreate.args.size() > 0) {
            Xdb xdbFile;

            for (const std::string &file : optCreate.args) {
                if (file != optFile.args[0]) {
                    xdbFile.PushBackFilename(file);
                    VERBOSE(file);
                }
            }

            xdbFile.WriteToFile(optFile.args[0]);

            return 0;
        } else {
            std::cerr << fmt::format("{:s} error: Missing FILEs for {:s}\n", program,
                    optCreate.caller);

            return 1;
        }
    } else {
        std::cerr << fmt::format(
                "{:s} error: Missing file for {:s}; call -f to set file\n", program,
                optCreate.caller);

        return 1;
    }

    return 0;
}

int App::List() {
    if (optFile.args.size() == optFile.argType) {
        Xdb xdbFile;

        xdbFile.ReadFromFile(optFile.args[0]);

        for (XdbEntry &entry : xdbFile.GetEntries()) {
            std::cout << entry.filename << '\n';
        }
    } else {
        std::cerr << fmt::format(
                "{:s} error: Missing file for {:s}; call -f to set file\n", program,
                optList.caller);

        return 1;
    }

    return 0;
}

App::App(int argc, char **argv)
    : /* Arguments-related */
        argc{argc}, argv{argv}, argParser(argc, argv),

        /* Option-objects-related */
        optHelp("help", 0, ArgParser::Option::ARG_NONE),
        optVersion("version", 0, ArgParser::Option::ARG_NONE),
        optVerbose("verbose", 'v', ArgParser::Option::ARG_NONE),
        optFile("file", 'f', 1),
        optCreate("create", 'c', ArgParser::Option::ARG_ANY),
        optList("list", 't', ArgParser::Option::ARG_NONE) {

            opts.push_back(optHelp);
            opts.push_back(optVersion);
            opts.push_back(optVerbose);
            opts.push_back(optFile);
            opts.push_back(optCreate);
            opts.push_back(optList);
        }

int App::Run() {
    if (argc < 2) {
        return Usage();
    }

    for (ArgParser::Option &opt : opts)
        argParser.AddOpt(opt);

    argParser.Parse();

    if (optHelp.flag) {
        return Help();
    }
    if (optVersion.flag) {
        return Version();
    }
    if (optVerbose.flag) {
        if (Verbose())
            return 1;
    }
    if (optFile.flag) {
        if (File())
            return 1;
    }
    if (optCreate.flag) {
        if (Create())
            return 1;
    }
    if (optList.flag) {
        if (List())
            return 1;
    }

    return 0;
}

int main(int argc, char **argv) {
    App main(argc, argv);

    return main.Run();
}
#elif XDB_PROGRAM == XDB_P_READTEST /* XDB_P_RELEASE */
#include "xdb.h"

int main(int argc, char **argv) {
    Xdb x;

    x.ReadFromFile("test.xdb");

    return 0;
}
#endif /* XDB_P_READTEST */
