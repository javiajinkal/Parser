/////////////////////////////////////////////////////////////////////////////////
// MetricsAnalysis.cpp-evaluates and displays size and complexity of functions //
// ver 1.0																	   //
// Language:    C++, Visual Studio 2015                                        //
// Application: Project2 , CSE687 - Object Oriented Design				       //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10						       //
// Author:      Jinkal Arvind Javia, SUID: 425325424     				       //
//              jjavia@syr.edu											       //
// Source:		Jim Fawcett, Syracuse University, CST 4-187				       //
//              jfawcett@twcny.rr.com								           //
/////////////////////////////////////////////////////////////////////////////////

#include "MetricsAnalysis.h"
#include <iostream>
#include <string>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../AST/AST.h"
#include "../Parser/Parser.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../FileMgr/FileSystem.h"

#define Util StringHelper
using namespace Utilities;

//Performs Metric Analysis on the provided file
bool metricsAnalysis::MetricAnalysis(std::string file)
{
		std::string fileSpec = FileSystem::Path::getFullFileSpec(file);
		std::string msg = "Processing file: " + fileSpec;
		std::cout << "\n--------------------------------------------------------------------------------";
		Util::title(msg);
		ConfigParseToConsole configure;
		Parser* pParser = configure.Build();
		try{
			if (pParser){
				if (!configure.Attach(fileSpec))
					std::cout << "\n  could not open file " << fileSpec << std::endl;
			}
			else{
				std::cout << "\n\n  Parser not built\n\n";
				return 1;
			}
			while (pParser->next())            //Using the built parser
				pParser->parse();
			Repository* repo = Repository::getInstance();
			std::vector<element> vectorFunction;
			std::cout << "\n\nREQUIREMENT 5 and 6\n";
			std::cout << "======================ABSTRACT SYNTAX TREE========================\n";
			std::cout << "Display Format for AST: (TYPE, NAME, LINECOUNT, COMPLEXITY)\n";
			repo->retrieveAST()->AST::getRoot()->lineCount = repo->lineCount();
			repo->retrieveAST()->AST::getRoot()->complexityCount = repo->retrieveAST()->complexity(repo->retrieveAST()->AST::getRoot());
			repo->retrieveAST()->AST::TreeWalk(repo->retrieveAST()->AST::getRoot());
			vectorFunction = repo->retrieveAST()->AST::getFunctionVector();
			std::cout << "\n\nREQUIREMENT 8";
			std::cout << "\n=====================METRICS ANALYSIS STARTS======================\n";
			std::cout << "\n\t Lines\tComplexity\tFunction Name\n";
			for (auto i : vectorFunction){
				std::cout << "\n\t" << i.lineCount << "\t";
				std::cout << i.complexityCount << "\t\t";
				std::cout << i.name;
			}
			std::cout << "\n\n=====================METRICS ANALYSIS ENDS========================\n";
			return 0;
		}
		catch (std::exception& ex){
			std::cout << "\n\n    " << ex.what() << "\n\n";
		}
	return 1;
}

#ifdef TEST_METRICSANALYSIS

int main(int argc, char* argv)
{
	bool check;
	std::string fileSpec = "../MetricsAnalysis/test.txt";
    if (argc < 2)
	{
		std::cout
			<< "\n  please enter name of file to process on command line\n\n";
		return 1;
	}
	metricsAnalysis ma;
	check = ma.MetricAnalysis(fileSpec);
	if (check == 1)
	{
		std::cout << "Metrics Analysis of input file is unsuccessful\n";
	}
	return 0;
}

#endif
