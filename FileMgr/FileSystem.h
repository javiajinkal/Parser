#ifndef FILESYSTEM_H
#define FILESYSTEM_H
///////////////////////////////////////////////////////////////////////////////
// FileSystem.h - Support file and directory operations                      //
// ver 1.0																     //
// Language:    C++, Visual Studio 2015                                      //
// Application: Project2 , CSE687 - Object Oriented Design				     //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10						     //
// Author:      Jinkal Arvind Javia, SUID: 425325424     				     //
//              jjavia@syr.edu											     //
// Source:		Jim Fawcett, Syracuse University, CST 4-187				     //
//              jfawcett@twcny.rr.com								         //
///////////////////////////////////////////////////////////////////////////////

/*
 * Module Operations:
 * ==================
 * This module provides classes, File, FileInfo, Path, Directory, and 
 * FileSystemSearch.
 *
 * The File class supports opening text and binary files for either input 
 * or output.  File objects have names, get and put lines of text, get and
 * put blocks of bytes if binary, can be tested for operational state,
 * cleared of errors, and output File objects can be flushed to their streams.
 *
 * FileInfo class accepts a fully qualified filespec and supports queries
 * concerning name, time and date, size, and attributes.  You can compare
 * FileInfo objects by name, date, and size.
 *
 * Path class provides static methods to turn a relative filespec into an
 * absolute filespec, return the path, name, or extension of the filespec,
 * and build filespecs from path and name.  Path also provides methods to
 * convert filespecs to uppercase and to lowercase.
 *
 * The Directory class supports getting filenames or directories from a 
 * fully qualified filespec, e.g., path + filename pattern using static
 * methods.  It also provides non-static methods to get and set the current
 * directory.
 *
 * Public Interface:
 * =================
 * File f(filespec,File::in,File::binary);
 * File g(filespec,File::out,File::binary);
 * while(f.isGood()) {
 *   block b = f.getBlock();
 *   g.putBlock(b);
 * }
 * FileInfo fi("..\foobar.txt");
 * if(fi.good())
 *   ...
 * std::string filespec = "..\temp.txt";
 * std::string fullyqualified = Path::getFullFileSpec(filespec);
 * std::string path = Path::getPath(fullyqualified);
 * std::string name = Path::getName(fullyqualified);
 * std::string extn = Path::getExt(fullyqualified);
 *
 * Directory d;
 * std::string dir = d.getCurrentDirectory();
 * d.setCurrentDirectory(dir);
 * std::vector<std::string> files = Directory::getFiles(path, pattern);
 * std::vector<std::string> dirs = Directory::getDirectories(path);
 * 
 * Required Files:
 * ===============
 * FileSystem.h, FileSystem.cpp
 *
 * Build Command:
 * ==============
 * devenv Project2.sln /rebuild debug
 *
 * Maintenance History:
 * ====================
 * ver 1.0 : 12 March 16
 * - first release
 */
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>

namespace FileSystem
{
  /////////////////////////////////////////////////////////
  // Block
  
  typedef char Byte;

  class Block
  {
  public:
    Block(size_t size=0) : bytes_(size) {}
    Block(Byte* beg, Byte* end);
    void push_back(Byte b);
    Byte& operator[](size_t i);
    Byte operator[](size_t i) const;
    bool operator==(const Block&) const;
    bool operator!=(const Block&) const;
    size_t size() const;
  private:
    std::vector<Byte> bytes_;
  };

  /////////////////////////////////////////////////////////
  // File
  
  class File
  {
  public:
    enum direction { in, out };
    enum type { text, binary };
    File(const std::string& filespec);
    bool open(direction dirn, type typ=File::text);
    ~File();
    std::string name();
    std::string getLine();
    void putLine(const std::string& line, bool wantReturn=true);
    Block getBlock(size_t size);
    void putBlock(const Block&);
    bool isGood();
    void clear();
    void flush();
    void close();
    static bool exists(const std::string& file);
    static bool copy(const std::string& src, const std::string& dst, bool failIfExists=false);
    static bool remove(const std::string& filespec);
  private:
    std::string name_;
    std::ifstream* pIStream;
    std::ofstream* pOStream;
    direction dirn_;
    type typ_;
    bool good_;
  };

  inline std::string File::name() { return name_; }

  /////////////////////////////////////////////////////////
  // FileInfo

  class FileInfo
  {
  public:
    enum dateFormat { fullformat, timeformat, dateformat };
    FileInfo(const std::string& fileSpec);
    bool good();
    std::string name() const;
    std::string date(dateFormat df=fullformat) const;
    size_t size() const;
    
    bool isArchive() const;
    bool isCompressed() const;
    bool isDirectory() const;
    bool isEncrypted() const;
    bool isHidden() const;
    bool isNormal() const;
    bool isOffLine() const;
    bool isReadOnly() const;
    bool isSystem() const;
    bool isTemporary() const;

    bool operator<(const FileInfo& fi) const;
    bool operator==(const FileInfo& fi) const;
    bool operator>(const FileInfo& fi) const;
    bool earlier(const FileInfo& fi) const;
    bool later(const FileInfo& fi) const;
    bool smaller(const FileInfo& fi) const;
    bool larger(const FileInfo& fi) const;
  private:
    bool good_;
    static std::string intToString(long i);
    WIN32_FIND_DATAA data;
  };

  /////////////////////////////////////////////////////////
  // Path

  class Path
  {
  public:
    static std::string getFullFileSpec(const std::string& fileSpec);
    static std::string getPath(const std::string& fileSpec);
    static std::string getName(const std::string& fileSpec, bool withExt=true);
    static std::string getExt(const std::string& fileSpec);
    static std::string fileSpec(const std::string& path, const std::string& name);
    static std::string toLower(const std::string& src);
    static std::string toUpper(const std::string& src);
  };
  
  /////////////////////////////////////////////////////////
  // Directory

  class Directory
  {
  public:
    static bool create(const std::string& path);
    static bool remove(const std::string& path);
    static bool exists(const std::string& path);
    static std::string getCurrentDirectory();
    static bool setCurrentDirectory(const std::string& path);
    static std::vector<std::string> getFiles(const std::string& path=".", const std::string& pattern="*.*");
    static std::vector<std::string> getDirectories(const std::string& path=".", const std::string& pattern="*.*");
  private:
    //static const int BufSize = 255;
    //char buffer[BufSize];
  };
}

#endif
