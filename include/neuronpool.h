#ifndef NEURONPOOL_H
#define NEURONPOOL_H
#include "neuron.h"
#include "dictionary.h"
#include "trie.h"
#include "kb.h"
#include <map>
#include <string>
#include <vector>
using namespace std;
class neuronpool
{
public:
	neuronpool();
	~neuronpool();
	void flushpool();
	neuron*& findNeuronBySymbol(int symbol);
	void findNeuronsBySymbol(int symbol,vector<neuron*>& excite, vector<neuron*>&);
	void addNeuron(int symbol,bool wordmarker);
	void addNeuron(int symbol,string wordsym,bool wordmarker);
	void deleteNeuron(int symbol);
	void updateNeuronConnections(dictionary* _dic,int source, int target);
	void generatePool(string path, dictionary* _dic, Trie* _tr);
	void constructElUpadateMap(dictionary* _dic, map<int, string> testphonemap);
	void calc_neuron(kb* KB, dictionary* _dic);
	void normalize();
	void leak();
private:
	double _min;
	double _max;
	map<int,neuron*> _pool;
	map<int, pair<vector<int>,vector<int>>> _el_update_map;
	bool isequal(vector<pair<int,double>> a, vector<pair<int,double>> b);
	void prune(vector<pair<int,double>>& a);

};

#endif // !NEURONPOOL_H
