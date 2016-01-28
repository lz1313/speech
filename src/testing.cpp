#include "../include/testing.h"
#include "../include/utilities.h"
#include <fstream>
#include <cmath>
using namespace std;
testing::testing()
{
	_dic = new dictionary();
	_np = new neuronpool();
	_topo = new topology();
	_kb = new kb();
	_tr = new Trie();
}

testing::~testing()
{
	delete _dic;
	delete _np;
	delete _topo;
	delete _kb;
	delete _tr;
}
void testing::initialization(string path)
{
	_dic->readLexicon(path);
	cout << "done read lexicon\n";
	this->constructphonemap(path);
	cout << "done construct phone map \n";
	_np->generatePool(path, _dic, _tr);
	cout << "done generate pool\n";
	_topo->readTopo(path);
	cout << "done read topo\n";
	_kb->readKB(path);
	cout << "done read kb\n";
	this->updateNeuronConnection();
	cout << "done update neuron connection\n";
	_np->constructElUpadateMap(_dic, _testphonemap);
	cout << "done construct el update map\n";

}
void testing::processing(string path)
{
	initialization(path);
	in.open(path + "/likes.1.txt", std::ios::in);
	int i = 0;
	char buffer[1024];
	int j = 0;
	_np->findNeuronBySymbol(_dic->operator[]("^"))->el()=1000;
	while (!in.eof())
	{
		in.getline(buffer, 1024);
		string buff(buffer);
		if (buff == "")
		{
			break;
		}
		if (buff.find("[") != string::npos)
		{
			_np->flushpool();
			j++; i = 0; continue;
		}
		if (j>1)
		{
			break;
		}
		cout << "Sentence: " << j << ",Frame: " << i++ << endl;
		readGMM(buff);
		_np->calc_neuron(_kb, _dic);
		//_np->normalize();
		_np->leak();
	}
	in.close();
}
void testing::updateNeuronConnection()
{
	for (set<pair<int, int>>::iterator it = _topo->Links().begin(); it != _topo->Links().end(); ++it)
	{
		_np->updateNeuronConnections(_dic, (*it).first, (*it).second);
	}
}

void testing::readGMM(string buff)
{
	buff = buff.substr(2);
	buff = buff.substr(0, buff.find_last_of(" "));
	vector<double> gmm = fromSerializeStrings<double>(buff);
	map<double, int> gmmmap;
	for (size_t i = 0; i < gmm.size(); i++)
	{
		gmmmap[gmm[i]] = i;
	}
	vector<double> can;
	int i = 0;
	for (map<double, int>::reverse_iterator it = gmmmap.rbegin(); it != gmmmap.rend(), i < CANDIDATE_SIZE; it++, i++)
	{
		can.push_back(it->first);
	}
	//double u = mean(can);
	//double d = deviation(can, u);
	i = 0;
	cout << "Input is [";
	vector<double> weight_vec;
	weight_vec = can;
	double sum=0.0;
	for (map<double, int>::reverse_iterator it = gmmmap.rbegin(); it != gmmmap.rend(); it++, i++)
	{
		double weight = it->first;//pow(E, Zscore(it->first, u, d));
		weight_vec.push_back(weight);
		sum+=weight;
	}
	i = 0;
	for (map<double, int>::reverse_iterator it = gmmmap.rbegin(); it != gmmmap.rend(), i < CANDIDATE_SIZE; it++, i++)
	{
		double weight = weight_vec[i];///*(double) weight_vec[i]/sum;*/ (double)(weight_vec[i]-weight_vec.back())/(weight_vec[0]-weight_vec.back());
		string symbol = _testphonemap[it->second];
		int sym = _dic->operator[](symbol);
		cout << symbol << ":" << weight << " ";
		//_np->findNeuronBySymbol(sym)->el()+=weight;
		vector<neuron*> excite;
		vector<neuron*> inhibit;
		_np->findNeuronsBySymbol(sym, excite, inhibit);
		//cout<<"excite following:\n";
		for (int j = 0; j < excite.size(); ++j)
		{
			excite[j]->el() += (double) weight ;// ( excite[j].first * 1.0);
			//cout << "[" << excite[j].second->symbol() << " " << _dic->operator[](excite[j].second->symbol()) << "]" << " el: " << excite[j].second->el() << endl;
			//cout<<10.0*pow(E,Zscore(it->first, u, d))<<endl;
			//cout << _np->findNeuronBySymbol(n[j]->symbol())->el() << endl;
		}
		for (int j = 0; j < inhibit.size(); ++j)
		{
			//inhibit[j]->el() -= (double) 0.5*weight;// ( inhibit[j].first *1.0);
			//cout<<10.0*pow(E,Zscore(it->first, u, d))<<endl;
			//cout << _np->findNeuronBySymbol(n[j]->symbol())->el() << endl;
		}
	}
	cout << "]\n";
}

void testing::constructphonemap(string path )
{
	ifstream phonein(path + "/phones.txt", ios::in);
	char buffer[1024];
	phonein.getline(buffer, 1024);
	int i = 0;
	_testphonemap[i++] = "SIL";
	_testphonemap[i++] = "SIL";
	while (!phonein.eof())
	{
		phonein.getline(buffer, 1024);
		string buff(buffer);
		if (buff[0] == '#') break;
		buff = buff.substr(0, buff.find(" "));
		_testphonemap[i++] = buff;
	}
	phonein.close();
}
#define TEST_TESTING
#ifdef TEST_TESTING

int main(int argc, char const *argv[])
{
	testing* t = new testing();
	t->processing(argv[1]);
	return 0;
}
#endif