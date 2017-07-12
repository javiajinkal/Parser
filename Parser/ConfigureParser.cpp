////////////////////////////////////////////////////////////////////////////
// ConfigureParser.cpp - builds and configures parsers					  //
// ver 1.0                                                                //
// Language:    C++, Visual Studio 2015                                   //
// Application: Project2 , CSE687 - Object Oriented Design				  //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10						  //
// Author:      Jinkal Arvind Javia, SUID: 425325424     				  //
//              jjavia@syr.edu											  //
// Source:		Jim Fawcett, Syracuse University, CST 4-187				  //
//              jfawcett@twcny.rr.com								      //
////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include "Parser.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "../AST/AST.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

using namespace Scanner;

//----< destructor releases all parts >------------------------------

ConfigParseToConsole::~ConfigParseToConsole(){
  // when Builder goes out of scope, everything must be deallocated
  delete pHandlePush;
  delete pBeginningOfScope;
  delete pHandlePop;
  delete pEndOfScope;
  delete pPrintClass;
  delete pPushClass;
  delete pClassDefinition;
  delete pPrintControlStatement;
  delete pPushControlStatement;
  delete pControlStatementDefinition;
  delete pPrintStruct;
  delete pPushStruct;
  delete pStructDefinition;
  delete pPushNamespace;
  delete pNamespaceDefinition;
  delete pPrintNamespace;
  delete pPrintFunction;
  delete pPushFunction;
  delete pFunctionDefinition;
  delete pDeclaration;
  delete pShowDeclaration;
  delete pExecutable;
  delete pShowExecutable;
  delete pRepo;
  delete pParser;
  delete pSemi;
  delete pToker;
  pIn->close();
  delete pIn;
}
//----< attach toker to a file stream or stringstream >------------

bool ConfigParseToConsole::Attach(const std::string& name, bool isFile)
{
  if(pToker == 0)
    return false;
  pIn = new std::ifstream(name);
  if (!pIn->good())
    return false;
  return pToker->attach(pIn);
}


//----< Here's where all the parts get assembled >---------------

Parser* ConfigParseToConsole::Build(){
	try{ pToker = new Toker;	// add Parser's main parts
    pToker->returnComments(false);
    pSemi = new SemiExp(pToker);
    pParser = new Parser(pSemi);
    pRepo = new Repository(pToker);
    pBeginningOfScope = new BeginningOfScope(); // configure to manage scope, these must come first - they return true on match, so rule checking continues
    pHandlePush = new HandlePush(pRepo);
    pBeginningOfScope->addAction(pHandlePush);
    pParser->addRule(pBeginningOfScope);
    pEndOfScope = new EndOfScope();
    pHandlePop = new HandlePop(pRepo);
    pEndOfScope->addAction(pHandlePop);
    pParser->addRule(pEndOfScope);
	pClassDefinition = new ClassDefinition;// configure to detect and act on class definition, these will stop further rule checking by returning false
	pPushClass = new PushClass(pRepo);  // no action
	pPrintClass = new PrintClass(pRepo);
	pClassDefinition->addAction(pPushClass);
	pClassDefinition->addAction(pPrintClass);
	pParser->addRule(pClassDefinition);
	pStructDefinition = new StructDefinition; // configure to detect and act on struct definitions, these will stop further rule checking by returning false
	pPushStruct = new PushStruct(pRepo);  // no action
	pPrintStruct = new PrintStruct(pRepo);
	pStructDefinition->addAction(pPushStruct);
	pStructDefinition->addAction(pPrintStruct); pParser->addRule(pStructDefinition);
    pFunctionDefinition = new FunctionDefinition; // configure to detect and act on function definitions, these will stop further rule checking by returning false
    pPushFunction = new PushFunction(pRepo);  // no action
    pPrintFunction = new PrintFunction(pRepo);
    pFunctionDefinition->addAction(pPushFunction);
    pFunctionDefinition->addAction(pPrintFunction); pParser->addRule(pFunctionDefinition);
	pControlStatementDefinition = new ControlStatementDefinition; //configure to detect and act on function definitions, these will stop further rule checking by returning false
	pPushControlStatement = new PushControlStatement(pRepo);  // no action
	pPrintControlStatement = new PrintControlStatement(pRepo);
	pControlStatementDefinition->addAction(pPushControlStatement);
	pControlStatementDefinition->addAction(pPrintControlStatement); pParser->addRule(pControlStatementDefinition);
	pNamespaceDefinition = new NamespaceDefinition;
	pPushNamespace = new pushNamespace(pRepo);
	pPrintNamespace = new PrintNamespace(pRepo);
	pNamespaceDefinition->addAction(pPushNamespace);
	pNamespaceDefinition->addAction(pPrintNamespace); pParser->addRule(pNamespaceDefinition);
    pDeclaration = new Declaration;  // configure to detect and act on declarations and Executables
    pShowDeclaration = new ShowDeclaration;
    pDeclaration->addAction(pShowDeclaration); pParser->addRule(pDeclaration);
    pExecutable = new Executable;
    pShowExecutable = new ShowExecutable;
    pExecutable->addAction(pShowExecutable); pParser->addRule(pExecutable);
    return pParser; 
  }
  catch(std::exception& ex){std::cout << "\n\n  " << ex.what() << "\n\n"; return 0;}
}

#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ConfigureParser module\n "
            << std::string(32,'=') << std::endl;

  // collecting tokens from files, named on the command line

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-');

    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << argv[i] << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }
      // now that parser is built, use it

      while(pParser->next())
        pParser->parse();
      std::cout << "\n\n";
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    std::cout << "\n\n";
  }
}

#endif
