#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
////////////////////////////////////////////////////////////////////////////
// ActionsAndRules.h - declares new parsing rules and actions			  //
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
  This module defines several action classes.  Its classes provide 
  specialized services needed for specific applications.  The modules
  Parser, SemiExp, and Tokenizer, are intended to be reusable
  without change.  This module provides a place to put extensions of
  these facilities and is not expected to be reusable. 

  Public Interface:
  =================
  Toker t(someFile);              // create tokenizer instance
  SemiExp se(&t);                 // create a SemiExp attached to tokenizer
  Parser parser(se);              // now we have a parser
  Rule1 r1;                       // create instance of a derived Rule class
  Action1 a1;                     // create a derived action
  r1.addAction(&a1);              // register action with the rule
  parser.addRule(&r1);            // register rule with parser
  while(se.getSemiExp())          // get semi-expression
    parser.parse();               //   and parse it

  Build Process:
  ==============
  Required files
    - Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp, AST.cpp, AST.h
      ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
      ItokCollection.h, SemiExp.h, SemiExp.cpp, Tokenizer.h, Tokenizer.cpp
  Build commands
    - devenv Project2.sln /rebuild debug

  Maintenance History:
  ====================
  * ver 1.0 : 12 March 16
  * - first release
*/

#include <queue>
#include <string>
#include <sstream>
#include "../AST/AST.h"
#include "Parser.h"
#include "../SemiExp/itokcollection.h"
#include "../ScopeStack/ScopeStack.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"

///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.
class Repository  // application specific
{
private:
  ScopeStack<element*> stack;
  Scanner::Toker* p_Toker;
  AST *ast;
  static Repository* instance;
public:
  Repository(Scanner::Toker* pToker)
  {
    instance = this;
    p_Toker = pToker;
	ast = new AST;
	element *root = new element;
	root->type = "namespace";
	root->name = "Global";
	root->lineCount = 0;
	ast->setRoot(root);
	stack.push(root);
  }
  ScopeStack<element*>& scopeStack()
  {
    return stack;
  }
  Scanner::Toker* Toker()
  {
    return p_Toker;
  }
  size_t lineCount()
  {
    return (size_t)(p_Toker->currentLineCount());
  }
  AST *retrieveAST()
  {
	  return ast;
  }
  static Repository* getInstance()
  {
	  return instance;
  }
};

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("{") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePush : public IAction
{
  Repository* p_Repos;
public:
  HandlePush(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    //std::cout << "\n--BeginningOfScope rule";
    element* elem = new element;
    elem->type = "unknown";
    elem->name = "anonymous";
    elem->startCount = p_Repos->lineCount();
	p_Repos->scopeStack().top()->insertChild(elem);
	p_Repos->scopeStack().push(elem);
  }
};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("}") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
  Repository* p_Repos;
public:
  HandlePop(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    //std::cout << "\n--EndOfScope rule";
    if(p_Repos->scopeStack().size() == 0)
      return;
    element* elem = p_Repos->scopeStack().pop();
	elem->endCount = p_Repos->lineCount();
	elem->lineCount = elem->endCount - elem->startCount + 1;
	elem->complexityCount = p_Repos->retrieveAST()->complexity(elem);
   /* if(elem->type == "function")
    {
      //std::cout << "\nHandlePop";
      //std::cout << "\n--popping at line count = " << p_Repos->lineCount();
      std::cout << "\n  Function " << elem->name << ", lines = " << elem->lineCount;
      std::cout << "\n";
    }
	if (elem->type == "class")
	{
		std::cout << "\n  Class " << elem->name << ", lines = " << elem->lineCount;
		std::cout << "\n";
	}
	if (elem->type == "struct")
	{
		std::cout << "\n  Struct " << elem->name << ", lines = " << elem->lineCount;
		std::cout << "\n";
	}
	if (elem->type == "controlStatement")
	{
		std::cout << "\n  ControlStatement " << elem->name << ", lines = " << elem->lineCount;
		std::cout << "\n";
	}*/
  }
};

///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements

class PreprocStatement : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("#") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to print preprocessor statement to console

