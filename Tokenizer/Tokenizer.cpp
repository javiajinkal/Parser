/////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream                   //
// ver 1.0                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Project2 , CSE687 - Object Oriented Design         //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10				   //
// Author:      Jinkal Arvind Javia, SUID: 425325424     	       //
//              jjavia@syr.edu		                               //
// Source:		Jim Fawcett, Syracuse University, CST 4-187        //
//              jfawcett@twcny.rr.com				               //
/////////////////////////////////////////////////////////////////////

#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>
#include <map>

namespace Scanner
{
  class ConsumeState
  {
  public:
	using Token = std::string;
    ConsumeState();
	ConsumeState(const ConsumeState&) = delete;
	ConsumeState& operator=(const ConsumeState&) = delete;
    virtual ~ConsumeState();
    void attach(std::istream* pIn) { _pIn = pIn; }
    virtual void eatChars() = 0;
    void consumeChars() {
      _pState->eatChars();
      _pState = nextState();
    }
    bool canRead() { return _pIn->good(); }
	void print_state( std::istream* stream ) {
		std::cout << " good()=" << stream->good();
		std::cout << " eof()=" << stream->eof();
	}
    std::string getTok() { return token; }
    bool hasTok() { return token.size() > 0; }
    ConsumeState* nextState();
	void setSpecialSingleChars(std::string ssc);
	void setSpecialCharPairs(std::string scp);
	void returnComments(bool doReturnComments = false);
	size_t currentLineCount();
	std::string checkFlag(bool flag, int nextchar, std::string charcurr, std::string charnext);
	ConsumeState* checkpunct(int, std::string, std::string);
	void reset() {
		firstC = true;
	}
  protected:
	static bool firstC;
	bool collectChar();
    static std::string token;
    static std::istream* _pIn;
    static int prevChar;
    static int currChar;
	static bool _doReturnComments;
	static size_t _lineCount;
	static bool show_comment;
    static ConsumeState* _pState;
    static ConsumeState* _pEatCppComment;
    static ConsumeState* _pEatCComment;
    static ConsumeState* _pEatWhitespace;
    static ConsumeState* _pEatPunctuator;
    static ConsumeState* _pEatAlphanum;
    static ConsumeState* _pEatNewline;
	static ConsumeState* _pEatSingleSpecialChar;
	static ConsumeState* _pEatDoubleSpecialChar;
	static ConsumeState* _pEatString;
	static ConsumeState* _pEatChar;
	static std::map<std::string, int> singleSpecialChar;
	static std::map<std::string, int> doubleSpecialChar;
  };
}

using namespace Scanner;
using Token = std::string;
std::string ConsumeState::token;
bool ConsumeState::firstC=true;
std::istream* ConsumeState::_pIn = nullptr;
int ConsumeState::prevChar;
int ConsumeState::currChar;
bool ConsumeState::_doReturnComments = false;
size_t ConsumeState::_lineCount = 1;
bool ConsumeState::show_comment = true;
ConsumeState* ConsumeState::_pState = nullptr;
ConsumeState* ConsumeState::_pEatCppComment = nullptr;
ConsumeState* ConsumeState::_pEatCComment = nullptr;
ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
ConsumeState* ConsumeState::_pEatNewline;
ConsumeState* ConsumeState::_pEatSingleSpecialChar = nullptr;
ConsumeState* ConsumeState::_pEatDoubleSpecialChar = nullptr;
ConsumeState* ConsumeState::_pEatString = nullptr;
ConsumeState* ConsumeState::_pEatChar = nullptr;
std::map<std::string, int> ConsumeState::singleSpecialChar = { {"<",1}, {">",2}, {"[",3}, {"]",4}, {"(",5}, {")",6}, {"{",7}, {"}",8}, {":",9}, {"=",10}, {"+",11}, {"-",12}, {"*",13}, {"\\n",14}};
std::map<std::string, int> ConsumeState::doubleSpecialChar = { {"<<",1}, {">>",2}, {"::",3}, {"++",4}, {"--",5}, {"==",6}, {"+=",7}, {"-=",8}, {"*=",9}, {"/=",10}};
void testLog(const std::string& msg);

