#pragma once

#ifdef _WIN32
#define DIRSEP "\\"
#else
#define DIRSEP "/"
#endif

#include <fmt/core.h>

extern int debug;

#define DEBUG(s) if(debug) fmt::print("{}(): {}\n", __func__, s)
