////////////////////////////////////////////////////////////////////////////////////////
// MetricsExecutive.cpp-collects metrics for all packages in a directory tree rooted  //
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

#include "MetricsExecutive.h"
#include "../MetricsAnalysis/MetricsAnalysis.h"
#include <iterator>

//Parse all the file patterns from command line
std::vector<std::string> MetricEx::patternCommandLine(std::string filepatterns) {
	std::istringstream iss(filepatterns);
	std::vector<std::string> patternVector{ std::istream_iterator<std::string>{iss},       // adding to vector
		std::istream_iterator<std::string>{} };
	return patternVector;
}

//Demonstrates Requirement 1 of Project2
void MetricEx::Req1()
{
	std::cout << "\n------------------------DEMONSTRATES REQUIREMENT 1------------------------\n";
	std::cout << "\nUses Visual Studio 2015 and its C++ Windows Console Projects, as provided in the ECS computer labs\n";
}

//Demonstrates Requirement 2 of Project2
void MetricEx::Req2()
{
	std::cout << "\n------------------------DEMONSTRATES REQUIREMENT 2------------------------\n";
	std::cout << "\nUses the C++ standard library's streams for all I/O and new and delete for all heap-based memory management\n";
}

//Demonstrates Requirement 3 of Project2
void MetricEx::Req3()
{
	std::cout << "\n------------------------DEMONSTRATES REQUIREMENT 3------------------------\n";
	std::cout << "\nC++ packages MetricsAnalysis and MetricsExecutive are provided for analyzing function size and complexity metrics for a set of specified packages. These packages will use my Tokenizer and SemiExp packages developed in Project #1\n";
}

//Demonstrates Requirement 4 of Project2
void MetricEx::Req4()
{
	std::cout << "\n------------------------DEMONSTRATES REQUIREMENT 4------------------------\n";
	std::cout << "\nParser package with a Parser class is provided that is a container for Rules and that provides the interfaces IRule and IAction for rules contained in the Parser and actions contained in each rule\n";
}

//Demonstrates Requirement 5 of Project2
void MetricEx::Req5()
{
	std::cout << "\n------------------------DEMONSTRATES REQUIREMENT 5------------------------\n";
	std::cout << "\nAn associated RulesAndActions package is provided that has rules to detect: \n";
	std::cout << "\t1. Global functions and static and non - static member function definitions\n";
	std::cout << "\t2. Beginning and end of all C++ scopes\n";
	std::cout << "\t3. Actions for each rule that support building the Abstract Syntax Tree(AST) that represents a single file's scope structure and identifies the type and line number extend of each scope\n";
}

//Demonstrates Requirement 6 of Project2
void MetricEx::Req6()
{
	std::cout << "\n------------------------DEMONSTRATES REQUIREMENT 6------------------------\n";
	std::cout << "\nFacility for building an abstract syntax tree is provided by AST package that provides an interface for adding scope nodes to the tree and an methods to analyze the contents of the tree\n";
}

// Demonstrates Requirement 7 of Project2
void MetricEx::Req7()
{
	std::cout << "\n------------------------DEMONSTRATES REQUIREMENT 7------------------------\n";
	std::cout << "\nFileMgr package that supports finding files and/or directories in a directory tree rooted at a specified path is provided\n";
}

// Demonstrates Requirement 8 of Project2
void MetricEx::Req8()
{
	std::cout << "\n------------------------DEMONSTRATES REQUIREMENT 8------------------------\n";
	std::cout << "\nMetricsAnalysis package is provided for evaluating and displaying the size and complexity of all global functions, static member functions, and non-static member functions in each of a set of specified packages\n";
}

// Demonstrates Requirement 9 of Project2
void MetricEx::Req9()
{
	std::cout << "\n------------------------DEMONSTRATES REQUIREMENT 9------------------------\n";
	std::cout << "\n MetricsExecutive package is provided that enables collecting metrics for all the packages with names that match a specified pattern in a directory tree rooted at a specified path. The path and file patterns are provided on command line\n";
}

// Demonstrates Requirement 10 of Project2
void MetricEx::Req10()
{
	std::cout << "\n------------------------DEMONSTRATES REQUIREMENT 10------------------------\n";
	std::cout << "\n This is an automated unit test suite that exercises all of the packages provided in the submission and demonstrates that all requirements are met\n";
}

// Grabs all the files on the given path which matches the specified pattern
std::vector <std::string> MetricEx::getfiles(std::string path, std::vector<std::string> pattern)
{
	std::vector <std::string> result;
	DataStore ds;
	FileMgr fm(path, ds);
	for(std::string pat: pattern)
		fm.addPattern(pat);
	fm.search();
	for (auto fs : ds)
		result.push_back(fs);
	std::cout << "\n\n";
	return result;
}

// Runs the Metrics Analyzer on each of the files grabed by getfiles function
void MetricEx::runAnalyzer(std::vector<std::string> inputFile)
{
	std::cout << "REQUIREMENT 7 and 9\n";
	std::cout << "*********************METRICS EXECUTIVE STARTS**********************\n";
	for (std::string fs : inputFile)
	{
		bool check;
		std::cout << "\n  " << fs;
		metricsAnalysis ma;
		check = ma.MetricAnalysis(fs);
		if (check == 1)
			std::cout << "Metrics Analysis of input file is unsuccessful\n";
	}
	std::cout << "\n*********************METRICS EXECUTIVE ENDS************************\n";
}

#ifndef TEST_METRICSEXECUTIVE
int main(int argc, char* argv[])
{
	MetricEx m;
	std::string path;
	std::vector <std::string> pat;
	std::vector <std::string> files;
	// collecting tokens from files, named on the command line
	if (argc == 3)   //Looking for pattern from command line
	{
		path = argv[1];
		pat = m.patternCommandLine(argv[2]);
	}
	else{ // When no pattern provided, default pattern is set to .cpp and .h
		pat.push_back("*.cpp");
		pat.push_back("*.h");
		path = "./Test"; // set default path.
		std::cout << "\n path and .cpp and .h are set as default path and patterns as pattern is not specified on command line.";
	}
	std::cout << "Input Path on command line: " << path << "\n";
	std::cout << "Input Patterns on command line are: ";
	for (auto p : pat)
		std::cout << p << "\t";
	files = m.getfiles(path, pat);
	//std::string path = "C:/Users/JinkalArvind/Documents/Visual Studio 2015/Projects/MyTest/MyTest/Test";
	std::cout << "\n--------------------------THIS IS START OF TEST SUITE-------------------------\n";
	m.Req1();
	m.Req2();
	m.Req3();
	m.Req4();
	m.Req5();
	m.Req6();
	m.Req7();
	m.Req8();
	m.Req9();
	m.Req10();
	m.runAnalyzer(files);
	std::cout << "\n--------------------------THIS IS END OF TEST SUITE-------------------------\n";
	return 0;
}
#endif