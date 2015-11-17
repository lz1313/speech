#ifndef TESTING_H
#define TESTING_H
#include "parameter.h"
#include "dictionary.h"
#include "kb.h"
#include "neuronpool.h"
#include "topology.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
using namespace std;
class testing
{
public:
	testing();
	~testing();
	void processing(string path=".");
	void readGMM(string buff);
	void constructphonemap(string path = ".");
private:
	void initialization(string path = ".");
	void updateNeuronConnection();

	ifstream in;
	ofstream out;
	dictionary* _dic;
	kb* _kb;
	neuronpool* _np;
	topology* _topo;
	Trie * _tr;
	map<int, string> _testphonemap;
};



#endif // !TESTING_H
