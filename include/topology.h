#ifndef TOPOLOGY_H
#define TOPOLOGY_H
#include "dictionary.h"
#include "trie.h"
#include <set>
#include <string>
using namespace std;
class topology
{
public:
	topology();
	~topology();
	void refreshLinks();
	void addInterWordLink(string w1, string w2, dictionary* _dic);
	void addIntraWordLink(dictionary* _dic);
	void dumpTopo(string path = ".");
	void readTopo(string path = ".");
	set<pair<int, int>>& Links();
private:
	set<pair<int, int>> _links;
};



#endif // !TOPOLOGY_H
