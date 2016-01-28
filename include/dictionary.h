#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <map>
#include <string>
#include <vector>
using namespace std;

class dictionary
{
	typedef map<string, int> intMap;
	typedef map<int, string> stringMap;
	typedef multimap<string, vector<string>> phonemap;
public:
	dictionary();
	~dictionary();
	int operator[](string stringkey);
	string operator[](int intkey);
	vector<vector<string>> getPhoneSequence(string word);
	vector<vector<string>> getPhoneMap();
	int insert(string s);
	void readLexicon(string path = ".");
	void readDic(string path = ".");
	void dumpDic(string path = ".");
	int size();
	intMap _mapToInt;
	stringMap _mapToString;
private:
	multimap<string, vector<string>> _mapToPhones;
	int _size;

};


#endif // !DICTIONARY_H