/*class PrintPreproc : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
  }
};*/
///////////////////////////////////////////////////////////////
// rule to detect class definitions
class ClassDefinition : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		size_t len = tc.find("class");
		if (len < tc.length())
		{
				doActions(pTc);
				return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push class name onto ScopeStack

class PushClass : public IAction
{
	Repository* p_Repos;
public:
	PushClass(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// next statement is now  handled in PrintClass
		// std::cout << "\n  ClassDef: " << pTc->show();
		element *temp;
		// pop anonymous scope
		temp = p_Repos->scopeStack().pop();
		p_Repos->retrieveAST()->deleteChild(p_Repos->scopeStack().top(), temp);
		// push class scope
		std::string name = (*pTc)[pTc->find("class") + 1];
		element* elem = new element;
		elem->type = "class";
		elem->name = name;
		elem->startCount = p_Repos->lineCount();
		p_Repos->scopeStack().top()->insertChild(elem);
		p_Repos->scopeStack().push(elem);
	}
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a class def
// to console

class PrintClass : public IAction
{
	Repository* p_Repos;
public:
	PrintClass(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//std::cout << "\n  ClassDef: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect struct definitions
class StructDefinition : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		size_t len = tc.find("struct");
		if (len < tc.length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push struct name onto ScopeStack

class PushStruct : public IAction
{
	Repository* p_Repos;
public:
	PushStruct(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// next statement is now  handled in PrintStruct
		// std::cout << "\n  StructDef: " << pTc->show();
		element *temp;
		// pop anonymous scope
		temp = p_Repos->scopeStack().pop();
		p_Repos->retrieveAST()->deleteChild(p_Repos->scopeStack().top(), temp);
		// push struct scope
		std::string name = (*pTc)[pTc->find("struct") + 1];
		element* elem = new element;
		elem->type = "struct";
		elem->name = name;
		elem->startCount = p_Repos->lineCount();
		p_Repos->scopeStack().top()->insertChild(elem);
		p_Repos->scopeStack().push(elem);
	}
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a class def
// to console

class PrintStruct : public IAction
{
	Repository* p_Repos;
public:
	PrintStruct(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//std::cout << "\n  StructDef: " << pTc->show().c_str();
	}
};
//--------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////
// Rule to detect beginning of namespace
class NamespaceDefinition : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("namespace") < pTc->length() && !(pTc->find("using")<pTc->length()))
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push namespace statement to repo
class pushNamespace : public IAction
{
	Repository* p_Repos;
public:
	pushNamespace(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		element* temp = p_Repos->scopeStack().pop();
		p_Repos->retrieveAST()->deleteChild(p_Repos->scopeStack().top(), temp);

		std::string name = (*pTc)[pTc->find("namespace") + 1];
		element *elem = new element;
		elem->type = "Namespace";
		elem->name = name;
		elem->startCount = p_Repos->lineCount();
		p_Repos->scopeStack().top()->insertChild(elem);
		p_Repos->scopeStack().push(elem);
	}
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a class def
// to console

class PrintNamespace : public IAction
{
	Repository* p_Repos;
public:
	PrintNamespace(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//std::cout << "\n  Namespace: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect function definitions

class FunctionDefinition : public IRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for(int i=0; i<5; ++i)
      if(tok == keys[i])
        return true;
    return false;
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if(tc[tc.length()-1] == "{")
    {
      size_t len = tc.find("(");
      if(len < tc.length() && !isSpecialKeyWord(tc[len-1]))
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction
{
  Repository* p_Repos;
public:
  PushFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    // next statement is now  handled in PrintFunction
    // std::cout << "\n  FunctionDef: " << pTc->show();
	element *temp;
    // pop anonymous scope
    temp = p_Repos->scopeStack().pop();
	p_Repos->retrieveAST()->deleteChild(p_Repos->scopeStack().top(), temp);
    // push function scope
    std::string name = (*pTc)[pTc->find("(") - 1];
    element* elem = new element;
    elem->type = "function";
    elem->name = name;
    elem->startCount = p_Repos->lineCount();
	p_Repos->scopeStack().top()->insertChild(elem);
	p_Repos->scopeStack().push(elem);
  }
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a function def
// to console

class PrintFunction : public IAction
{
  Repository* p_Repos;
public:
  PrintFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    //std::cout << "\n  FuncDef: " << pTc->show().c_str();
  }
};

///////////////////////////////////////////////////////////////
// action to send signature of a function def to console

class PrettyPrintFunction : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    pTc->remove("public");
    pTc->remove(":");
    pTc->trimFront();
    int len = pTc->find(")");
    std::cout << "\n\n  Pretty Stmt:    ";
    for(int i=0; i<len+1; ++i)
      std::cout << (*pTc)[i] << " ";
    std::cout << "\n";
  }
};

//------------------------------------------------------------
///////////////////////////////////////////////////////////////
// rule to detect controlStatement definitions

class ControlStatementDefinition : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i<5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			if ((len < tc.length() && isSpecialKeyWord(tc[len - 1])) || (tc.find("else") < tc.length()) || (tc.find("try") < tc.length()) || (tc.find("do") < tc.length()))
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push controlStatement name onto ScopeStack

class PushControlStatement : public IAction
{
	Repository* p_Repos;
public:
	PushControlStatement(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// next statement is now  handled in PrintControlStatement
		// std::cout << "\n  FunctionDef: " << pTc->show();
		element *temp;
		// pop anonymous scope
		temp = p_Repos->scopeStack().pop();
		p_Repos->retrieveAST()->deleteChild(p_Repos->scopeStack().top(), temp);
		// push controlStatement scope
		std::string name;

		ControlStatementDefinition cs;
		bool b = cs.isSpecialKeyWord((*pTc)[pTc->find("(") - 1]);
		if (b)
			name = (*pTc)[pTc->find("(") - 1];
		else if(pTc->find("else") < pTc->length())
			name = (*pTc)[pTc->find("else")];
		else if(pTc->find("try") < pTc->length())
			name = (*pTc)[pTc->find("try")];
		else
			name = (*pTc)[pTc->find("do")];
		
		element* elem = new element;
		elem->type = "controlStatement";
		elem->name = name;
		elem->startCount = p_Repos->lineCount();
		p_Repos->scopeStack().top()->insertChild(elem);
		p_Repos->scopeStack().push(elem);
	}
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a controlStatement def
// to console

class PrintControlStatement : public IAction
{
	Repository* p_Repos;
public:
	PrintControlStatement(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//std::cout << "\n  ControlStatementDef: " << pTc->show().c_str();
	}
};

//------------------------------------------------------------
///////////////////////////////////////////////////////////////
// rule to detect declaration

class Declaration : public IRule          // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
  bool isModifier(const std::string& tok) // modifiers and initializers.
  {                                       // If you have two things left
    const size_t numKeys = 22;            // its declar else executable.
    const static std::string keys[numKeys] = {
      "const", "extern", "friend", "mutable", "signed", "static",
      "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
      "public", "protected", "private", ":", "typename", "typedef", "++", "--"
    };
    for (int i = 0; i<numKeys; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void condenseTemplateTypes(ITokCollection& tc)
  {
    size_t start = tc.find("<");
    size_t end = tc.find(">");
    if (start >= end || start == 0)
      return;
    else
    {
      if (end == tc.length())
        end = tc.find(">::");
      if (end == tc.length())
        return;
    }
    std::string save = tc[end];
    std::string tok = tc[start - 1];
    for (size_t i = start; i < end + 1; ++i)
      tok += tc[i];
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
    if (save == ">::")
    {
      tok += tc[start + 1];
      tc.remove(start);
    }
    tc[start - 1] = tok;
    //std::cout << "\n  -- " << tc.show();
  }
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for (int i = 0; i<5; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void removeInvocationParens(ITokCollection& tc)
  {
    size_t start = tc.find("(");
    size_t end = tc.find(")");
    if (start >= end || end == tc.length() || start == 0)
      return;
    if (isSpecialKeyWord(tc[start - 1]))
      return;
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
    //std::cout << "\n  -- " << tc.show();
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& in = *pTc;
    Scanner::SemiExp tc;
    for (size_t i = 0; i<in.length(); ++i)
      tc.push_back(in[i]);

    if (tc[tc.length() - 1] == ";" && tc.length() > 2)
    {
      //std::cout << "\n  ++ " << tc.show();
      removeInvocationParens(tc);
      condenseTemplateTypes(tc);

      // remove modifiers, comments, newlines, returns, and initializers

      Scanner::SemiExp se;
      for (size_t i = 0; i < tc.length(); ++i)
      {
        if (isModifier(tc[i]) || se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
          continue;
       // if (se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
         //  continue;
        if (tc[i] == "=" || tc[i] == ";")
          break;
        else
          se.push_back(tc[i]);
      }
      //std::cout << "\n  ** " << se.show();
      if (se.length() == 2)  // type & name, so declaration
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to show declaration

class ShowDeclaration : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    // remove comments
    Scanner::SemiExp se;
    for (size_t i = 0; i<tc.length(); ++i)
      if (!se.isComment(tc[i]))
        se.push_back(tc[i]);
    // show cleaned semiExp
   // std::cout << "\n  Declaration: " << se.show();
  }
};

///////////////////////////////////////////////////////////////
// rule to detect executable

class Executable : public IRule           // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
  bool isModifier(const std::string& tok) // modifiers and initializers.
  {                                       // If you have two things left
    const size_t numKeys = 22;            // its declar else executable.
    const static std::string keys[numKeys] = {
      "const", "extern", "friend", "mutable", "signed", "static",
      "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
      "public", "protected", "private", ":", "typename", "typedef", "++", "--"
    };
    for (int i = 0; i<numKeys; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void condenseTemplateTypes(ITokCollection& tc)
  {
    size_t start = tc.find("<");
    size_t end = tc.find(">");
    if (start >= end || start == 0)
      return;
    else
    {
      if (end == tc.length())
        end = tc.find(">::");
      if (end == tc.length())
        return;
    }
    std::string save = tc[end];
    std::string tok = tc[start - 1];
    for (size_t i = start; i < end+1; ++i)
      tok += tc[i];
    for (size_t i = start; i < end+1; ++i)
      tc.remove(start);
    if (save == ">::")
    {
      tok += tc[start + 1];
      tc.remove(start);
    }
    tc[start - 1] = tok;
    //std::cout << "\n  -- " << tc.show();
  }
  
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for (int i = 0; i<5; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void removeInvocationParens(ITokCollection& tc)
  {
    size_t start = tc.find("(");
    size_t end = tc.find(")");
    if (start >= end || end == tc.length() || start == 0)
      return;
    if (isSpecialKeyWord(tc[start - 1]))
      return;
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
    //std::cout << "\n  -- " << tc.show();
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& in = *pTc;
    Scanner::SemiExp tc;
    for (size_t i = 0; i<in.length(); ++i)
      tc.push_back(in[i]);

    if (tc[tc.length() - 1] == ";" && tc.length() > 2)
    {
      //std::cout << "\n  ++ " << tc.show();
      removeInvocationParens(tc);
      condenseTemplateTypes(tc);

      // remove modifiers, comments, newlines, returns, and initializers

      Scanner::SemiExp se;
      for (size_t i = 0; i < tc.length(); ++i)
      {
        if (isModifier(tc[i]) || se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
          continue;
      //  if (se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return")
       //   continue;
        if (tc[i] == "=" || tc[i] == ";")
          break;
        else
          se.push_back(tc[i]);
      }
      //std::cout << "\n  ** " << se.show();
      if (se.length() != 2)  // not a declaration
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

///////////////////////////////////////////////////////////////
// action to show executable

class ShowExecutable : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    // remove comments
    Scanner::SemiExp se;
    for (size_t i = 0; i < tc.length(); ++i)
    {
      if (!se.isComment(tc[i]))
        se.push_back(tc[i]);
    }
    // show cleaned semiExp
    //std::cout << "\n  Executable: " << se.show();
  }
};

#endif