//Checks what the nextState is for the input stream
ConsumeState* ConsumeState::nextState()
{
  if (!(_pIn->good()))
	return nullptr;
  int chNext = _pIn->peek();
  bool flag = true;
  std::string current, next, specialChar;
  current = currChar;
  specialChar = checkFlag(flag,chNext,current,next);
  if (currChar == '/' && chNext == '/') {
	  testLog("state: eatCppComment");
	  return _pEatCppComment;
  }
  if (currChar == '/' && chNext == '*') {
	  testLog("state: eatCComment");
	  return _pEatCComment;
  }
  if (currChar == '\'' ){
	  testLog("state: eatChar");
	  return _pEatChar;
  }
  if (currChar == '"'){
	  testLog("state: eatString");
	  return _pEatString;
  }
  if (std::isspace(currChar) && currChar != '\n'){
    testLog("state: eatWhitespace");
    return _pEatWhitespace;
  }
  if (currChar == '\n'){
    testLog("state: eatNewLine");
    return _pEatNewline;
  } 
  if (std::isalnum(currChar)){
    testLog("state: eatAlphanum");
    return _pEatAlphanum;
  }
  ConsumeState* punct = checkpunct(currChar, current, specialChar);
  if (punct!= nullptr)
	  return punct;
  if (!_pIn->good())
	  return _pEatWhitespace;
  throw(std::logic_error("invalid type"));
}

//Eats Whitespace whenever they are encountered
class EatWhitespace : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    do {
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (std::isspace(currChar) && currChar != '\n');
  }
};

/*EatCppComment eats Cpp comment when show_comment is false,
else it returns Cpp comment as single token*/
class EatCppComment : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    do {
		if(_doReturnComments)
			token += currChar;
		if (!_pIn->good())  // end of stream
			return;
      currChar = _pIn->get();
    } while (currChar != '\n');
  }
};

/*EatCComment eats C comment when show_comment is false,
else it returns C comment as single token*/
class EatCComment : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    do {
		if (!_pIn->good())  // end of stream
			return;
		if (currChar == '\n')
			++_lineCount;
		if(_doReturnComments)
			token += currChar; 	
      currChar = _pIn->get();
    } while (currChar != '*' || _pIn->peek() != '/');
	if (currChar == '*')
	{
		if(_doReturnComments)
			token += currChar;
		currChar = _pIn->get();
	}
	if (currChar == '/')
	{
		if(_doReturnComments)
			token += currChar;
	}
    _pIn->get();
    currChar = _pIn->get();
  }
};

/*EatSingleSpecialChar returns single token for each single special
character encountered*/
class EatSingleSpecialChar : public ConsumeState
{
public:
	virtual void eatChars()
	{
		std::string current, next, specialChar;
		current = currChar;
		next = _pIn->peek();
		specialChar = current + next;
		token.clear();
		//if (!_pIn->good())  // end of stream
		//	return;
	    if ((singleSpecialChar.find(current) != singleSpecialChar.end()))
		{
				token += currChar;
		}
		else if ((singleSpecialChar.find(specialChar) != singleSpecialChar.end()))
		{
			    token += currChar;
			    currChar = _pIn->get();
			    token += currChar;
		}
		currChar = _pIn->get();
	}
};

/*EatDoubleSpecialChar returns single token for each double special
character encountered*/
class EatDoubleSpecialChar : public ConsumeState
{
public:
	virtual void eatChars()
	{
		std::string current, next,specialChar;
		current = currChar;
		next = _pIn->peek();
		specialChar = current + next;
		token.clear();
		if (!_pIn->good())  // end of stream
			return;
		if (doubleSpecialChar.find(specialChar) != doubleSpecialChar.end())
		{
			token += currChar;
			currChar = _pIn->get();
			token += currChar;
		}
		currChar = _pIn->get();
	}
};

/*EatChar returns single token for each character encountered*/
class EatChar : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
		if (currChar == '\'')
		{
			currChar = _pIn->get();
			return;
		}
		if (currChar == '\\')
		{
			if (_pIn->peek() != '\"' && _pIn->peek() != '\'')
				token += currChar;
			currChar = _pIn->get();
			token += currChar;
			currChar = _pIn->get();
			currChar = _pIn->get();
			return;
		}
		token += currChar;
		currChar = _pIn->get();
		currChar = _pIn->get();
	}
};

