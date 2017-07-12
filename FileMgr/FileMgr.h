#ifndef FILEMGR_H
#define FILEMGR_H
///////////////////////////////////////////////////////////////////////////////
// FileMgr.h - find files matching specified patterns						 //
//             on a specified path											 //
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
Module Operations
-----------------
find files matching specified patterns on a specified path

Required Files
--------------
FileMgr.h, FileMgr.cpp

Build Command
--------------
devenv Project2.sln /rebuild debug

Maintenance History:
====================
* ver 1.0 : 12 March 16
* - first release
*/

#include "FileSystem.h"
#include "../DataStore/DataStore.h"
#include <iostream>

class FileMgr
{
public:
  using iterator = DataStore::iterator;
  using patterns = std::vector<std::string>;

  FileMgr(const std::string& path, DataStore& ds) : path_(path), store_(ds)
  {
    patterns_.push_back("*.*");
  }

  void addPattern(const std::string& patt)
  {
    if (patterns_.size() == 1 && patterns_[0] == "*.*")
      patterns_.pop_back();
    patterns_.push_back(patt);
  }

  void search()
  {
    find(path_);
  }

  void find(const std::string& path)
  {
    std::string fpath = FileSystem::Path::getFullFileSpec(path);
    //---------------------------------------------------------
    // The statement above is not necessary, but helps to
    // make the processing clearer.  To show that, comment
    // it out and uncomment the following line.
    //
    // std::string fpath = path;
    //
    // Remove the std::cout statements below after you're 
    // convinced that everything works as it should.
    //---------------------------------------------------------

    //std::cout << "\n  ++ " << fpath;
    for (auto patt : patterns_)  // the scope of this for loop was
    {                            // changed to improve efficiency
      std::vector<std::string> files = FileSystem::Directory::getFiles(fpath, patt);
      for (auto f : files)
      {
        //std::cout << "\n  --   " << f;
        store_.save(fpath+"\\"+f);
      }
    }
    std::vector<std::string> dirs = FileSystem::Directory::getDirectories(fpath);
    for (auto d : dirs)
    {
      if (d == "." || d == "..")
        continue;
      std::string dpath = fpath + "\\" + d;  // here's the fix
      find(dpath);
    }
  }
private:
  std::string path_;
  DataStore& store_;
  patterns patterns_;
};

#endif
