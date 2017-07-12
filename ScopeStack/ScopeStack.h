#ifndef SCOPESTACK_H
#define SCOPESTACK_H
////////////////////////////////////////////////////////////////////////////
// ScopeStack.h -implements template stack holding specified element type //
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
  Package Operations:
  ===================
  This package contains one class: ScopeStack<element>.  The element type
  is an application specific type designed to hold whatever information you
  need to stack.

  Public Interface:
  =================
  ScopeStack<element> stack;
  // define some element type
  element elem;
  stack.push(elem);
  element popped = stack.pop();

  Required Files:
  ===============
  ScopeStack.h, ScopeStack.cpp

  Build Command:
  ==============
  devenv Project2.sln /rebuild debug

  Maintenance History:
  ====================
  * ver 1.0 : 12 March 16
  * - first release
*/
#include <list>

// element is a application specific type.  It must provide a function:
// std::string show() that returns a string display of its parts. see
// test stub for an example.

// This class provides stack functionalities
template<typename element>
class ScopeStack {
public:
  typename typedef std::list<element>::iterator iterator;
  void push(const element& item);
  element pop();
  element top();
  size_t size();
  iterator begin();
  iterator end();
private:
  std::list<element> stack;
};

//Returns stack size
template<typename element>
typename inline size_t ScopeStack<element>::size() { return stack.size(); }

//Returns begining of stack
template<typename element>
typename inline ScopeStack<element>::iterator ScopeStack<element>::begin() { return stack.begin(); }

//Returns end of stack
template<typename element>
typename inline ScopeStack<element>::iterator ScopeStack<element>::end() { return stack.end(); }

//Pushes element on stack
template<typename element>
void ScopeStack<element>::push(const element& item)
{
  stack.push_back(item);
}

//Pops element from stack
template<typename element>
element ScopeStack<element>::pop() 
{ 
  element item = stack.back(); 
  stack.pop_back(); 
  return item; 
}

//Returns top of the stack
template<typename element>
element ScopeStack<element>::top()
{
  return stack.back();
}

// Displays the contents of stack without using pointer 
template<typename element>
void showStack(ScopeStack<element>& stack, bool indent = true)
{
  if (stack.size() == 0)
  {
    std::cout << "\n  ScopeStack is empty";
    return;
  }
  ScopeStack<element>::iterator iter = stack.begin();
  while (iter != stack.end())
  {
    std::string strIndent = std::string(2 * stack.size(), ' ');
    if (!indent)
      strIndent = "";
    element temp = *iter;
    std::cout << "\n  " << strIndent << temp->show();
    ++iter;
  }
}

// Displays the contents of stack using pointer
template<typename element>
void showStack(ScopeStack<element*>& stack, bool indent = true)
{
  if (stack.size() == 0)
  {
    std::cout << "\n  ScopeStack is empty";
    return;
  }
  ScopeStack<element*>::iterator iter = stack.begin();
  while (iter != stack.end())
  {
    std::string strIndent = std::string(2 * stack.size(), ' ');
    if (!indent)
      strIndent = "";
    std::cout << "\n  " << strIndent << ((*iter)->show());
    ++iter;
  }
}
#endif
