#include "../include/neuronpool.h"
#include "../include/utilities.h"
#include "../include/parameter.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <limits>
using namespace std;
neuronpool::neuronpool()
{
	_min = numeric_limits<double>::max();
	_max = numeric_limits<double>::min();
}
neuronpool::~neuronpool()
{
}
void neuronpool::flushpool()
{
	for (map<int, neuron*>::iterator it = _pool.begin(); it != _pool.end(); it++)
	{
		it->second->el() = 0.0;
	}
}
void neuronpool::normalize()
{
	for (map<int, neuron*>::iterator it = _pool.begin(); it != _pool.end(); it++)
	{
		it->second->el() = (double)(it->second->el() - _min) / (_max - _min);
	}
}
neuron*& neuronpool::findNeuronBySymbol(int s)
{
	//if (_pool.find(s) != _pool.end())
	//{
	return _pool[s];
	//}
	//else return nullptr;
}
void neuronpool::constructElUpadateMap(dictionary* _dic, map<int, string> testphonemap)
{
	for (map<int, string>::iterator i = testphonemap.begin(); i != testphonemap.end(); i++)
	{
		string strsym = i->second;
		int sym = _dic->operator[](strsym);
		vector<int> excite_vec;
		vector<int> inhibit_vec;
		for (map<string, int>::iterator it = _dic->_mapToInt.begin(); it != _dic->_mapToInt.end(); ++it)
		{
			vector<string> symbols = fromSerializeStrings<string>(it->first);
			bool found = false;
			if (symbols.back() == strsym)
			{
				found = true;
			}
			if (found)
			{
				excite_vec.push_back(it->second);
			}
			else
			{
				inhibit_vec.push_back(it->second);
			}
		}
		_el_update_map[sym] = std::pair<vector<int>, vector<int>>(excite_vec, inhibit_vec);
	}
}
void neuronpool::findNeuronsBySymbol(int symbol, vector<neuron*>& excite, vector<neuron*>& inhibit)
{
	vector<int> e_vec = _el_update_map[symbol].first;
	vector<int> i_vec = _el_update_map[symbol].second;
	for (size_t i = 0; i < e_vec.size(); i++)
	{
		excite.push_back(findNeuronBySymbol(e_vec[i]));
	}
	for (size_t i = 0; i < i_vec.size(); i++)
	{
		inhibit.push_back(findNeuronBySymbol(i_vec[i]));
	}
}
void neuronpool::addNeuron(int symbol, bool wordmarker)
{
	neuron* n = new neuron(symbol, wordmarker);
	_pool.insert(std::pair<int, neuron*>(symbol, n));
}
void neuronpool::addNeuron(int symbol, string wordsym, bool wordmarker)
{
	neuron* n = new neuron(symbol, wordsym, wordmarker);
	_pool.insert(std::pair<int, neuron*>(symbol, n));
}
void neuronpool::deleteNeuron(int symbol)
{
	map<int, neuron*>::iterator it = _pool.find(symbol);
	if (it != _pool.end())
	{
		_pool.erase(it);
	}
}
void neuronpool::generatePool(string path, dictionary* _dic, Trie* _tr)
{
	ifstream in(path + "/lexicon.txt", std::ios::in);
	char buffer[1024];
	string spechar = "^";
	int spechar_int = _dic->insert(spechar);
	this->addNeuron(spechar_int,spechar,true);
	while (!in.eof())
	{
		in.getline(buffer, 1024);
		string buff(buffer);
		string word = buff.substr(0, buff.find("\t"));
		buff = buff.substr(buff.find("\t") + 1);
		vector<string> phones = fromSerializeStrings<string>(buff);
		int symbol;
		//int wordsym = _dic->insert(word);
		string phone_comb = "";
		for (size_t i = 0; i < phones.size(); i++)
		{
			//int pre = _dic->operator[](phone_comb);
			if (phone_comb == "")
				phone_comb = phones[i];
			else
				phone_comb += " " + phones[i];
			symbol = _dic->insert(phone_comb);
			if (i != phones.size() - 1)
				this->addNeuron(symbol, false);
			else
				this->addNeuron(symbol, word, true);
		}
		//_tr->addword(word, phones);
	}
	in.close();
}

