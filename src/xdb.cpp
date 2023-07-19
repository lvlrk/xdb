#include <raylib.h>
#include <iostream> // std::cout
#include <fstream> // std::ifstream
#include <fmt/core.h> // fmt::format, fmt::print
#include <sys/stat.h> // stat
#include <cstring> // std::strerror
#include <errno.h> // errno
#include "xdb.h"
#include "util.h" // ReadStringFromFile

std::vector<XdbEntry> Xdb::GetEntries() {
    return entries;
}

int XdbStat::FromFilename(const std::string& filename) {
    struct stat st;

    if(stat(filename.c_str(), &st) == -1) {
        std::cerr << fmt::format("{}() error: Could not stat file '{}'\n", __func__, filename);
    } else {
        mode = st.st_mode;
        atime = st.st_atime;
        mtime = st.st_mtime;
        ctime = st.st_ctime;
    }

    return 0;
}

XdbEntry::XdbEntry() {} // empty default constructor

XdbEntry::XdbEntry(const XdbEntry& oldEntry)
    :
        buffer(std::move(oldEntry.buffer))
{ // phat copier
    stat = oldEntry.stat;
    filename = oldEntry.filename;
    name = oldEntry.name;
    bufferSize = oldEntry.bufferSize;
}

void Xdb::EntryFromFilename(const std::string& filename, XdbEntry& entry) {
    XdbStat xdbStat;

    xdbStat.FromFilename(filename);
    entry.stat = xdbStat;
    entry.filename = filename;
    entry.name = GenerateName(filename);
    entry.bufferSize = 0; // for now

    std::ifstream inf(filename, std::ios::binary | std::ios::ate);
    if(!inf.is_open()) {
        std::cerr << fmt::format("{}() error: Could not open file '{}'\n", __func__, filename);
    }

    entry.bufferSize = inf.tellg();

    inf.seekg(0);

    entry.buffer = std::vector<char>(entry.bufferSize);

    inf.read(entry.buffer.data(), entry.bufferSize);
}

int Xdb::PushBackFilename(const std::string& filename) {
    if(filename == "") return 1;

    XdbEntry entry;
    EntryFromFilename(filename, entry);

    if(entry.buffer.size() != 0) entries.push_back(entry);
    else return 1;

    return 0;
}

int Xdb::ReadFromFile(const std::string& filename) {
    if(filename == "") return 1;

    std::ifstream inf(filename, std::ios::binary);
    if(!inf.is_open()) return 1;

    char magic[4];

    inf.read(magic, 4);

    if(std::string(magic) != "xdba") return 1;

    int tmp;

    inf.read(reinterpret_cast<char*>(&tmp), 4);
    tags.resize(tmp);
    
    inf.read(reinterpret_cast<char*>(&tmp), 4);
    entries.resize(tmp);

    for(int i = 0; i < tags.size(); i++) {
        tags[i] = ReadStringFromFile(inf);
    }

    for(int i = 0; i < entries.size(); i++) {
        inf.read(reinterpret_cast<char*>(&entries[i].stat), 16);

        entries[i].filename = ReadStringFromFile(inf);
        if(i == 0) entries[i].filename = entries[i].filename.substr(4);

        entries[i].name = ReadStringFromFile(inf);

        inf.read(reinterpret_cast<char*>(&entries[i].bufferSize), 4);
        entries[i].buffer = std::vector<char>(entries[i].bufferSize);
        inf.read(reinterpret_cast<char*>(entries[i].buffer.data()), entries[i].bufferSize);
    }

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
        // stat
        outf.write(reinterpret_cast<char*>(&entries[i].stat.mode), 4);
        outf.write(reinterpret_cast<char*>(&entries[i].stat.atime), 4);
        outf.write(reinterpret_cast<char*>(&entries[i].stat.mtime), 4);
        outf.write(reinterpret_cast<char*>(&entries[i].stat.ctime), 4);

        // strings
        outf.write(entries[i].filename.c_str(), entries[i].filename.size() + 1);
        outf.write(entries[i].name.c_str(), entries[i].name.size() + 1);

        // buffer
        outf.write(reinterpret_cast<char*>(&entries[i].bufferSize), 4);
        outf.write(entries[i].buffer.data(), entries[i].bufferSize);
    }

    outf.close();

    return 0;
}

std::string Xdb::GenerateName(const std::string& filename) {
    if(filename == "") return "";

    return std::string(GetFileNameWithoutExt(filename.c_str()));
}
