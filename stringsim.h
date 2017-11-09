#ifndef _STRINGSIM_H
#define _STRINGSIM_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>
#include <map>
#include <unordered_map>
#include <popcntintrin.h>
#include <algorithm>
#include "Util.h"

using namespace std;

extern int K;
extern int Q;
extern int Tau;

using POSWIDTH=uint64_t;

uint32_t HashGram(const vector<char>::iterator& beg, const int len, const int blocknum);

class GramHT{
public:
	int parnum;
	unordered_map<uint32_t, vector<int> > HT;

	void HTinsert(uint32_t, int);
};

class StringSim{
public:	
	uint32_t stringnum;
	int minlen;
	int maxlen;
	vector<uint64_t> strindex;
	vector<int> lengthindex;
	vector<char> strcol;
	vector<uint64_t> startpos;
	vector<uint8_t> occurence;
	vector< vector<uint64_t> > gramset;
	unordered_map<uint64_t, int> frequency;

	vector< pair<char*, char*> > candpair;
	vector< pair<int, int> > candpairlength;
	
	void ReadStrcol(const string& filename);
	void PrintStrcol(const string& filename);
	void GenQuery(const string& outfilename, int num, int diff);
	void Indexing();
	void Occurencing();
	void Query_hashscan(const string& queryfile);
	
	
	StringSim(){
		stringnum=0;
	}
	~StringSim(){

	}
};


#endif
