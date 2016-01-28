#ifndef KB_H
#define KB_H
#include <map>
#include <string>
using namespace std;
class kb
{
	typedef map<int, double> col;
	typedef map<int, col> kbmap;
public:
	kb();
	~kb();
	kbmap& KB_map();
	kbmap& KB_map2();
	col& operator[](int source);
	void readKB(string path = ".");
	void dumpKB(string path = ".");
	void increment(int s, int t);
	void recalculate(int dic_size);
	bool exist(int source, int target);
private:
	kbmap _map;
	kbmap _map2;
};

#endif // !KB_H
