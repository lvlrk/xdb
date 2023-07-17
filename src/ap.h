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

    Option(const std::string& longName, char shortName, int argType, std::vector<void*> args = std::vector<void*>());
    bool IsArgTypeSpecial();

    std::string longName = "";
    char shortName = 0;
    bool flag = false;
    int argType = ARG_NONE;
    std::vector<void*> args;
  };

  enum OptionType {
    OPT_SHORT,
    OPT_LONG,
    OPT_NO
  };

  ArgParser(int argc, char **argv);

  void AddOpt(Option& opt);
  void Parse();
  int OptionType(const std::string& arg);
private:
  int argc;
  char **argv;
  std::vector<std::reference_wrapper<Option>> opts;
  std::string program; // argv[0]
};
