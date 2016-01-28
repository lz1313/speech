#ifndef TRAINING_H
#define TRAINING_H
#include "parameter.h"
#include "dictionary.h"
#include "kb.h"
#include "neuronpool.h"
#include "topology.h"
#include "trie.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
class training
{
public:
	training();
	~training();
	void processing(string path="." );
private:
	void simpleKB();
	void addLinkandIncrement(vector<string>);
	void initialization(string path="." );
	ifstream in;
	dictionary* _dic;
	kb* _kb;
	neuronpool* _np;
	topology* _topo;
	Trie * _tr;
};




#endif // !TRAINING_H
