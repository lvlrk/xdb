#include <iostream>
#include "ap.h"

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
}

void ArgParser::AddOpt(Option& opt)
{
  opts.push_back(opt);
}

void ArgParser::Parse()
{
  // int zero = 0,
  // one = 1;
  
  // currently debugging dont judge

  bool *f;

  for(int i = 1; i < argc; i++) {
    if(argv[i][0] == '-' && argv[i][1] == '-') // long boi
    {
      for(int j = 0; j < opts.size(); j++)
      {
        if(std::string(argv[i] + 2) == opts[j].get().longName) {
          std::cout << "yessir " << opts[j].get().longName << '\n';
          opts[j].get().flag = true;
        }
        else std::cout << "unrecognized --" << std::string(argv[i] + 2) << '\n';
      }
    }
    else if(argv[i][0] == '-' && argv[i][1] != '-') // shorty
    {
      for(int j = 1; j < std::string(argv[i]).size(); j++)
      {
        for(int k = 0; k < opts.size(); k++) {
          if(argv[i][j] == opts[k].get().shortName) {
            std::cout << "shorty " << opts[k].get().shortName << '\n';
            opts[k].get().flag = true;
          } else {
            std::cout << "unrecognized -" << argv[i][j] << '\n';
          }
        }
        // std::cout << argv[j] << '\n';
      }
    }
    else // arg
    {

    }
  }
}
