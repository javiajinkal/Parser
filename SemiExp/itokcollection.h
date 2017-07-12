#ifndef ITOKCOLLECTION_H
#define ITOKCOLLECTION_H
///////////////////////////////////////////////////////////////////////
//  ITokCollection.h - package for the ITokCollection interface      //
//  ver 1.0                                                          //
//  Language:    C++, Visual Studio 2015                             //
//  Application: Project2 , CSE687 - Object Oriented Design          //
//  Platform:    Lenovo Yoga 3, Core-i5, Windows 10				     //
//  Author:      Jinkal Arvind Javia, SUID: 425325424     	         //
//               jjavia@syr.edu		                                 //
//  Source:		 Jim Fawcett, Syracuse University, CST 4-187         //
//               jfawcett@twcny.rr.com								 //
///////////////////////////////////////////////////////////////////////
/*
  Module Purpose:
  ===============
  ITokCollection is an interface that supports substitution of different
  types of scanners for parsing.  In this solution, we illustrate that
  by binding two different types of collections, SemiExp to this interface.
  This is illustrated in the test stubs for the SemiExpression module.

  Build Process:
  ===================
  Required Files: SemiExp.h, SemiExp.cpp, Tokenizer.h, Tokenizer.cpp
  Build Command: devenv Project2.sln /rebuild debug

  Maintenance History:
  ====================
  ver 1.0 : 09 Feb 16
  - first release
*/
#include <string>

struct ITokCollection
{
	virtual bool get(bool clear = true) = 0;
	virtual size_t length() = 0;
	virtual std::string& operator[](int n) = 0;
	virtual size_t find(const std::string& tok) = 0;
	virtual void push_back(const std::string& tok) = 0;
	virtual bool merge(const std::string& firstTok, const std::string& secondTok) = 0;
	virtual bool remove(const std::string& tok) = 0;
	virtual bool remove(size_t i) = 0;
	virtual void toLower() = 0;
	virtual void trimFront() = 0;
	virtual void clear() = 0;
	virtual std::string show(bool showNewLines = false) = 0;
	virtual ~ITokCollection() {};
};
#endif
