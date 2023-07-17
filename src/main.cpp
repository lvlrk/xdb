#define P_XDB 0
#define P_DEBUG 1
#define P_APTEST 2

#define PROG P_XDB

#if PROG == P_XDB
#define VERSION_MAX 0
#define VERSION_MIN 3

#include <iostream>
#include <string>
#include <fmt/core.h>
#include <vector>
#include "ap.h"

// arg parser idea
// {
//  ArgParser parser(argc, argv);
//
//  parser.AddOpt("help", 0, &help_flag, ARGS_NONE);
//  parser.AddOpt("version", 'v', &version_flag, ARGS_NONE);
//  parser.AddOpt("file", 'f', &file_flag, 1, &file_name);
//
//  parser.Parse();
// }

int main(int argc, char **argv)
{
  const std::string usage = "Usage: xdb [-v]\n"
    "\txdb [--help]\n";
  const std::string help = "Usage: xdb [OPTION...] [FILE...]\n"
    "The best FOSS viewer\n\n"
    "      --help      display this help and exit\n"
    "  -v, --version   output version information and exit\n\n"
    "Report bugs to https://github.com/lvlrk/xdb/issues\n";

  if(argc < 2) {
    std::cerr << usage;

    return 1;
  }

  ArgParser parser(argc, argv);

  ArgParser::Option helpOpt("help", 0, ArgParser::Option::ARG_NONE);
  ArgParser::Option versionOpt("version", 'v', ArgParser::Option::ARG_NONE);
  ArgParser::Option testOpt("test", 't', ArgParser::Option::ARG_NONE);

  parser.AddOpt(helpOpt);
  parser.AddOpt(versionOpt);
  parser.AddOpt(testOpt);

  parser.Parse();

  if(helpOpt.flag) {
    std::cout << help;

    return 0;
  }
  else if(versionOpt.flag) {
    fmt::print("xdb-{}.{}\n", VERSION_MAX, VERSION_MIN);

    return 0;
  }

  return 0;
}
#elif PROG == P_DEBUG
#include "xdb.h"

int main(int argc, char **argv)
{
  Xdb x;

  x.PushBackFilename("Makefile");
  // x.PushBackFilename("xdb");

  x.WriteToFile("debug.xdb");

  return 0;
}
#elif PROG == P_APTEST
#include <iostream>
#include "ap.h"

int main(int argc, char **argv) {
  ArgParser parser(argc, argv);

  ArgParser::Option testOpt("test", 't', ArgParser::Option::ARG_NONE, std::vector<void*>());

  parser.AddOpt(testOpt);

  parser.Parse();

  if(testOpt.flag) {
    std::cout << "yo test flag passed\n";
  }

  return 0;
}
#endif
