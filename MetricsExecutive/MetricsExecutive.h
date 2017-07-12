#ifndef METRICSEXECUTIVE_H
#define METRICSEXECUTIVE_H
////////////////////////////////////////////////////////////////////////////////////////
// MetricsExecutive.h - collects metrics for all packages in a directory tree rooted  //
//						at a specified path											  //
// ver 1.0																			  //
// Language:    C++, Visual Studio 2015                                               //
// Application: Project2 , CSE687 - Object Oriented Design				              //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10						              //
// Author:      Jinkal Arvind Javia, SUID: 425325424     				              //
//              jjavia@syr.edu											              //
// Source:		Jim Fawcett, Syracuse University, CST 4-187				              //
//              jfawcett@twcny.rr.com								                  //
////////////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package enables collecting metrics for all the packages with names 
* that match a specified pattern in a directory tree rooted at a specified path. 
* The file patterns and the path are to be provide on MetricsExecutive commandline
*
* Build Process:
* --------------
* Required Files: Parser.h, Parser.cpp, SemiExp.h, SemiExp.cpp,
*      Tokenizer.h, Tokenizer.cpp,
*      ActionsAndRules.h, ActionsAndRules.cpp,
*      ConfigureParser.h, ConfigureParser.cpp,
*      Utilities.h, Utilities.cpp,FileSystem.h,
*      FileSystem.cpp, FileMgr.h, FileMgr.cpp
*      ScopeStack.h, ScopeStack.cpp, AST.h, AST.cpp,
*      MetricsAnalysis.h, MetricsAnalysis.cpp,
*      MetricsExecutive.h, MetricsExecutive.cpp
*
* Build Command: devenv Project2.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 12 March 16
* - first release
*/
#include "../FileMgr/FileMgr.h"
#include <string>
#include <iostream>
#include <vector>

//Excercises all the packages of Project2 and demonstrates all requirements
class MetricEx
{
private:
	std::vector <std::string> files;
public:
	std::vector <std::string> getfiles(std::string path, std::vector<std::string> pattern);
	std::vector<std::string> patternCommandLine(std::string filepatterns);
	void Req1();
	void Req2();
	void Req3();
	void Req4();
	void Req5();
	void Req6();
	void Req7();
	void Req8();
	void Req9();
	void Req10();
	void runAnalyzer(std::vector<std::string> inputFile);
};
#endif