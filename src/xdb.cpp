#include <raylib.h> // GetDirectoryPath
#include <iostream>
#include <fstream> // std::ifstream
#include <cstring> // std::strlen (im sorry avecp)
#include "xdb.h"

XdbStat_t *Xdb::XdbStatFromFilename(const std::string& filename) {
  if(filename == "") return nullptr;

  XdbStat_t *xdbStat = new XdbStat_t;

  struct stat *st = new struct stat;
  stat(filename.c_str(), st);
  if(st == nullptr) return nullptr;

  xdbStat->mode = st->st_mode;
  xdbStat->atime = st->st_atime;
  xdbStat->mtime = st->st_mtime;
  xdbStat->ctime = st->st_ctime;

  return xdbStat;
}

XdbEntry_t *Xdb::EntryFromFilename(const std::string& filename) {
  if(filename == "") return nullptr;

  XdbEntry_t *entry = new XdbEntry_t;

  entry->stat = *XdbStatFromFilename(filename);
  entry->path = std::string(GetDirectoryPath(filename.c_str()));
  entry->filename = filename;
  entry->name = GenerateName(filename);
  entry->bufferSize = 0; // for now
  entry->buffer = nullptr; // for now
  
  std::ifstream inf(filename, std::ios::binary | std::ios::ate);
  if(!inf.is_open()) goto error;

  entry->bufferSize = inf.tellg();

  inf.seekg(0);

  entry->buffer = new char[entry->bufferSize];

  inf.read(entry->buffer, entry->bufferSize);

  inf.close();

  return entry;
error:
  delete entry;
  if(inf.is_open()) inf.close();

  return nullptr;
}

int Xdb::PushBackFilename(const std::string& filename) {
  if(filename == "") return 1;

  entries.push_back(*EntryFromFilename(filename));

  return 0;
}

int Xdb::WriteToFile(const std::string& filename) {
  if(filename == "") return 1;

  std::ofstream outf(filename, std::ios::binary);
  if(!outf.is_open()) return 1;

  int tmp;

  outf.write("xdba", 4);
  tmp = tags.size();
  outf.write(reinterpret_cast<char*>(&tmp), 4);
  tmp = entries.size();
  outf.write(reinterpret_cast<char*>(&tmp), 4);
#ifndef _WIN32
  tmp = 0;
#else
  tmp = 1;
#endif
  outf.write(reinterpret_cast<char*>(&tmp), 4);

  for(int i = 0; i < tags.size(); i++) {
    outf.write(tags[i].c_str(), tags[i].size() + 1);
  }

  for(int i = 0; i < entries.size(); i++) {
    outf.write(reinterpret_cast<char*>(&entries[i].stat), sizeof(XdbStat_t));
    outf.write(entries[i].path.c_str(), entries[i].path.size() + 1);
    outf.write(entries[i].filename.c_str(), entries[i].filename.size() + 1);
    outf.write(entries[i].name.c_str(), entries[i].name.size() + 1);
    tmp = entries[i].bufferSize;
    outf.write(reinterpret_cast<char*>(&tmp), 4);
    outf.write(entries[i].buffer, entries[i].bufferSize);
  }

  outf.close();

  return 0;
}

std::string Xdb::GenerateName(const std::string& filename) {
  if(filename == "") return "";

  return std::string(GetFileNameWithoutExt(filename.c_str()));
}
