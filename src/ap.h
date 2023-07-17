#pragma once

#include <string>
#include <vector>

class ArgParser
{
public:
  class Option
  {
  public:
    enum ArgType {
      ARG_NONE = 0,
      ARG_ANY = -1
    };

    Option(const std::string& longName, char shortName, bool *flag, int argType, std::vector<void*> args);
    bool IsArgTypeSpecial();

    std::string longName;
    char shortName;
    bool *flag;
    int argType;
    std::vector<void*> args;
  };
  ArgParser(int argc, char **argv);

  void AddOpt(Option& opt);
  void Parse();
private:
  int argc;
  char **argv;
  std::vector<std::reference_wrapper<Option>> opts;
};
