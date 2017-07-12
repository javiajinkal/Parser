#ifndef METRICSANALYSIS_H
#define METRICSANALYSIS_H
/////////////////////////////////////////////////////////////////////////////////
// MetricsAnalysis.h - evaluates and displays size and complexity of functions //
// ver 1.0																	   //
// Language:    C++, Visual Studio 2015                                        //
// Application: Project2 , CSE687 - Object Oriented Design				       //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10						       //
// Author:      Jinkal Arvind Javia, SUID: 425325424     				       //
//              jjavia@syr.edu											       //
// Source:		Jim Fawcett, Syracuse University, CST 4-187				       //
//              jfawcett@twcny.rr.com								           //
/////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package evaluates and displays the size and complexity of 
*	all global functions, 
*	static member functions, 
*	and non-static member functions in each of a set of specified packages.
*
* Build Process:
* --------------
* Required Files: Parser.h, Parser.cpp, SemiExp.h, SemiExp.cpp,
*      Tokenizer.h, Tokenizer.cpp,
*      ActionsAndRules.h, ActionsAndRules.cpp,
*      ConfigureParser.h, ConfigureParser.cpp,
*      Utilities.h, Utilities.cpp,FileSystem.h, 
*      FileSystem.cpp, AST.h, AST.cpp, 
*      ScopeStack.h, ScopeStack.cpp,
*      MetricsAnalysis.h, MetricsAnalysis.cpp
*
* Build Command: devenv Project2.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 12 March 16
* - first release
*/
#include <string>
#include <iostream>
#include "../Parser/Parser.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"

class metricsAnalysis {
public:
	bool MetricAnalysis(std::string);
};

#endif