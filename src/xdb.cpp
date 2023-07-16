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
  entry->path = const_cast<char*>(GetDirectoryPath(filename.c_str()));
  entry->filename = const_cast<char*>(filename.c_str());
  entry->name = const_cast<char*>(GenerateName(filename).c_str());
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

void Xdb::PopulateFileHeader() {
  file.header.magic[0] = 'x';
    file.header.magic[1] = 'd';
    file.header.magic[2] = 'b';
    file.header.magic[3] = 'a';
  file.header.tagCount = tags.size();
  file.header.entryCount = entries.size();
#ifndef _WIN32
  file.header.host = 0; // linux
#else
  file.header.host = 1; // not linux lol
#endif
}

void Xdb::PopulateTags() {
  file.tags = new char*[tags.size()];

  for(int i = 0; i < tags.size(); i++) {
    file.tags[i] = const_cast<char*>(tags[i].c_str());
  }
}

void Xdb::PopulateEntries() {
  file.entries = new XdbEntry_t[entries.size()];

  for(int i = 0; i < entries.size(); i++) {
    file.entries[i] = entries[i];
  }
}

int Xdb::WriteToFile(const std::string& filename) {
  if(filename == "") return 1;

  std::ofstream outf(filename, std::ios::binary);
  if(!outf.is_open()) return 1;

  PopulateFileHeader();

  outf.write(reinterpret_cast<char*>(&file.header), sizeof(XdbHeader_t));

  PopulateTags();

  for(int i = 0; i < tags.size(); i++) {
    outf.write(file.tags[i], std::strlen(file.tags[i] + 1));
  }

  PopulateEntries();

  for(int i = 0; i < entries.size(); i++) {
    outf.write(reinterpret_cast<char*>(&file.entries[i].stat), sizeof(XdbStat_t));
    outf.write(file.entries[i].path, std::strlen(file.entries[i].path) + 1);
    outf.write(file.entries[i].filename, std::strlen(file.entries[i].filename) + 1);
    outf.write(file.entries[i].name, std::strlen(file.entries[i].name) + 1);
    outf.write(reinterpret_cast<char*>(&file.entries[i].bufferSize), 4);
    outf.write(file.entries[i].buffer, file.entries[i].bufferSize);
  }

  outf.close();

  return 0;
}

std::string Xdb::GenerateName(const std::string& filename) {
  if(filename == "") return "";

  return std::string(GetFileNameWithoutExt(filename.c_str()));
}
