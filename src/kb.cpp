#include "../include/kb.h"
#include "../include/parameter.h"
#include <sstream>
#include <fstream>
#include <cmath>
#include <iostream>
using namespace std;
kb::kb()
{
}

kb::~kb()
{
}

kb::kbmap& kb::KB_map()
{
	return _map;
}
kb::kbmap& kb::KB_map2()
{
	return _map2;
}
kb::col& kb::operator[](int source)
{
	return _map[source];
}
void kb::increment(int s, int t)
{
	_map[s][t]++;
	_map2[t][s]++;
}
void kb::readKB(string path)
{
	cout << "==============reading KB================\n";
	ifstream in(path + "/KB.kb", std::ios::in);
	string buffer;
	while (!in.eof())
	{
		getline(in,buffer);
		int source, target;
		int count;
		double value;
		string temp;
		istringstream is(buffer);
		is >> temp >> source >> count;
		for (int i = 0; i < count; i++)
		{
			getline(in, buffer);
			istringstream iss(buffer);
			iss >> target >> value;
			_map[source][target] = value;
		}
	}
	in.close();
	ifstream in2(path + "/KB2.kb", std::ios::in);
	string buffer2;
	while (!in2.eof())
	{
		getline(in2,buffer2);
		int source, target;
		int count;
		double value;
		string temp;
		istringstream is(buffer);
		is >> temp >> source >> count;
		for (int i = 0; i < count; i++)
		{
			getline(in, buffer);
			istringstream iss(buffer);
			iss >> target >> value;
			_map2[source][target] = value;
		}
	}
	in.close();
}
bool kb::exist(int source, int target)
{
	bool ret = false;
	bool ret2=false;
	if (_map.find(source)!=_map.end())
	{
		if (_map[source].find(target)!=_map[source].end())
		{
			ret = true;
		}
	}
	if (_map2.find(target)!=_map.end())
	{
		if (_map2[target].find(source)!=_map2[target].end())
		{
			ret2 = true;
		}
	}
	return ret&&ret2;
}
void kb::dumpKB(string path)
{
	cout << "=============dumping KB===================\n";
	ofstream out(path + "/KB.kb", std::ios::out);
	for (kb::kbmap::iterator it = _map.begin(); it != _map.end(); ++it)
	{
		out << "source" <<" "<< it->first << " " << _map[it->first].size() << endl;
		for (kb::col::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
		{
			out << it2->first << " " << it2->second << "\n";
		}
	}
	out.close();
	ofstream out2(path + "/KB2.kb", std::ios::out);
	for (kb::kbmap::iterator it = _map2.begin(); it != _map2.end(); ++it)
	{
		out2 << "source" <<" "<< it->first << " " << _map2[it->first].size() << endl;
		for (kb::col::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
		{
			out2 << it2->first << " " << it2->second << "\n";
		}
	}
	out2.close();
}
void kb::recalculate(int dic_size)
{
	//p(s|t)
	double *col_val = new double[dic_size]{};

	for (kb::kbmap::iterator it = _map.begin(); it != _map.end(); ++it)
	{
		for (kb::col::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
		{
			col_val[it2->first] += it2->second;
		}
	}
	for (kb::kbmap::iterator it = _map.begin(); it != _map.end(); ++it)
	{
		for (kb::col::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
		{
			//cout << "row: " << it->first << " col:" << it2->first << "\n value:" << it2->second << " priori: " << col_val[it2->first] << "\n";
			_map[it->first][it2->first] = log((it2->second / col_val[it2->first]) / P0);
		}
	}
	//p(t|s)
	double *col_val2 = new double[dic_size]{};
	for (kb::kbmap::iterator it = _map2.begin(); it != _map2.end(); ++it)
	{
		for (kb::col::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
		{
			col_val2[it2->first] += it2->second;
		}
	}
	for (kb::kbmap::iterator it = _map2.begin(); it != _map2.end(); ++it)
	{
		for (kb::col::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
		{
			//cout << "row: " << it->first << " col:" << it2->first << "\n value:" << it2->second << " priori: " << col_val[it2->first] << "\n";
			_map2[it->first][it2->first] = log((it2->second / col_val[it2->first]) / P0);
		}
	}
	cout << "=============done recalculation===================\n";
}
//#define TEST_KB
#ifdef TEST_KB
void main()
{
	kb *KB = new kb();
	KB->increment(0, 1);
	KB->increment(0, 2);
	KB->increment(0, 1);
	KB->increment(1, 1);
	KB->increment(2, 2);
	KB->recalculate(3);
	KB->dumpKB("./");
}

#endif