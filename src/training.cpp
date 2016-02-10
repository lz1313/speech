#include "../include/training.h"
#include "../include/utilities.h"

training::training()
{
	_dic = new dictionary();
	_np = new neuronpool();
	_topo = new topology();
	_kb = new kb();
	//_tr = new Trie();
}

training::~training()
{
	delete _dic;
	delete _np;
	delete _topo;
	delete _kb;
	//delete _tr;
}
void training::initialization(string path)
{
	_dic->readLexicon(path);
	cout << "done readLexicon\n";
	_np->generatePool(path, _dic/*, _tr*/);
	cout << "done generatePool\n";
	_topo->addIntraWordLink(_dic);
	cout << "done addIntraWordLink\n";
	//this->simpleKB();
	_dic->dumpDic(path);
}
void training::simpleKB()
{
	vector<vector<string>> wrdseq = _dic->getPhoneMap();
	for (size_t i = 0; i < wrdseq.size(); i++)
	{
		for (size_t j = 1; j < wrdseq[i].size(); j++)
		{
			int symbol1 = _dic->operator[](wrdseq[i][j - 1]);
			int symbol2 = _dic->operator[](wrdseq[i][j]);
			_kb->increment(symbol1, symbol2);
		}
	}
}
void training::processing(string path)
{
	initialization(path);
	cout << "done initialization\n";
	in.open(path + "/train.txt", std::ios::in);
	int i = 0;
	char buffer[1024];
	while (!in.eof())
	{
		cout << "processing sentence " << i + 1 << "\n";
		in.getline(buffer, 1024);
		string buff(buffer);
		string dictator = buff.substr(0, buff.find(" "));
		buff = buff.substr(buff.find(" ") + 1);
		vector<string> sentence = fromSerializeStrings<string>(buff);
		this->addLinkandIncrement(sentence);
		i++;
	}
	_kb->recalculate(_dic->size());
	_kb->dumpKB(path);
	_topo->dumpTopo(path);
	in.close();
}

void training::addLinkandIncrement(vector<string> sentence)
{
	/*vector<string> newsentence;
	newsentence.push_back("!SIL");
	for (int i = 0; i < sentence.size(); ++i)
	{
		newsentence.push_back(sentence[i]);
	}
	newsentence.push_back("!SIL");
	for (int i = 1; i < newsentence.size(); ++i)
	{
		_topo->addInterWordLink(newsentence[i - 1], newsentence[i], _dic);
		vector<vector<string>> ws1 = _dic->getPhoneSequence(newsentence[i-1]);
		vector<vector<string>> ws2 = _dic->getPhoneSequence(newsentence[i]);

		for (size_t j = 0; j < ws1.size(); j++)
		{
			for (size_t k = 0;k < ws2.size(); k++)
			{
				vector<string> phone_seq = ws1[j];
				phone_seq.insert(phone_seq.end(), ws2[k].begin(), ws2[k].end());
				for (size_t l = 1; l < phone_seq.size(); l++)
				{
					int symbol1 = _dic->operator[](phone_seq[l - 1]);
					int symbol2 = _dic->operator[](phone_seq[l]);
					_kb->increment(symbol1, symbol2);
				}
			}
	}*/
	//sentence.insert(sentence.begin(), "!SIL");
	//sentence.push_back("!SIL");
	_topo->addInterWordLink("^", sentence[0], _dic);
	vector<vector<string>> ws0 = _dic->getPhoneSequence(sentence[0]);
	for (size_t k = 0; k < ws0.size(); k++)
	{
		vector<string> phone_seq = ws0[k];
		phone_seq.insert(phone_seq.begin(), "^");
		for (size_t l = 1; l < phone_seq.size(); l++)
		{
			int symbol1 = _dic->operator[](phone_seq[l - 1]);
			int symbol2 = _dic->operator[](phone_seq[l]);
			_kb->increment(symbol1, symbol2);
		}
		int symbol1 = _dic->operator[]("^");
		int symbol2 = _dic->operator[](ws0[k].back());
		_kb->increment(symbol1, symbol2);
	}

	for (size_t i = 1; i < sentence.size(); i++)
	{
		_topo->addInterWordLink(sentence[i - 1], sentence[i], _dic);
		vector<vector<string>> ws1 = _dic->getPhoneSequence(sentence[i - 1]);
		vector<vector<string>> ws2 = _dic->getPhoneSequence(sentence[i]);

		for (size_t j = 0; j < ws1.size(); j++)
		{
			for (size_t k = 0; k < ws2.size(); k++)
			{
				vector<string> phone_seq = ws1[j];
				phone_seq.insert(phone_seq.end(), ws2[k].begin(), ws2[k].end());
				for (size_t l = 1; l < phone_seq.size(); l++)
				{
					int symbol1 = _dic->operator[](phone_seq[l - 1]);
					int symbol2 = _dic->operator[](phone_seq[l]);
					_kb->increment(symbol1, symbol2);
				}
				int symbol1 = _dic->operator[](ws1[j].back());
				int symbol2 = _dic->operator[](ws2[k].back());
				_kb->increment(symbol1, symbol2);
			}
		}
	}
	/*for (size_t i = 1; i < sentence.size(); i++)
	{
		vector<vector<string>> ws1 = _dic->getPhoneSequence(sentence[i]);
		for (size_t j = 0; j < ws1.size(); j++)
		{
			vector<string> phone_seq = ws1[j];
			for (size_t l = 1; l < phone_seq.size(); l++)
			{
				int symbol1 = _dic->operator[](phone_seq[l - 1]);
				int symbol2 = _dic->operator[](phone_seq[l]);
				_kb->increment(symbol1, symbol2);
			}
		}
	}*/

}
#define TEST_TRAINING
#ifdef TEST_TRAINING

int main(int argc, char const *argv[])
{
	training * t = new training();
	t->processing(argv[1]);
	return 0;
}
#endif