void neuronpool::updateNeuronConnections(dictionary* _dic, int source, int target)
{
	neuron*& n2 = this->findNeuronBySymbol(target);
	n2->connection().push_back(source);
}
void neuronpool::calc_neuron(kb* KB, dictionary* _dic)
{
	vector<pair<int, double>> pre_el;
	vector<pair<int, double>> current_el;
	_min = numeric_limits<double>::max();
	_max = numeric_limits<double>::min();
	int i = 0;
	while (i < 1)
	{
		double sum = 0.0;
		for (map<int, neuron*>::iterator it = _pool.begin(); it != _pool.end(); it++)
		{
			if (it->second->el() < 0) it->second->el() = 0;
			it->second->prev_el() = it->second->el();
		}
		for (map<int, neuron*>::iterator it = _pool.begin(); it != _pool.end(); it++)
		{
			it->second->calc_el(KB, _dic, &this->_pool);
			sum += it->second->el();
			double e = it->second->el();
			if (e > _max) _max = e;
			if (e < _min) _min = e;
			//it->second->el()= sigmoid(it->second->el());
//			if (it->second->el() != 0)
//				cout << "[" << it->first << " " << _dic->operator[](it->first) << "]" << " el: " << it->second->el() << endl;
			//cout << it->first << " el: " << it->second->el() << endl;
			//current_el.push_back(std::pair<int, double>(it->first, it->second->el()));
		}
		//normalize
		for (map<int, neuron*>::iterator it = _pool.begin(); it != _pool.end(); it++)
		{
			it->second->el() = (double)(it->second->el() - _min) / (_max - _min) + _min;
			current_el.push_back(std::pair<int, double>(it->first, it->second->el()));
		}
		prune(current_el);
		for (int j = 0; j < current_el.size(); ++j)
		{
			neuron* n = this->findNeuronBySymbol(current_el[j].first);
			if (n->el() > 0.0 &&n->isWord())
				for (int k = 0; k < n->wordsym().size(); ++k)
				{
					cout << "[" << n->wordsym()[k] << ":" << _dic->operator[](current_el[j].first) << "]" << " el: " << current_el[j].second << endl;
				}
				
		}
		/*current_el.clear();
		cout<<"After prune:\n";
		for (map<int, neuron*>::iterator it = _pool.begin(); it != _pool.end(); it++)
		{
			it->second->el() = (double)(it->second->el() - _min) / (_max - _min);
			current_el.push_back(std::pair<int, double>(it->first, it->second->el()));
		}
		prune(current_el);
		for (int j = 0; j < current_el.size(); ++j)
		{
			if (this->findNeuronBySymbol(current_el[j].first)->el() > 0.2)
				cout << "[" << current_el[j].first << " " << _dic->operator[](current_el[j].first) << "]" << " el: " << current_el[j].second << endl;
		}*/
		i++;
		/*cout << "iteration count:" << i << "\n";
		i++;
		if (isequal(pre_el, current_el))
		{
			cout << "iteration ends\n";
			break;
		}
		else
		{
			pre_el = current_el;
			current_el.clear();
		}*/
	}
}
void neuronpool::leak()
{
	for (map<int, neuron*>::iterator it = _pool.begin(); it != _pool.end(); it++)
	{
		it->second->leak();
	}
}
struct comp
{
	bool operator()(pair<int, double> a, pair<int, double> b)
	{
		if (a.second > b.second)
			return true;
		else if (a.second == b.second)
		{
			return a.first < b.first;
		}
		else return false;
	}
} comp;
void neuronpool::prune(vector<pair<int, double>>& a)
{
	sort(a.begin(), a.end(), comp);
	/*for (size_t i = 2 * CANDIDATE_SIZE; i < a.size(); i++)
	{
		_pool[a[i].first]->el() = 0.0;
	}*/
	//a.resize(CANDIDATE_SIZE);

}
bool neuronpool::isequal(vector<pair<int, double>> a, vector<pair<int, double>> b)
{
	if (a.size() != b.size()) return false;
	for (size_t i = 0; i < CANDIDATE_SIZE; i++)
	{
		if (a[i].first != b[i].first) return false;
	}
	return true;
}
//#define TEST_NEURONPOOL
#ifdef TEST_NEURONPOOL

void main()
{
	neuronpool* np = new neuronpool();
	np->generatePool("./");
	np->findNeuronBySymbol(15);
}
#endif