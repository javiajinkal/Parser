#ifndef TOKENIZER_H
#define TOKENIZER_H
///////////////////////////////////////////////////////////////////////
// Tokenizer.h - read words from a std::stream                       //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Project2 , CSE687 - Object Oriented Design           //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10				     //
// Author:      Jinkal Arvind Javia, SUID: 425325424     	         //
//              jjavia@syr.edu		                                 //
// Source:		Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com								 //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a public Toker class and private ConsumeState class.
 * Toker reads words from a std::stream, throws away whitespace, throws comments
 * if show_comment is set to false and returns words from the stream in the 
 * order encountered. Quoted strings and certain punctuators and newlines are 
 * returned as single tokens.
 * - Return quoted strings as single token and handles \" and \'
 *   correctly.
 * - Return [, ], {, }, (, ), <, >, :, =, +, -, *, \n as single character tokens
 * - Return <<, >>, +=, -=, *=, /=, ::, ++, --, == as two character tokens
 *
 * This is version, based on the State Design Pattern.
 *
 * Build Process:
 * --------------
 * Required Files: Tokenizer.h, Tokenizer.cpp
 * Build Command: devenv Project2.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 09 Feb 16
 * - first release
 */
#include <iosfwd>
#include <string>

namespace Scanner
{
  class ConsumeState;

  class Toker
  {
  public:
    Toker();
	Toker(const Toker&) = delete;
    ~Toker();
    bool attach(std::istream* pIn);
    std::string getTok();
    bool canRead();
	void returnComments(bool doReturnComments = true);
	size_t currentLineCount();
	void setSpecialSingleChars(std::string ssc);
	void setSpecialCharPairs(std::string scp);
	void reset();
  private:
    ConsumeState* pConsumer;
  };
}
#endif