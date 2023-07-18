#include <raylib.h> // GetDirectoryPath
#include <iostream>
#include <fstream> // std::ifstream
#include <fmt/core.h>
#include <sys/stat.h>
#include <cstring> // std::strerror
#include <errno.h>
#include "xdb.h"
#include "util.h"

int XdbStat::FromFilename(const std::string& filename) {
    XdbStat xdbStat;

    struct stat st;

    if(stat(filename.c_str(), &st)) {
        std::cout << fmt::format("{}() error: Could not stat file '{}'\n", __func__, filename);

        DEBUG(strerror(errno));
    } else {
        xdbStat.mode = st.st_mode;
        xdbStat.atime = st.st_atime;
        xdbStat.mtime = st.st_mtime;
        xdbStat.ctime = st.st_ctime;
    }

    return 0;
}

XdbEntry Xdb::EntryFromFilename(const std::string& filename) {
    XdbEntry entry;
    XdbStat xdbStat;

    xdbStat.FromFilename(filename);
    entry.stat = xdbStat;
    entry.filename = filename;
    entry.name = GenerateName(filename);
    entry.bufferSize = 0; // for now
    entry.buffer = nullptr; // for now

    std::ifstream inf(filename, std::ios::binary | std::ios::ate);
    if(!inf.is_open()) {
        std::cout << fmt::format("{}() error: Could not open file '{}'\n", __func__, filename);

        return entry;
    }

    entry.bufferSize = inf.tellg();

    inf.seekg(0);

    entry.buffer = std::make_unique<char>(entry.bufferSize);

    inf.read(entry.buffer.get(), entry.bufferSize);

    inf.close();

    return entry;
}

int Xdb::PushBackFilename(const std::string& filename) {
    if(filename == "") return 1;

    struct XdbEntry entry = EntryFromFilename(filename);

    if(entry.buffer != nullptr) entries.push_back(entry);
    else return 1;

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
        outf.write(entries[i].buffer.get(), entries[i].bufferSize);
    }

    outf.close();

    return 0;
}

std::string Xdb::GenerateName(const std::string& filename) {
    if(filename == "") return "";

    return std::string(GetFileNameWithoutExt(filename.c_str()));
}
