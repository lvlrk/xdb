#include <iostream>
#include <fmt/core.h>
#include "ap.h"
#include "util.h"

bool ArgParser::Option::IsArgTypeSpecial() {
  return (argType == ARG_NONE || argType == ARG_ANY);
}

ArgParser::Option::Option(const std::string& longName, char shortName, int argType, std::vector<void*> args)
{
  this->longName = longName;
  this->shortName = shortName;
  this->argType = argType;
  this->args = args;

  if(IsArgTypeSpecial()) {

  } else args.resize(argType);
}

ArgParser::ArgParser(int argc, char **argv)
{
  this->argc = argc;
  this->argv = argv;

  if(argv[0][0] == '.' && argv[0][1] == *DIRSEP) program = argv[0] + 2;
  else program = argv[0];
}

void ArgParser::AddOpt(Option& opt)
{
  opts.push_back(opt);
}

void ArgParser::Parse()
{
  char *carg;
  bool unrecognized;
  int tmp;

  for(int i = 1; i < argc; i++) {
    unrecognized = false;
    carg = argv[0];
    tmp = 0;

    if(argv[i][0] == '-' && argv[i][1] == '-') // long boi
    {
      carg = argv[i] + 2;

      for(int j = 0; j < opts.size(); j++)
      {
        if(std::string(carg) == opts[j].get().longName) {
          opts[j].get().flag = true;
          unrecognized = false;
        } else
          unrecognized = true;
      }

      if(unrecognized) {
        std::cerr << fmt::format("{}: unrecognized option --{}\n",
          program, carg);
      }
    }
    else if(argv[i][0] == '-' && argv[i][1] != '-') // shorty
    {
      carg = argv[i] + 1;

      for(int j = 0; j < std::string(carg).size(); j++)
      {
        for(int k = 0; k < opts.size(); k++) {
          if(carg[j] == opts[k].get().shortName) {
            opts[k].get().flag = true;
            unrecognized = false;
        } else {
            unrecognized = true;
            tmp = j;
          }
        }
      }

      if(unrecognized) {
        std::cerr << fmt::format("{}: unrecognized option -{}\n",
          program, carg[tmp]);
      }
    }
    else // arg
    {

    }
  }
}
