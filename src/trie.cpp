#include "../include/trie.h"
#include <iostream>
using namespace std;
Node::Node() :_content(""), _wordmarker(false),_parent(NULL){}
Node::~Node(){ _children.clear(); }

bool Node::isWord(){ return _wordmarker; }
void Node::setWordmarker(bool wordmarker){ _wordmarker = wordmarker; }
vector<string> & Node::word(){ return _words; }

string Node::getContent(){ return _content; }
void Node::setContent(string content){ _content = content; }

vector<Node*>& Node::children(){ return _children; }
Node*& Node::parent(){ return _parent; }
void Node::appendChild(Node* child){ _children.push_back(child); }
Node* Node::findChild(string content)
{
	for (size_t i = 0; i < _children.size(); i++)
	{
		Node* tmp = _children.at(i);
		if (tmp->getContent() == content)
		{
			return tmp;
		}
	}
	return NULL;
}
void Node::printChildren()
{
	if (_children.size() == 0)
	{
		cout << "No Children\n";
	}
	for (size_t i = 0; i < _children.size(); i++)
	{
		Node* tmp = _children.at(i);
		cout << tmp->getContent() << ", ";
	}
}

Trie::Trie() :_root(new Node())
{
}

Trie::~Trie()
{
	Node* current = _root;
	while (_root->children().size() != 0)
	{
		if (current->children().size() != 0)
		{
			Node* temp = current->children().back();
			current = temp;
		}
		else
		{
			Node* temp = current->parent();
			temp->children().pop_back();
			delete current;
			current = temp;
		}
	}
}

void Trie::addword(string w, vector<string> s)
{
	Node* current = _root;
	if (s.size() == 0)
	{
		current->setWordmarker(true);
		return;
	}

	for (size_t i = 0; i < s.size(); i++)
	{
		Node* child = current->findChild(s[i]);
		if (child != NULL)
		{
			current = child;
		}
		else
		{
			Node* tmp = new Node();
			tmp->setContent(s[i]);
			current->appendChild(tmp);
			tmp->parent() = current;
			current = tmp;
		}
		if (i == s.size() - 1)
		{
			current->setWordmarker(true);
			current->word().push_back(w);
		}
	}
}

vector<string> Trie::searchword(vector<string> s)
{
	vector<string> ret;
	Node* current = _root;
	if (current != NULL)
	{
		for (size_t i = 0; i < s.size(); i++)
		{
			Node* tmp = current->findChild(s[i]);
			if (tmp == NULL) return ret;
			current = tmp;
		}
		if (current->isWord()) return current->word();
		else return ret;
	}
	return ret;
}
//#define TEST_TRIE
#ifdef TEST_TRIE
#include "../include/utilities.h"
int main()
{
	Trie* t = new Trie();
	t->addword("abc",fromSerializeStrings("aa bb cc"));
	t->addword("abd",fromSerializeStrings("aa bb dd"));
	t->addword("adg",fromSerializeStrings("aa dd gg"));
	t->addword("bfc",fromSerializeStrings("bb ff cc"));
}
#endif
