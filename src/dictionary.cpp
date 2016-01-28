#include "../include/dictionary.h"
#include "../include/parameter.h"
#include "../include/utilities.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;
dictionary::dictionary()
{
	_size = 1;
}

dictionary::~dictionary()
{
}
void dictionary::readLexicon(string path)
{
	ifstream in(path + "/lexicon.txt", std::ios::in);
	char buffer[1024];
	while (!in.eof())
	{
		in.getline(buffer, 1024);
		string buff(buffer);
		string word = buff.substr(0, buff.find("\t"));
		buff = buff.substr(buff.find("\t") + 1);
		vector<string> phones = fromSerializeStrings<string>(buff);
		for (size_t i = 1; i < phones.size(); i++)
		{
			phones[i] = phones[i - 1] + " " + phones[i];
		}
		//_mapToPhones.emplace(word, phones);
		_mapToPhones.insert(std::pair<string,std::vector<string>>(word, phones));
	}
	in.close();
}
vector<vector<string>> dictionary::getPhoneSequence(string word)
{
	vector<vector<string>> ret;
	dictionary::phonemap::iterator it;
	for (it = _mapToPhones.equal_range(word).first; it!=_mapToPhones.equal_range(word).second; ++it)
	{
		ret.push_back(it->second);
	}
	return ret;
}
vector<vector<string>> dictionary::getPhoneMap()
{
	vector<vector<string>> ret;
	dictionary::phonemap::iterator it;
	for (it = _mapToPhones.begin(); it != _mapToPhones.end();++it)
	{
		ret.push_back(it->second);
	}
	return ret;

}
void dictionary::readDic(string path)
{
	ifstream in(path + "/dictionary.kb", std::ios::in);
	string str;
	int dic_size;
	getline(in, str);
	string temp;
	istringstream is(str);
	is >> temp >> temp >> dic_size;
	for (int i = 0; i < dic_size; i++)
	{
		getline(in, str);
		istringstream iss(str);
		int j;
		std::string s1, s2;
		iss >> j >> s1 >> s2;
		if (s2.length() != 0)
		{
			s1 += " " + s2;
		}
		_size++;
		_mapToInt[s1] = j;
		_mapToString[j] = s1;
	}
	in.close();
}
void dictionary::dumpDic(string path)
{
	ofstream out(path + "/dictionary.KB", std::ios::out);
	cout << "dump dictionary\n";
	out << "dictionary size " << _mapToString.size() << "\n";
	for (dictionary::stringMap::iterator i = _mapToString.begin(); i != _mapToString.end(); i++)
	{
		out << i->first << " " << i->second << "\n";
	}
	out.close();
}
int dictionary::size()
{
	return _size;
}
int dictionary::operator[](string stringkey)
{
	if (_mapToInt.find(stringkey) != _mapToInt.end())
		return _mapToInt[stringkey];
	else
		return OOV;
}
string dictionary::operator[](int intkey)
{
	if (_mapToString.find(intkey) != _mapToString.end())
		return _mapToString[intkey];
	else
		return "SIL";
}
int dictionary::insert(string s)
{
	dictionary::intMap::iterator it = _mapToInt.find(s);
	if (it!=_mapToInt.end())
	{
		return it->second;
	}
	else
	{
		_mapToInt[s] = _size;
		_mapToString[_size] = s;
		int loc = _size;
		_size++;
		return loc;
	}
}