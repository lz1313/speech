#ifndef NEURON_H
#define NEURON_H
#include "kb.h"
#include "dictionary.h"
#include <vector>
#include <string>
using namespace std;

class neuron
{
	//typedef map<int, neuron*> neuron_map;
public:
	neuron();
	neuron(int);
	//neuron(int, int);//reserved
	neuron(int, bool);
	neuron(int, string, bool);
	~neuron();
	double& el();
	double& prev_el();
	int& symbol();
	void calc_el(kb*, dictionary*, map<int, neuron*>*);
	void leak();
	bool output();
	void incrementCount();
	//int& predecessor();
	bool isWord(){return _word_marker;}
  vector<string>& wordsym(){return _wordsym;}
	vector<int>& connection();
	bool isExcite(int);
private:
	//int _prev_symbol;
    vector<string> _wordsym;
	int _symbol;
	double _el_now;
	double _el_previous;
	int _fire_el_count;
	int _total_count;
	bool _word_marker;
	vector<int> _connect;
};


#endif