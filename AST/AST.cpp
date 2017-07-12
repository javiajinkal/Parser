////////////////////////////////////////////////////////////////////////////////////////
// AST.cpp - Builds Abstract Syntax Tree      										  //
// ver 1.0																			  //
// Language:    C++, Visual Studio 2015                                               //
// Application: Project2 , CSE687 - Object Oriented Design				              //
// Platform:    Lenovo Yoga 3, Core-i5, Windows 10						              //
// Author:      Jinkal Arvind Javia, SUID: 425325424     				              //
//              jjavia@syr.edu											              //
// Source:		Jim Fawcett, Syracuse University, CST 4-187				              //
//              jfawcett@twcny.rr.com								                  //
////////////////////////////////////////////////////////////////////////////////////////

#include "AST.h"
// Sets the Root node for AST
void AST::setRoot(element* node){
	root = node;
}

// Returns the Root node for AST
element* AST::getRoot(){
	return root;
}

//Performs TreeWalk on the AST
void AST::TreeWalk(element* node){
	static size_t indentLevel = 0;
	std::cout << "\n  " << std::string(2 * indentLevel, ' ') << node->show();
	auto iter = node->_children.begin();
	++indentLevel;
	while (iter != node->_children.end())
	{
		TreeWalk(*iter);
		++iter;
	}
	--indentLevel;
}

// Computes the complexity for each node in AST
size_t AST::complexity(element *node)
{
	size_t complexityCount = 1;
	std::vector<element*> *numberOfChildren = node->retriveChild();
	for (element * child : *numberOfChildren)
	{
		complexityCount += AST::complexity(child);
	}
	return complexityCount;
}

// Deletes the Child node from AST
void AST::deleteChild(element *node, element *deleteNode)
{
	if (node == nullptr)
		return;
	std::vector<element*>::iterator iterate;
	std::vector<element*> *children = node->retriveChild();
	for (iterate = children->begin(); iterate != children->end();)
	{
		if ((*iterate) == deleteNode)
		{
			delete * iterate;
			iterate = children->erase(iterate);
		}
		else
		{
			deleteChild((*iterate), deleteNode);
			++iterate;
		}
	}
}

// Returns the vector containing all the function nodeds from AST
std::vector<element> AST::getFunctionVector()
{
	std::vector<element> fnode;
	getFunction(getRoot(), fnode);
	std::vector<element> v(fnode.begin(), fnode.end());
	return v;
}

// Finds all the function nodes from AST
void AST::getFunction(element *node, std::vector<element> &fnode)
{
	if (node != nullptr && node->type == "function")
		fnode.push_back(*node);
	if (node != nullptr)
	{
		auto iter = node->_children.begin();
		while (iter != node->_children.end())
		{
			getFunction(*iter, fnode);
			++iter;
		}
	}
}

#ifdef TEST_AST
int main()
{
		std::cout << "\n----------------Demo AST-----------------\n";
		element *root = new element;
		root->type = "namespace";
		root->name = "Global";
		root->startCount = 0;
		root->endCount = 30;
		root->lineCount = 31;

		element *elem = new element;
		elem->type = "class";
		elem->name = "class1";
		elem->startCount = 7;
		elem->endCount = 14;
		elem->lineCount = 8;

		element *elem1 = new element;
		elem1->type = "function";
		elem1->name = "function1";
		elem1->startCount = 13;
		elem1->endCount = 25;
		elem1->lineCount = 13;

		element *elem2 = new element;
		elem2->type = "control";
		elem2->name = "if";
		elem2->startCount = 8;
		elem2->endCount = 10;
		elem2->lineCount = 3;

		element *elem3 = new element;
		elem3->type = "control";
		elem3->name = "else";
		elem3->startCount = 11;
		elem3->endCount = 15;
		elem3->lineCount = 5;

		AST *tree = new AST;
		tree->setRoot(root);
		root->insertChild(elem);
		root->insertChild(elem1);
		elem->insertChild(elem2);
		elem2->insertChild(elem3);
		root->complexityCount = tree->complexity(root);
		elem->complexityCount = tree->complexity(elem);
		elem1->complexityCount = tree->complexity(elem1);
		elem2->complexityCount = tree->complexity(elem2);
		elem3->complexityCount = tree->complexity(elem3);
		tree->TreeWalk(root);
		return 0;
}
#endif