/*EatString returns single token for each string encountered*/
class EatString : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
		if (currChar == '"')
		{ 
			currChar = _pIn->get();
			return;
		}
		else {
			do {
				if (!_pIn->good())  // end of stream
					return;
				if((currChar == '\\' && _pIn->peek() == '"')|| (currChar == '\\' && _pIn->peek() == '\''))
				{
					    currChar = _pIn->get();
						token += currChar;
						currChar = _pIn->get();
				}
				else if (currChar == '\\' && _pIn->peek() == '\\')
				{
					token += currChar;
					currChar = _pIn->get();
					token += currChar;
					currChar = _pIn->get();
				}
				else {
					token += currChar;
					currChar = _pIn->get();
				}
			} while (currChar != '"');
			currChar = _pIn->get();
		}
	}
};

/*EatPunctuator returns single token for punctuators encountered*/
class EatPunctuator : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
	std::string specialChar;
	std::string current, next;
    do {
      token += currChar;
      if (!_pIn->good())  // end of stream
        return;
	  currChar = _pIn->get();
	  current = currChar;
	  next = _pIn->peek();
	  specialChar = current + next;
    } while (ispunct(currChar) && currChar!='/' && currChar!='\'' && currChar != '\"' && (doubleSpecialChar.find(specialChar) == doubleSpecialChar.end()) && (singleSpecialChar.find(current) == singleSpecialChar.end()));
  }
};

/*EatAlphanum returns single token for alphanums when encountered*/
class EatAlphanum : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    do {
      token += currChar;
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (isalnum(currChar));
  }
};

/*EatNewLine returns single token when newline is encountered*/
class EatNewline : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
	if (currChar == '\n')
	{
		++_lineCount;
		token += currChar;
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
	}
  }
};

//Function to check if next state is EatPunctuator OR EatDoubleSpecialChar OR EatSingleSpecialChar
ConsumeState* Scanner::ConsumeState::checkpunct(int current, std::string specialChar1,std::string specialChar2)
{
	if (ispunct(currChar) && (doubleSpecialChar.find(specialChar2) == doubleSpecialChar.end()) && (singleSpecialChar.find(specialChar2) == singleSpecialChar.end()) &&(singleSpecialChar.find(specialChar1) == singleSpecialChar.end())) {
		testLog("state: eatPunctuator");
		return _pEatPunctuator;
	}
	if (doubleSpecialChar.find(specialChar2) != doubleSpecialChar.end()) {
		testLog("state: eatDoubleSpecialChar");
		return _pEatDoubleSpecialChar;
	}
	if (singleSpecialChar.find(specialChar1) != singleSpecialChar.end() || (singleSpecialChar.find(specialChar2) != singleSpecialChar.end())) {
		testLog("state: eatSingleSpecialChar");
		return _pEatSingleSpecialChar;
	}
	return nullptr;
}

//Function to check if End of file is detected and sets flag accordingly
std::string Scanner::ConsumeState::checkFlag(bool flag,int nextchar,std::string charcurr, std::string charnext)
{
	std::string specialChar;
	//if (!(_pIn->good()))
		//return "";
	if (nextchar == EOF) {
		_pIn->clear(); //clear() restores state to good
		flag = false;
		std::string temp;
		temp += currChar;
		return temp;
	}
	if (flag) {
		charcurr = currChar;
	    charnext = _pIn->peek();
		specialChar = charcurr + charnext;
	}
	return specialChar;
}

//Function setSpecialSingleChars(string ssc): to change defaults of Single Special Character tokens
void Scanner::ConsumeState::setSpecialSingleChars(std::string ssc)
{
	std::string::size_type p = 0;
	std::string::size_type q = ssc.find(',');
	if (q == std::string::npos)
	{
		singleSpecialChar[ssc] = singleSpecialChar.size() + 1;
	}
	while (q != std::string::npos) 
	{
		singleSpecialChar[ssc.substr(p, q - p)] = singleSpecialChar.size() + 1;
		p = ++p;
		q = ssc.find(',', q);
		if (q == std::string::npos)
			singleSpecialChar[ssc.substr(p, ssc.length())] = singleSpecialChar.size() + 1;
	}
}

void::Scanner::Toker::setSpecialSingleChars(std::string ssc)
{
	pConsumer->setSpecialSingleChars(ssc);
}

