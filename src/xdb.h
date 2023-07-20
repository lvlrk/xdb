#pragma once

// #define XDB_USE_64

#ifndef _WIN32        // check for penguin os, as it should be
#include <sys/stat.h> // mode_t, time_t

#define XDBSTAT_USE_MODE
#else
#include <ctime> // time_t

typedef mode_t int;
#endif

#include <memory> // std::unique_ptr
#include <string> // std::string
#include <vector> // std::vector

struct MemDesc { // offset,size pair struct for describing memory
    int off;
    int size;
};

#ifdef XDB_USE_64
struct MemDesc64 {
    long long off;
    long long size;
};
#endif

class XdbStat { // contains 'portable' stat info
    public:
        int FromFilename(const std::string &filename);

        mode_t mode = 0; // will be 0 on NOT PENGUIN OS >:( L windows
        time_t atime = 0;
        time_t mtime = 0;
        time_t ctime = 0;
};

class XdbEntry { // note: all char* are cstrings;
                 // will read from binary until 0
    public:
        XdbEntry();
        XdbEntry(const XdbEntry &oldEntry); // copy constructor

        struct XdbStat stat; // custom stat metadata
        std::string filename;
        std::string name; // generated 'cleaner' filename
        int bufferSize;
        std::vector<char> buffer; // file data buffer
};

class Xdb {
    public:
        /*** Userspace functions ***/
        // pushes filename file onto entries; returns non-zero on error
        int PushBackFilename(const std::string &filename);

        // writes file buffer data to a xdb database; returns non-zero on error
        int WriteToFile(const std::string &filename);

        // reads from file; returns non-zero on error
        int ReadFromFile(const std::string &filename);

        /*** Utility functions ***/
        // returns xdbstat created from filename; returns nullptr on error

        // creates a 'clean' name from filename; returns "" on error
        std::string GenerateName(const std::string &filename);

        // returns entry created from filename; returns nullptr on error
        void EntryFromFilename(const std::string &filename, XdbEntry &entry);

        std::vector<XdbEntry> GetEntries();

    private:
        std::vector<std::string> tags;
        std::vector<XdbEntry> entries;
};
