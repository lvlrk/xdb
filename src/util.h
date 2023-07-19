#pragma once

#include <iostream> // std::cout
#include <string> // std::string
#include <fstream> // std::ifstream
#include <fmt/core.h> // fmt::format

#ifdef _WIN32
#define DIRSEP "\\"
#else
#define DIRSEP "/"
#endif

extern int verbose;

#define VERBOSE(s) if(verbose == 1) std::cout << fmt::format("{:s}\n", s);

std::string ReadStringFromFile(std::ifstream& inf);
