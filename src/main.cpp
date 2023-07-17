#define RELEASE // define or undefine to toggle
#ifdef RELEASE
#define VERSION_MAX 0
#define VERSION_MIN 3

#include <iostream>
#include <string>
#include <fmt/core.h>
#include <vector>

// arg parser idea
// {
//  ArgParser parser(argc, argv);
//
//  parser.AddOpt("help", 0, &help_flag, ARGS_NONE);
//  parser.AddOpt("version", 'v', &version_flag, ARGS_NONE);
//  parser.AddOpt("file", 'f', &file_flag, 1, &file_name);
// }

int main(int argc, char **argv)
{
  const std::string usage = "Usage: xdb [-vg]\n"
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

  std::string cOpt;

  for(int i = 1; i < argc; i++) {
    cOpt = std::string(argv[i]);

    if(cOpt == "--help") {
      std::cout << help;

      return 0;
    }
    if(cOpt == "-v" || cOpt == "--version") {
      std::cout << fmt::format("xdb-{}.{}\n", VERSION_MAX, VERSION_MIN);

      return 0;
    }
  }

  return 0;
}
#endif

#ifndef RELEASE
#include "xdb.h"

int main(int argc, char **argv)
{
  Xdb x;

  x.PushBackFilename("Makefile");
  // x.PushBackFilename("xdb");

  x.WriteToFile("debug.xdb");

  return 0;
}
#endif
