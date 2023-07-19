/* Enum of different programs */
#define XDB_P_RELEASE
#define XDP_P_READTEST

// --- leave is as (in commit) until @lvlrk finished new API interface
#define XDB_PROGRAM XDB_P_READTEST // program enum number

#if (XDB_PROGRAM == XDL_P_RELEASE)
#include <iostream> // std::cout, std::cerr
#include <string> // std::string
#include <vector> // std::vector
#include <fmt/core.h> // fmt::format
#include "ap.h" // ArgParser

class App {
public:
    App(int argc, char **argv);
    int Run();
private:
    class Usage {
    public:
        Usage(App& app);
        int Run();
    private:
        App& app;
        std::string text;
    };

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
    struct Version version = {0, 0, 1};

    /* Options-related */
    Usage usage;

    /* Option-objects-related */
    ArgParser::Option optHelp;
    std::vector<std::reference_wrapper<ArgParser::Option>> opts;
};

App::Usage::Usage(App& app):
app{app} {
    text = fmt::format("Usage: {:s}\n", app.program);

    for(ArgParser::Option opt: app.opts) {
        text += fmt::format("\t{:s} [--{:s}]\n", app.program, opt.longName);
    }
}

int App::Usage::Run() {
    std::cerr << text;

    return 1;
}

App::App(int argc, char **argv):
    /* Arguments-related */
    argc{argc},
    argv{argv},
    argParser(argc, argv),

    /* Options-related */
    usage(*this),
    
    /* Option-objects-related */
optHelp("help", 0, ArgParser::Option::ARG_NONE) {
    opts.push_back(optHelp);
}

int App::Run() {
    if(argc < 2) {
        return usage.Run();
    }

    for(ArgParser::Option opt: opts) argParser.AddOpt(opt);

    argParser.Parse();

    return 0;
}

int main(int argc, char **argv) {
    App main(argc, argv);

    return main.Run();
}
#elif (XDB_PROGRAM == XDB_P_READTEST) /* XDB_P_RELEASE */
#include "xdb.h"

int main(int argc, char **argv) {
    // Xdb x;

    // x.ReadFromFile("test.xdb");

    return 0;
}
#endif /* XDB_P_READTEST */
