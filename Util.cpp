#include "Util.h"

uint64_t RAND_A;
uint64_t RAND_B;

int EDistance(const char* s1, const char* s2, int s1len, int s2len){
    unsigned int lastdiag, olddiag;
//	unsigned int column[s1len+1];
	static vector<int> column(400);
	
	column.resize(s1len+1);
    for (int y = 0; y <= s1len; y++)
        column[y] = y;
    for (int x = 1; x <= s2len; x++) {
        column[0] = x;
        for (int y = 1, lastdiag = x-1; y <= s1len; y++) {
            olddiag = column[y];
            column[y] = min3(column[y] + 1, column[y-1] + 1, lastdiag + (s1[y-1] == s2[x-1] ? 0 : 1));
            lastdiag = olddiag;
        }
    }
    return(column[s1len]);
}

bool EDcheck(char* s1, char* s2, int s1len, int s2len, int tau){
	if(s2len>s1len){
		char* stmp=s1;
		s1=s2;
		s2=stmp;
		int slentmp=s1len;
		s1len=s2len;
		s2len=slentmp;
	}
	
	const int delta=s1len-s2len;
	const int upper=(tau+delta)/2;	//underflow
	
#ifndef Release
	if(tau<delta){
		cout << "tau<delta\t" << tau << '\t' << delta << endl;
		quit();	
	}
#endif

	if(__builtin_expect(upper==0, 0)){
		while(__builtin_expect(*s1!=NULL, 1)){
			if(__builtin_expect(*s1!=*s2, 0)){
				tau--;
				if(tau<0)
					return false;
			}
			s1++;
			s2++;
		}
	} else {
		const int lower=(tau-delta)/2;	//underflow
		const int length=lower+upper+1;
		const int ltmp=length+s1len-upper;
		const int lefttmp=delta+lower;
		const int righttmp=delta+lower;
		int arr[80];
		int* column=&arr[1];
		
#ifndef Release
		if(length > 78){
			cout << "length > 78:\t" << length << endl;
			quit();
		}
#endif

		int i=0;
		arr[0]=0xffff;
		for(; i<lower; i++){
			column[i]=lower-i;
		}
		for(int j=0; j<=upper; i++, j++){
			column[i]=j;
		}
		column[i]=0xffff;
		
		s1--;
		s2--;
		int left=0;
		int right=length-1;
		int nextvalue;
		for(i=1; i<=s2len; i++){
			for(int j=max(left, lower+1-i); j<=right && j<ltmp-i; j++){	//lower+1-i, ltmp-i are used to set the matrix limit
				column[j]=min3(column[j-1]+1, column[j+1]+1, column[j]+(s2[i]==s1[j-(lower-i)] ? 0 : 1));
			}

			if(column[lower+delta]>tau)	//lower+delta is the diagnoal containing the final value, and lefttmp==righttmp==lower+delta
				return false;

			if(column[left]+lefttmp-left>tau){
				column[left]=0xffff;
				left++;
			}
			if(column[right]+right-righttmp>tau){
				column[right]=0xffff;
				right--;
			}
		}

		return true;
	}
}

unsigned long long MyRand64(){
	unsigned long long ret, tmp;
	ret=rand();
	ret=ret<<33;
	tmp=rand();
	ret=ret|(tmp<<2);
	tmp=rand();
	ret=ret|(tmp>>29);

	return ret;
}

void SetRand(){
	srand(time(0));
	
	uint64_t tmp;
	RAND_A=rand();
	RAND_A=RAND_A<<33;
	tmp=rand();
	RAND_A=RAND_A|(tmp<<2);
	if(rand()%2)
		RAND_A=RAND_A|1;
	else
		RAND_A=RAND_A|3;

	RAND_B=rand();
	RAND_B=RAND_B<<33;
	tmp=rand();
	RAND_B=RAND_B|(tmp<<2);
	if(rand()%2)
		RAND_B=RAND_B|1;
	else
		RAND_B=RAND_B|3;
}

string extractFilename(const char* filename){
	string name(filename);
	int pos=name.find_last_of('.');

	return name.substr(0, pos);
}

void quit(){
	system("pause");
	exit(0);
}

