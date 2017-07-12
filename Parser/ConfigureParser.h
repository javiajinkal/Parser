#ifndef CONFIGUREPARSER_H
#define CONFIGUREPARSER_H
////////////////////////////////////////////////////////////////////////////
// ConfigureParser.h - builds and configures parsers                      //
// ver 1.0                                                                //
// Language:    C++, Visual Studio 2015                                   //
// Application: Project2 , CSE687 - Object Oriented Design				  //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10						  //
// Author:      Jinkal Arvind Javia, SUID: 425325424     				  //
//              jjavia@syr.edu											  //
// Source:		Jim Fawcett, Syracuse University, CST 4-187				  //
//              jfawcett@twcny.rr.com								      //
////////////////////////////////////////////////////////////////////////////

/*
  Module Operations: 
  ==================
  This module builds and configures parsers.  It builds the parser
  parts and configures them with application specific rules and actions.

  Public Interface:
  =================
  ConfigParseToConsole conConfig;
  conConfig.Build();
  conConfig.Attach(someFileName);

  Build Process:
  ==============
  Required files
    - ConfigureParser.h, ConfigureParser.cpp, Parser.h, Parser.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp,
      SemiExp.h, SemiExp.cpp, Tokenizer.h, Tokenizer.cpp
  Build commands
    - devenv Project2.sln /rebuild debug

  Maintenance History:
  ====================
  * ver 1.0 : 12 March 16
  * - first release
*/

#include <fstream>
#include "Parser.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "ActionsAndRules.h"
#include "../AST/AST.h"

///////////////////////////////////////////////////////////////
// build parser that writes its output to console

class ConfigParseToConsole : IBuilder{
public:
  ConfigParseToConsole() {};
  ~ConfigParseToConsole();
  bool Attach(const std::string& name, bool isFile=true);
  Parser* Build();
private:
  // Builder must hold onto all the pieces
  std::ifstream* pIn;
  Scanner::Toker* pToker;
  Scanner::SemiExp* pSemi;
  Parser* pParser;
  Repository* pRepo;
  // add Rules and Actions
  BeginningOfScope* pBeginningOfScope;
  HandlePush* pHandlePush;
  EndOfScope* pEndOfScope;
  HandlePop* pHandlePop;
  FunctionDefinition* pFunctionDefinition;
  PushFunction* pPushFunction;
  PrintFunction* pPrintFunction;
  Declaration* pDeclaration;
  ShowDeclaration* pShowDeclaration;
  Executable* pExecutable;
  ShowExecutable* pShowExecutable;
  ClassDefinition* pClassDefinition;
  PushClass* pPushClass;
  PrintClass* pPrintClass;
  StructDefinition* pStructDefinition;
  PushStruct* pPushStruct;
  PrintStruct* pPrintStruct;
  ControlStatementDefinition* pControlStatementDefinition;
  PushControlStatement* pPushControlStatement;
  PrintControlStatement* pPrintControlStatement;
  NamespaceDefinition* pNamespaceDefinition;
  pushNamespace* pPushNamespace;
  PrintNamespace* pPrintNamespace;
  ConfigParseToConsole(const ConfigParseToConsole&) = delete; // prohibit copies and assignments
  ConfigParseToConsole& operator=(const ConfigParseToConsole&) = delete;
};


#endif