//Function setSpecialCharPairs(std::string spc): to change defaults of Double Special Character tokens
void Scanner::ConsumeState::setSpecialCharPairs(std::string spc)
{
	std::string::size_type p = 0;
	std::string::size_type q = spc.find(',');
	if (q == std::string::npos)
	{
		doubleSpecialChar[spc] = doubleSpecialChar.size() + 1;
	}
	while (q != std::string::npos) {
		doubleSpecialChar[spc.substr(p, q - p)] = doubleSpecialChar.size() + 1;
		p = ++q;
		q = spc.find(',', q);
		if (q == std::string::npos)
			doubleSpecialChar[spc.substr(p, spc.length())] = doubleSpecialChar.size() + 1;
	}
}

void::Scanner::Toker::setSpecialCharPairs(std::string spc)
{
	pConsumer->setSpecialCharPairs(spc);
}
//----< turn on\off returning comments as tokens >-------------------

void ConsumeState::returnComments(bool doReturnComments)
{
	_doReturnComments = doReturnComments;
}

//----< return number of newlines collected from stream >------------

size_t ConsumeState::currentLineCount() { return _lineCount; }

//----< collect character from stream >------------------------------

bool ConsumeState::collectChar()
{
	if (_pIn->good())
	{
		prevChar = currChar;
		currChar = _pIn->get();
		if (currChar == '\n')
			++_lineCount;
		return true;
	}
	return false;
}

//----< start/stop returning comments as tokens >--------------------

void Toker::returnComments(bool doReturnComments)
{
	pConsumer->returnComments(doReturnComments);
}
//-----< return number of newlines pulled from stream >--------------

size_t Toker::currentLineCount()
{
	return pConsumer->currentLineCount();
}
//Resets pConsumer
void Scanner::Toker::reset()
{
	pConsumer->reset();
	pConsumer = new EatWhitespace();
}

//ConsumeState Constructor 
ConsumeState::ConsumeState()
{
  static bool first = true;
  if (firstC)
  {
	  firstC = false;
    _pEatAlphanum = new EatAlphanum();
    _pEatCComment = new EatCComment();
    _pEatCppComment = new EatCppComment();
    _pEatPunctuator = new EatPunctuator();
    _pEatWhitespace = new EatWhitespace();
    _pEatNewline = new EatNewline();
	_pEatDoubleSpecialChar = new EatDoubleSpecialChar();
	_pEatSingleSpecialChar = new EatSingleSpecialChar();
	_pEatString = new EatString();
	_pEatChar = new EatChar();
    _pState = _pEatWhitespace;
  }
}

//ConsumeState Destructor
ConsumeState::~ConsumeState()
{
  static bool first = true;
  if (first)
  {
    first = false;
    delete _pEatAlphanum;
    delete _pEatCComment;
    delete _pEatCppComment;
    delete _pEatPunctuator;
    delete _pEatWhitespace;
    delete _pEatNewline;
	delete _pEatSingleSpecialChar;
	delete _pEatDoubleSpecialChar;
	delete _pEatString;
	delete _pEatChar;
  }
}


Toker::Toker() : pConsumer(new EatWhitespace()) { pConsumer->reset(); }

Toker::~Toker() { delete pConsumer; }

//returns true if attaching pConsumer pointer to input stream is successful
bool Toker::attach(std::istream* pIn)
{
  if (pIn != nullptr && pIn->good())
  {
    pConsumer->attach(pIn);
    return true;
  }
  return false;
}

//gets the tokens
std::string Toker::getTok()
{
  while(true) 
  {
    if (!pConsumer->canRead())
      return "";
    pConsumer->consumeChars();
    if (pConsumer->hasTok())
      break;
  }
  return pConsumer->getTok();
}

bool Toker::canRead() { return pConsumer->canRead(); }

void testLog(const std::string& msg)
{
#ifdef TEST_LOG
  std::cout << "\n  " << msg;
#endif
}

//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{  
  std::string fileSpec = "../Tokenizer/test.txt";
  //std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
  //std::string fileSpec = "../Tokenizer/Tokenizer.h";
 
  std::ifstream in(fileSpec);
  if (!in.good())
  {
    std::cout << "\n  can't open " << fileSpec << "\n\n";
    return 1;
  }
  Toker toker;
  toker.attach(&in);
 do
  {
    std::string tok = toker.getTok();
    if (tok == "\n")
      tok = "newline";
    std::cout << "\n -- " << tok;
 } while (in.good());
  std::cout << "\n\n";
  return 0;
}
#endif