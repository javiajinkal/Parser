///////////////////////////////////////////////////////////////////////
// SemiExp.cpp - collect tokens for analysis                         //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Project2 , CSE687 - Object Oriented Design           //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10				     //
// Author:      Jinkal Arvind Javia, SUID: 425325424     	         //
//              jjavia@syr.edu		                                 //
// Source:		Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com								 //
///////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <exception>
#include "SemiExp.h"
#include "../Tokenizer/Tokenizer.h"

using namespace Scanner;

SemiExp::SemiExp(Toker* pToker) : _pToker(pToker) {}
std::string handleFor(std::string);
bool isComment(std::string);
bool isTerminator(std::string);
bool isPublicPrivateProtected(std::string, std::string);

//returns true when SemiExpression is collected
bool SemiExp::get(bool clear)
{
  if (clear)
		_tokens.clear();
  std::string previous;
  bool flag = false;
  while (true)
  {
	std::string token = _pToker->getTok();
    if (token == "")
      break;
	_tokens.push_back(token);
	if (token == "\n" && flag == true)
	{
	    flag = false;
		return true;
	}
	if (token == "#")
	{
		flag = true;
		continue;
	}
	if (token == "for")
	{
		token = handleFor(token);
		return true;
	}
	bool testPublicPrivateProtected = isPublicPrivateProtected(token, previous);
	bool testTerminator = isTerminator(token);
	bool testcomment = isComment(token);
	if (testcomment || testTerminator || testPublicPrivateProtected)
	{
		return true;
	}
	previous = token;
  }
  return false;
}

//returns true if access modifier is detected, else returns false
bool SemiExp::isPublicPrivateProtected(std::string t1,std::string t2)
{
	if (t1 == ":")
	{
		if (t2 == "public" || t2 == "protected" || t2 == "private")
			return true;
		else
			return false;
	}
	else
		return false;
}

//returns true if token is terminator, else returns false
bool SemiExp::isTerminator(std::string token)
{
	if (token == "{" || token == "}" || token == ";")
		return true;
	return false;
}

//returns true if token is comment, else returns false
bool SemiExp::isComment(std::string token)
{
	if ((token[0] == '/') && (token[1] == '/' || token[1] == '*'))
		return true;
	return false;
}

//Handles special case: for(;;) expression
std::string SemiExp::handleFor(std::string token)
{
	int count_semicolon = 0, count_colon = 0;
	while (true)
	{
		    token = _pToker->getTok();
			_tokens.push_back(token);
			if (token == ";" && count_semicolon < 2)
			{
				count_semicolon++;
				continue;
			}
			if (token == ":" && count_colon < 1)
			{
				count_colon++;
				continue;
			}
			if (count_semicolon == 2 || count_colon == 1)
			{
				while (token != "{" && token != ";")
				{
					token = _pToker->getTok();
					_tokens.push_back(token);
				}
				if (token == "{" || token == ";")
					break;
			}
	}
	return token;
}

std::string& SemiExp::operator[](int n)
{
  if (n < 0 || ((size_t) n) >=  _tokens.size())
    throw(std::invalid_argument("index out of range"));
  return _tokens[n];
}

//Calculates the length of vector of tokens
size_t SemiExp::length()
{
  return _tokens.size();
}

//Displays the vector of tokens
std::string SemiExp::show(bool showNewLines)
{
	if (_tokens.empty())
		return "";
	std::string showstring;
	for (auto token : _tokens)
	{
		if (showNewLines)
			showstring.append(token).append(" ");
		else
		{
			if(token != "\n")
				showstring.append(token).append(" ");
		}
	}
	return showstring;
}

//Finds token passed in parameter from vector of tokens
size_t SemiExp::find(const std::string& tok)
{
	size_t i = 0;
	while (i < length()  && _tokens[i] != tok)
		i++;
	if(i < length() &&_tokens[i] == tok)
		return i;
    return length();
}

//Pushes tokens into vector 
void SemiExp::push_back(const std::string& tok)
{
	 _tokens.push_back(tok);
	 return;
}

//Merge is not implemented
bool SemiExp::merge(const std::string& firstTok, const std::string& secondTok)
{
	return true;
}

//Removes token from vector based on value of token
bool SemiExp::remove(const std::string& tok)
{
	for (std::vector<std::string>::iterator iter = _tokens.begin(); iter != _tokens.end(); ++iter)
	{
		if (*iter == tok)
		{
			_tokens.erase(iter);
			return true;
			break;
		}
	}
	return false;
}

//Removes token from vector based on position/index
bool SemiExp::remove(size_t i)
{
	if(i < length())
	{
			_tokens.erase(_tokens.begin() + i);
			return true;	
	}
	else
		return false;
}

//Converts token into Lower Case
void SemiExp::toLower()
{
	for (size_t i = 0; i < length(); i++)
	{
		std::string data = _tokens[i];
		std::transform(data.begin(), data.end(), data.begin(), ::tolower);
		_tokens[i] = data;
	}
}

//Performs trim from front when encounters whitespace or newline
void SemiExp::trimFront()
{
	size_t i = 0; 
	while ((_tokens[0] == " " || _tokens[0] == "\n" ) && i < _tokens.size())
	{
		remove(_tokens[0]);
		i++;
	}
	return;
}

//Clears the tokens
void SemiExp::clear()
{
	_tokens.clear();
}

#ifdef TEST_SEMIEXP
int main()
{
  Toker toker;
 std::string fileSpec = "test.txt";
 // std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
  //std::string fileSpec = "../SemiExp/SemiExp.cpp";
  std::fstream in(fileSpec);
  if (!in.good())
  {
    std::cout << "\n  can't open file " << fileSpec << "\n\n";
    return 1;
  }
  toker.attach(&in);

  SemiExp semi(&toker);
  while(semi.get())
  {
    std::string temp;
    std::cout << "\n  -- semiExpression --";
    temp= semi.show();
	std::cout << "\n" << temp << "\n";
  }
  ////////////
    // May have collected tokens, but reached end of stream
    // before finding SemiExp terminator.
  ///////////
  if (semi.length() > 0)
  {
	std::string temp;
    std::cout << "\n  -- semiExpression --";
	temp = semi.show();
	std::cout << "\n" << temp << "\n";
    std::cout << "\n\n";
  }
  return 0;
}
#endif