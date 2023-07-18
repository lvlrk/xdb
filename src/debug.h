#pragma once

#include <fmt/core.h>

extern int debug;

#define DEBUG(s) if(debug == 1) fmt::print("{}(): {}\n", __func__, s)
#define DEBUG_S()
