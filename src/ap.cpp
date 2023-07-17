#include "ap.h"

ArgParser::Option::Option(const std::string& longName, char shortName, bool *flag, int argType)
{
  this->longName = longName;
  this->shortName = shortName;
  this->flag = flag;
  this->argType = argType;
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
  for(int i = 1; i < argc; i++) {

  }
}
