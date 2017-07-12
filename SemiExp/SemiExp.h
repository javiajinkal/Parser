#ifndef SEMIEXPRESSION_H
#define SEMIEXPRESSION_H
///////////////////////////////////////////////////////////////////////
//  SemiExp.h - collect tokens for analysis                          //
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
* Package Operations:
* -------------------
* This package provides a public SemiExp class that collects and makes
* available sequences of tokens.  SemiExp uses the services of a Toker
* class to acquire tokens.  Each call to SemiExp::get() returns a
* sequence of tokens that ends in {.  This will be extended to use the
* full set of terminators: {, }, ;, and '\n' if the line begins with #.
* public :, protected :, private : as terminators. It also handles special
* circumstances like for(;;) expression
*
* Build Process:
* --------------
* Required Files:
*   SemiExp.h, SemiExp.cpp, Tokenizer.h, Tokenizer.cpp
*
* Build Command: devenv Project2.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 09 Feb 16
* - first release
*
*/

#include <vector>
#include "itokcollection.h"
#include "../Tokenizer/Tokenizer.h"

namespace Scanner
{
	using Token = std::string;
	using namespace Scanner;

	class SemiExp : public ITokCollection
	{
	public:
		SemiExp(Toker* pToker = nullptr);
		SemiExp(const SemiExp&) = delete;
		SemiExp& operator=(const SemiExp&) = delete;
		std::string& operator[](int n);
		bool get(bool clear = true);
		size_t find(const std::string& tok);
		void push_back(const std::string& tok);
		bool merge(const std::string& firstTok, const std::string& secondTok);
		bool remove(const std::string& tok);
		bool remove(size_t i);
		void toLower();
		void trimFront();
		void clear();
		bool isComment(std::string t);
		bool isTerminator(std::string t);
		bool isPublicPrivateProtected(std::string t1,std::string t2);
		std::string handleFor(std::string t);
		std::string show(bool showNewLines = false);
		size_t length();
	private:
		std::vector<Token> _tokens;
		Toker* _pToker;
	};
}
#endif
