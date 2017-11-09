#ifndef UTIL_H_
#define UTIL_H_

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <ctime>
#include <cmath>
#include <vector>
#include <string>

#include <stdint.h>   // for uint32_t
#include <limits.h>   // for CHAR_BIT
#include <assert.h>
#define NDEBUG

using namespace std;

extern uint64_t RAND_A;
extern uint64_t RAND_B;

int EDistance(const char* s1, const char* s2, int s1len, int s2len);
bool EDcheck(char* s1, char* s2, int s1len, int s2len, int tau);
unsigned long long MyRand64();
void SetRand();
void PrintBinary(uint32_t num);

inline int min3(const int a, const int b, const int c){
    int m = a;
    if (m > b) m = b;
    if (m > c) m = c;
    return m;
}

inline uint32_t rotl32 (uint32_t n, unsigned int c){
	return (n<<c) | (n>>(32-c));
}


inline uint32_t rotr32 (uint32_t n, unsigned int c){
  const unsigned int mask = (CHAR_BIT*sizeof(n)-1);

#ifndef Release
  assert ( (c<=mask) &&"rotate by type width or more");
#endif
  c &= mask;  // avoid undef behaviour with NDEBUG.  0 overhead for most types / compilers
  return (n>>c) | (n<<( (-c)&mask ));
}


inline uint64_t rotl64 (uint64_t n, unsigned int c){
	return (n<<c) | (n>>(64-c));
}

inline uint64_t rotr64 (uint64_t n, unsigned int c){
  const unsigned int mask = (CHAR_BIT*sizeof(n)-1);

#ifndef Release
  assert ( (c<=mask) &&"rotate by type width or more");
#endif
  c &= mask;  // avoid undef behaviour with NDEBUG.  0 overhead for most types / compilers
  return (n>>c) | (n<<( (-c)&mask ));
}

inline uint32_t Hash32(unsigned int x){
	return x*(uint32_t)(2654435761)+(uint32_t)(3435961857);
}

inline uint64_t Hash64(unsigned int x){
	return x*RAND_A+RAND_B;
}

class Int{
public:
	int num;

	Int(){
		num=0;
	}
};

string extractFilename(const char* filename);
void quit();

bool PrimeTest(unsigned long long p);

template<class T>
void VectorPreprocessing(vector<T>& v, T u){
	if(v.size()<2)
		return ;

	int p1, p2;
	
	sort(v.begin(), v.end());
	p1=0;
	p2=1;
	while(v[0]==u){
		v.erase(v.begin());
	}

	while(p2<v.size()){
		if(v[p2]==u || v[p1]==v[p2]){
			v.erase(v.begin()+p2);
			continue;
		}
		p1++;
		p2++;
	}
	
}



template<class T>
int IntersectionSize(const vector<T>& v1, const vector<T>& v2){
	int i=0, j=0;
	int count=0;

	while(i<v1.size()&&j<v2.size()){
		if(v1[i]==v2[j]){
			count++;
			i++;
			j++;
			continue;
		}
		if(v1[i]<v2[j]){
			i++;
			continue;
		}
		if(v1[i]>v2[j]){
			j++;
			continue;
		}
	}

	return count;
}


template<class T>
int IntersectionSize(const T* v1, const T* v2, int s1, int s2, T u){
	int i=upper_bound(v1, v1+s1, u)-v1;
	int j=lower_bound(v2, v2+s2, v1[i])-v2;
	int count=0;

	while(i<s1&&j<s2){
		if(v1[i]==v2[j]){
			count++;
			i++;
			j++;
			continue;
		}
		if(v1[i]<v2[j]){
			i++;
			continue;
		}
		if(v1[i]>v2[j]){
			j++;
			continue;
		}
	}

	return count;
}


template<class T>
inline bool VectorEq(const vector<T>& v1, const vector<T>& v2){
	if(v1.size()!=v2.size())
		return false;

	for(int i=0; i<v1.size(); i++){
		if(v1[i]!=v2[i])
			return false;
	}

	return true;
}


template<class T>
bool IsIntersect(const vector<T>& v1, const vector<T>& v2){
    int i=0, j=0;

    while(i<v1.size()&&j<v2.size()){
        if(v1[i]==v2[j]){
			return true;
        }
        if(v1[i]<v2[j]){
			i++;
        }
        if(v1[i]>v2[j]){
			j++;
        }
    }

    return false;
}

#endif

