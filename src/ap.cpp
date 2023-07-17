#include <iostream>
#include <fmt/core.h>
#include "ap.h"
#include "util.h"

int ArgParser::OptionType(const std::string& arg) {
  int ret = OPT_NO;

  if(arg[0] == '-' && arg[1] != '-') {
    ret = OPT_SHORT;
  } else if(arg[0] == '-' && arg[1] == '-') {
    ret = OPT_LONG;
  }

  return ret;
}

bool ArgParser::Option::IsArgTypeSpecial() {
  return (argType == ARG_NONE || argType == ARG_ANY);
}

ArgParser::Option::Option(const std::string& longName, char shortName, int argType, std::vector<void*> args)
{
  this->longName = longName;
  this->shortName = shortName;
  this->argType = argType;
  this->args = args;

  if(!IsArgTypeSpecial())
    this->args.resize(argType);
}

ArgParser::ArgParser(int argc, char **argv)
{
  this->argc = argc;
  this->argv = argv;

  if(argv[0][0] == '.' && argv[0][1] == *DIRSEP) program = argv[0] + 2; // remove './' from argv[0]
  else program = argv[0];
}

void ArgParser::AddOpt(Option& opt)
{
  opts.push_back(opt);
}

void ArgParser::Parse()
{
  char *carg;
  char copt; // used for short options
  bool unrecognized = false;
  int tmp;
  int optType;

  for(int i = 1; i < argc; i++) {
    // resetting the 'temporary' variables
    unrecognized = true;
    carg = argv[0];
    tmp = 0;

    optType = OptionType(argv[i]);
    switch(optType)
    {
    case OPT_LONG:
      carg = argv[i] + 2; // remove '--' from argv[i]

      for(int j = 0; j < opts.size(); j++)
      {
        if(std::string(carg) == opts[j].get().longName) {
          opts[j].get().flag = true;
          unrecognized = false;

          if(opts[j].get().argType != Option::ARG_NONE) {
            
          }
        }
      }

      if(unrecognized) {
        std::cerr << fmt::format("{}: unrecognized option --{}\n",
          program, carg);
      }

      break;
    case OPT_SHORT:
      carg = argv[i] + 1; // remove '-' from argv[i]
      copt = 0;

      for(int j = 0; j < std::string(carg).size(); j++)
      {
        copt = carg[j];

        for(int k = 0; k < opts.size(); k++) {
          if(copt == opts[k].get().shortName) {
            opts[k].get().flag = true;
            unrecognized = false;
          } else {
            tmp = j;
          }
        }
      }

      if(unrecognized) {
        std::cerr << fmt::format("{}: unrecognized option -{}\n",
          program, carg[tmp]);
      }

      break;
    default:
      break;
    }
  }
}
