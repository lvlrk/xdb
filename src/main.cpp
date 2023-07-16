#define RELEASE
#ifdef RELEASE
#define VERSION_MAX 0
#define VERSION_MIN 1

#include <iostream>
#include <string>
#include <fmt/core.h>

int main(int argc, char **argv)
{
  const std::string usage = "Usage: xdb [-v]\n"
    "\txdb [--help]\n";
  const std::string help = "Usage: xdb [OPTION...] [FILE...]\n"
    "The best FOSS p*rn viewer\n\n"
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

// #define DEBUG
#ifdef DEBUG
#include "xdb.h"

int main(int argc, char **argv)
{
  Xdb x;

  x.PushBackFilename("Makefile");
  x.PushBackFilename("xdb");

  x.WriteToFile("debug.xdb");

  return 0;
}
#endif
