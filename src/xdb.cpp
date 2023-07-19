#include <raylib.h>
#include <iostream> // std::cout
#include <fstream> // std::ifstream
#include <fmt/core.h> // fmt::format, fmt::print
#include <sys/stat.h> // stat
#include <cstring> // std::strerror
#include <errno.h> // errno
#include "xdb.h"
#include "util.h" // DEBUG

int XdbStat::FromFilename(const std::string& filename) {
    HDEBUG("[");
    HDEBUG("{ xdbStat()");
    XdbStat xdbStat;
    HDEBUG("}");

    struct stat st;

    if(stat(filename.c_str(), &st)) {
        HDEBUG("stat bad");
        std::cout << fmt::format("{}() error: Could not stat file '{}'\n", __func__, filename);

        DEBUG(strerror(errno));
    } else {
        HDEBUG("stat good");
        xdbStat.mode = st.st_mode;
        xdbStat.atime = st.st_atime;
        xdbStat.mtime = st.st_mtime;
        xdbStat.ctime = st.st_ctime;
    }

    HDEBUG("]");

    return 0;
}

XdbEntry::XdbEntry() {} // empty default constructor

XdbEntry::XdbEntry(const XdbEntry& oldEntry) { // phat copier
    HDEBUG("copy=[");
    stat = oldEntry.stat;
    filename = oldEntry.filename;
    name = oldEntry.name;
    bufferSize = oldEntry.bufferSize;
    buffer = std::make_unique<char>(*oldEntry.buffer);
    HDEBUG("]");
}

void Xdb::EntryFromFilename(const std::string& filename, XdbEntry& entry) {
    HDEBUG("{ xdbStat()");
    XdbStat xdbStat;
    HDEBUG("}");

    xdbStat.FromFilename(filename);
    entry.stat = xdbStat;
    entry.filename = filename;
    entry.name = GenerateName(filename);
    entry.bufferSize = 0; // for now
    entry.buffer = nullptr; // for now

    std::ifstream inf(filename, std::ios::binary | std::ios::ate);
    if(!inf.is_open()) {
        std::cout << fmt::format("{}() error: Could not open file '{}'\n", __func__, filename);
    }

    entry.bufferSize = inf.tellg();

    inf.seekg(0);

    HDEBUG("buffer create");
    entry.buffer = std::make_unique<char>(entry.bufferSize);

    HDEBUG("buffer read");
    inf.read(entry.buffer.get(), entry.bufferSize);
    HDEBUG("buffer readed");
}

int Xdb::PushBackFilename(const std::string& filename) {
    if(filename == "") return 1;

    HDEBUG("entry init");
    XdbEntry entry;
    HDEBUG("after");
    EntryFromFilename(filename, entry);
    HDEBUG("done");

    HDEBUG("check  entry");
    if(entry.buffer.get() != nullptr) entries.push_back(entry);
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
        HDEBUG(tags[i]);
    }

    for(int i = 0; i < entries.size(); i++) {
        inf.read(reinterpret_cast<char*>(&entries[i].stat), 16);

        entries[i].filename = ReadStringFromFile(inf);
        HDEBUG(entries[i].filename);

        entries[i].name = ReadStringFromFile(inf);
        HDEBUG(entries[i].name);

        fmt::print("{}({})\n", entries[i].filename, entries[i].name);

        inf.read(reinterpret_cast<char*>(&entries[i].bufferSize), 4);
        entries[i].buffer = std::make_unique<char>(entries[i].bufferSize);
        inf.read(entries[i].buffer.get(), entries[i].bufferSize);
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

    HDEBUG("whatup");
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
        HDEBUG("burh");
        outf.write(entries[i].buffer.get(), entries[i].bufferSize);
        HDEBUG("biggie");
    }

    outf.close();

    HDEBUG("ok");

    return 0;
}

std::string Xdb::GenerateName(const std::string& filename) {
    if(filename == "") return "";

    return std::string(GetFileNameWithoutExt(filename.c_str()));
}
