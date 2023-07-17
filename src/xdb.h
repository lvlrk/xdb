#pragma once

#ifndef _WIN32 // check for penguin os, as it should be
#include <sys/stat.h> // mode_t, time_t

#define XDBSTAT_USE_MODE
#else
#include <ctime> // time_t

typedef mode_t int;
#endif

#include <string> // std::string
#include <vector> // std::vector
#include <memory> // std::unique_ptr

struct XdbStat { // contains 'portable' stat info
  mode_t mode; // will be 0 on NOT PENGUIN OS >:( L windows
  time_t atime;
  time_t mtime;
  time_t ctime;
};

struct XdbEntry { // note: all char* are cstrings;
  // will read from binary until 0
  struct XdbStat stat; // custom stat metadata
  std::string filename;
  std::string name; // generated 'cleaner' filename
  int bufferSize;
  char *buffer; // file data buffer
};

class Xdb {
public:
  /*** Userspace functions ***/
  // pushes filename file onto entries; returns non-zero on error
  int PushBackFilename(const std::string& filename);
  
  // writes file buffer data to a xdb database; returns non-zero on error
  int WriteToFile(const std::string& dbname);

  /*** Utility functions ***/
  // returns xdbstat created from filename; returns nullptr on error
  struct XdbStat *XdbStatFromFilename(const std::string& filename);

  // creates a 'clean' name from filename; returns "" on error
  std::string GenerateName(const std::string& filename);

  // returns entry created from filename; returns nullptr on error
  XdbEntry EntryFromFilename(const std::string& filename);
private:
  std::vector<std::string> tags;
  std::vector<struct XdbEntry> entries;
};
