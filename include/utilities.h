#ifndef UTILITIES_H
#define UTILITIES_H
#define E 2.71828182846
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
using namespace std;

template<typename T>
static T From_string(std::string in_val)
{
	T output;
	std::stringstream temp;
	temp << in_val;
	temp >> output;
	return output;
}

template<typename T>
static std::string To_string(T in_val)
{
	std::ostringstream temp;
	temp << in_val;
	return temp.str();
}
template<typename T>
static string toSerializeStrings(vector<T> s)
{
	string ret = "";
	if (s.size()==0)  
	{
		return ret;
	}
	for (size_t i = 0; i < s.size()-1; i++)
	{
		ret += To_string<T>(s[i]) + " ";
	}
	ret += To_string<T>(s.back());
	return ret;
}
template<typename T>
static vector<T> fromSerializeStrings(string s)
{
	vector<T> ret;
	if (s=="")
	{
		return ret;
	}
	while (s.find(" ")!=string::npos)
	{
		ret.push_back(From_string<T>(s.substr(0, s.find(" "))));
		s = s.substr(s.find(" ") + 1);
	}
	ret.push_back(From_string<T>(s));
	return ret;
}

static double Zscore(double x, double u, double d)
{
	return (x - u) / d;
}
static double mean(vector<double> v)
{
	double sum = 0.0;
	for (size_t i = 0; i < v.size(); i++)
	{
		sum += v[i];
	}
	return (double)sum / (v.size()*1.0);
}
static double deviation(vector<double> v, double u)
{
	double sum = 0.0;
	for (size_t i = 0; i < v.size(); i++)
	{
		sum += (v[i] - u)*(v[i] - u);
	}
	return (double)sqrt(sum / (v.size()*1.0));
}
static double sigmoid(double t)
{
	double S;
	S = (double)t/(1.0+abs(t));
	return S;
}


#endif
