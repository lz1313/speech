#include "../include/neuron.h"
#include "../include/parameter.h"
#include "../include/utilities.h"
#include <cmath>
using namespace std;
neuron::neuron()
{
	_fire_el_count = 0;
	_total_count = 0;
	_el_now = 0.0;
	_symbol = -1;
	_el_previous=0.0;
	//_prev_symbol = -1;
	_word_marker = false;
}
neuron::neuron(int symbol)
{
	_fire_el_count = 0;
	_total_count = 0;
	_el_now = 0.0;
	_symbol = symbol;
	_el_previous=0.0;
	//_prev_symbol = -1;
	_word_marker = false;
}
neuron::neuron(int symbol, bool isword)
{
	_fire_el_count = 0;
	_total_count = 0;
	_el_now = 0.0;
	_symbol = symbol;
	_el_previous=0.0;
	//_prev_symbol = pre;
	_word_marker = isword;
}
neuron::neuron(int symbol,string wordsym, bool isword)
{
	_fire_el_count = 0;
	_total_count = 0;
	_el_now = 0.0;
	_symbol = symbol;
	_el_previous=0.0;
	//_prev_symbol = pre;
	_word_marker = isword;
	_wordsym.push_back(wordsym);
}
neuron::~neuron()
{
	//_excite_connect.clear();
	//_inhibit_connect.clear();
}
double& neuron::el() { return _el_now; }
double& neuron::prev_el(){ 
	return _el_previous;
}
int& neuron::symbol() { return _symbol; }
void neuron::incrementCount()
{
	if (_el_now > FIRE_THRESHOLD)
		_fire_el_count++;
	_total_count++;
}
void neuron::calc_el(kb* KB, dictionary* dic, map<int, neuron*>* np)
{
	/*for (map<int, neuron*>::iterator it = np->begin(); it != np->end(); it++)
	{
		if (it->second->el() > 0)
			_el_now -= BETA * log(it->second->el())/10.0;
	}*/
	for (size_t i = 0; i < _connect.size(); i++)
	{
		neuron* n = np->operator[](_connect[i]);
		if (!KB->exist(_connect[i], this->_symbol) || n->prev_el() <= 0)
			continue;
		if (KB->KB_map()[n->symbol()][this->_symbol]<0)
			continue;
		if (KB->KB_map2()[this->_symbol][n->symbol()]<0)
			continue;
		_el_now += (n->prev_el() + log(KB->KB_map()[this->_symbol][n->symbol()]) - log(KB->KB_map()[n->symbol()][this->_symbol]));// + BETA * log(n->el());
	}
	//if (_el_now < 0) _el_now = 0.0;
}
/*
bool neuron::isExcite(int symbol)
{
	if (_connect.find(symbol) != _connect.end())
		return true;
	return false;
}*/
/*
int*& neuron::incomingExcitation()
{
	return _excite_connect;
}
int*& neuron::incomingInhibition()
{
	return _inhibit_connect;
}*/

vector<int>& neuron::connection()
{
	return _connect;
}
void neuron::leak()
{
	_el_now *= (1 - LEAK_RATE);
}
bool neuron::output()
{
	double ratio = (double) _fire_el_count * 1.0 / _total_count;
	if (ratio > FIRE_RATE&&_word_marker)
	{
		return true;
	}
	return false;
}

/*int& neuron::predecessor()
{
	return _prev_symbol;
}*/