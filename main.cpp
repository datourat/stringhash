#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "stringsim.h"
#include "Util.h"

using namespace std;

int Tau=3;
int Q=2;
int K=-1;

int main(int argc, char* argv[]){
	SetRand();
	
	string filename;
	string queryfile;
	StringSim collection;

	int i=1;
	while(i<argc){
		if (strcmp("-q", argv[i]) == 0) {
			i++;
			Q = atoi(argv[i++]);
			if(Q>=8){
				cout << "q should be smaller than 8" << endl;
			}
		}
		else if (strcmp("-k", argv[i]) == 0) {
			i++;
			K = atoi(argv[i++]);
		}
		else if (strcmp("-tau", argv[i]) == 0) {
			i++;
			Tau = atoi(argv[i++]);
		}
		else if (strcmp("-query", argv[i]) == 0) {
			i++;
			queryfile = argv[i++];
		}
		else {
			filename = argv[i++];
		}
	}
	if(K==-1){
		K=Tau*Q*16/32;
	}
	cout << filename << endl;
	cout << "K: " << K << "\tQ: " << Q << "\tTau: " << Tau << endl;
	cout << "RAND_A: " << RAND_A << "\tRAND_B: " << RAND_B << endl;
	
	collection.ReadStrcol(filename);


	clock_t start, end;


	start=clock();
	collection.Occurencing();
	collection.Indexing();
	end=clock();
	cout << "indexing time: " << (double)(end-start)/CLOCKS_PER_SEC << endl;
	cout << "index size: " << (double)(collection.stringnum)*K*8/1024/1024 << "MB" << endl;

	if(queryfile!=""){
		collection.Query_hashscan(queryfile);
	}
	
	cout << endl;
	
}
