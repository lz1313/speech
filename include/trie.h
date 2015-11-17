#ifndef TRIE_H
#define TRIE_H
#include <vector>
#include <string>
using namespace std;
class Node
{
public:
	Node();
	~Node();
	string getContent();
	void setContent(string phone);
	bool isWord();
	vector<string>& word();
	void setWordmarker(bool isWord);
	Node* findChild(string content);
	void printChildren();
	void appendChild(Node* child);
	vector<Node*>& children();
	Node*& parent();
private:
	vector<Node*> _children;
	Node* _parent;
	bool _wordmarker;
	string _content;
	vector<string> _words;
};

class Trie
{
public:
	Trie();
	~Trie();
	void addword(string w, vector<string> s);
	vector<string> searchword(vector<string> s);
	//void deleteword(vector<string> s);
	Node* root();
private:
	Node* _root;
};







#endif // !TRIE_H
