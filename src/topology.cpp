#include "../include/topology.h"
#include "../include/utilities.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;
topology::topology()
{
}
topology::~topology()
{
}
void topology::dumpTopo(string path)
{
	cout << "=============dumping Topo===================\n";
	ofstream out(path + "/topo.kb", std::ios::out);
	for (set<pair<int,int>>::iterator it = _links.begin(); it != _links.end(); ++it)
	{
		out << it->first << " " << it->second << endl;
	}
	out.close();
}
void topology::readTopo(string path)
{
	cout << "==============reading Topo================\n";
	ifstream in(path + "/topo.kb", std::ios::in);
	string buffer;
	while (!in.eof())
	{
		getline(in,buffer);
		int source, target;
		istringstream iss(buffer);
		iss >> source >> target;
		_links.insert(make_pair(source, target));
	}
	in.close();
}
void topology::refreshLinks()
{
	_links.clear();
}
void topology::addInterWordLink(string w1,string w2,dictionary* _dic)
{
	vector<vector<string>> ws1 = _dic->getPhoneSequence(w1);
	vector<vector<string>> ws2 = _dic->getPhoneSequence(w2);
	for (size_t i = 0; i < ws1.size(); i++)
	{
		for (size_t j = 0; j < ws2.size(); j++)
		{
			int symbol1 = _dic->operator[](ws1[i].back());
			int symbol2 = _dic->operator[](ws2[j].front());
			_links.insert(make_pair(symbol1, symbol2));
		}
	}
}
void topology::addIntraWordLink(dictionary* _dic)
{
	vector<vector<string>> wrdseq = _dic->getPhoneMap();
	for (size_t i = 0; i < wrdseq.size(); i++)
	{
		for (size_t j = 1; j < wrdseq[i].size(); j++)
		{
			int symbol1 = _dic->operator[](wrdseq[i][j-1]);
			int symbol2 = _dic->operator[](wrdseq[i][j]);
			_links.insert(make_pair(symbol1, symbol2));
		}
	}
}
set<pair<int,int>>& topology::Links()
{
	return _links;
}