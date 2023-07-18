#pragma once

#include <string>
#include <fstream>

#ifdef _WIN32
#define DIRSEP "\\"
#else
#define DIRSEP "/"
#endif

#include <fmt/core.h>

extern int debug;
extern int hardDebug;

#define DEBUG(s) if(debug) fmt::print("{}(): {}\n", __func__, s)
#define HDEBUG(s) if(hardDebug) fmt::print("{}() +{}.{}: {}\n", __func__, __FILE__, __LINE__, s);

std::string ReadStringFromFile(std::ifstream& inf);
