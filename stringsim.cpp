#include "stringsim.h"

void StringSim::ReadStrcol(const string& filename){
	ifstream fp(filename.c_str());/*{{{*/
	string buf;
	uint64_t startcount=0;
	stringnum=0;
	
	if(fp.is_open()==false){
		cout << "Fail to open " << filename << endl;
		quit();	
	}

	vector<string> tmpstringset;
	while(getline(fp, buf)){
		if(buf[buf.size()-1]=='\r')
			buf=buf.substr(0, buf.size()-1);
		tmpstringset.emplace_back(buf);
	}

	sort(tmpstringset.begin(), tmpstringset.end(), [](const string& a, const string& b)->bool{
		return a.size()<b.size();
	});
	minlen=tmpstringset.front().size();
	maxlen=tmpstringset.back().size();
	stringnum=tmpstringset.size();
	lengthindex.clear();
	lengthindex.resize(maxlen+2, -1);
	
	strcol.reserve(10000000);
	startpos.reserve(1000000);
	for(int i=0; i<Q; i++){
		strcol.emplace_back(NULL);
	}
	startcount=Q;

	int tmplength=minlen;
	for(int j=0; j<tmpstringset.size(); j++){
		strcol.insert(strcol.end(), tmpstringset[j].begin(), tmpstringset[j].end());
		for(int i=0; i<Q-1; i++){
			strcol.emplace_back(NULL);
		}
		startpos.push_back(startcount);
		startcount+=tmpstringset[j].size()+Q-1;
		while(tmplength!=tmpstringset[j].size()){
			if(lengthindex[tmplength]==-1)
				lengthindex[tmplength]=j;
			tmplength++;
		}
		if(lengthindex[tmplength]==-1)
			lengthindex[tmplength]=j;
	}
	startpos.push_back(startcount);
	
	if(tmplength!=maxlen)
		cout << "tmplength!=maxlen" << endl;
	lengthindex[maxlen+1]=stringnum;
	
	cout << "minlen: " << minlen << endl;
	cout << "maxlen: " << maxlen << endl;
	cout << "# of lines: " << stringnum << endl;
	cout << "total length of strings: " << strcol.size() << endl;
	
	fp.close();
	strcol.shrink_to_fit();
	startpos.shrink_to_fit();
	cout << "Read Complete" << endl;
	/*}}}*/
}

void StringSim::PrintStrcol(const string& filename){
/*{{{*/
	FILE *fp;
	fp=fopen(filename.c_str(), "w");
	if(fp==NULL){
		cout << "Fail to open " << filename << endl;
		quit();
	}
	
	for(int i=0; i<stringnum; i++){
		fwrite(strcol.data()+startpos[i], sizeof(char), startpos[i+1]-startpos[i]-Q+1, fp);
		fputc('\n', fp);
	}	
	
	fclose(fp);
/*}}}*/
}


void StringSim::GenQuery(const string& outfilename, int num, int diff){
	ofstream out(outfilename);/*{{{*/
	char replace=strcol[startpos[0]];
	for(int i=0; i<num; i++){
		int strid=rand()%stringnum;
		string str(strcol.begin()+startpos[strid], strcol.begin()+startpos[strid+1]-Q+1);
		int realdiff=rand()%diff;
		for(int j=0; j<realdiff; j++){
			int pos=rand()%(str.size());
			int tmp=rand()%3;
			if(str.size()==0)
				tmp=1;
			switch(tmp){
				case 0:	//delete
					replace=str[pos];
					str.erase(str.begin()+pos);
					break;
				case 1: //insert
					str.insert(str.begin()+pos, replace);
					break;
				case 2:	//replace
					char tmpchar=replace;
					replace=str[pos];
					str[pos]=tmpchar;
					break;
			}
		}
		out << str << endl;
	}

	out.close();/*}}}*/
}

void StringSim::Indexing(){
	int VARK=K;
	int VARQ=Q;
	uint32_t VARstringnum=stringnum;

	strindex.resize(VARK*VARstringnum, 0);
	
	uint64_t hashvalue;
	hashvalue=Hash64(0);
	for(int i=0; i<VARQ-1; i++){
		hashvalue=rotl64(hashvalue, 1);
		hashvalue^=Hash64(0);
	}


	uint64_t realvalue;
	uint64_t pos=VARQ;
	for(int i=0; i<VARstringnum; i++){
		for(; __builtin_expect(pos<startpos[i+1],0); pos++){
			hashvalue=rotl64(hashvalue, 1);
			hashvalue^=Hash64(strcol[pos]);
			hashvalue^=rotl64(Hash64(strcol[pos-VARQ]), VARQ);
			
			realvalue=rotl64(hashvalue, 1) ^ Hash64(occurence[pos]);
			realvalue=(realvalue >> (VARQ-1))%(VARK<<6);		//realvalue=(realvalue >> (VARQ-1))%(VARK*64);

#ifndef OXLABEL
			strindex[i*VARK+(realvalue >> 6)] ^= ((uint64_t)1 << (realvalue & 63));		// for XX label
#else
			strindex[i*VARK+(realvalue >> 6)] |= ((uint64_t)1 << (realvalue & 63));		// for OX label
#endif
		}
	}

}

void StringSim::Occurencing(){
/*{{{*/
	uint64_t mask=0;
	for(int i=0; i<Q; i++){
		mask <<= 8;
		mask |= 0xff;
	}

	uint64_t strcolsize=strcol.size();
	unordered_map<uint64_t, uint64_t> prev;
	occurence.resize(strcolsize, 11);
	prev.reserve(5000);
	uint64_t trans=0;
	
	for(int i=0; i<stringnum; i++){
		if(startpos[i+1]-startpos[i]<=2*Q-1)
			continue;
		trans=0;
		for(int64_t j=startpos[i], limitj=startpos[i]+Q-1; j<limitj; j++){
			trans <<= 8;
			trans |= strcol[j];
		}
		for(int64_t j=startpos[i]+Q-1, limitj=startpos[i+1]-Q+1; j<limitj; j++){
			trans <<= 8;
			trans &= mask;
			trans |= strcol[j];
			
			auto it=prev.find(trans);
			if(it!=prev.end()){
				if(it->second > startpos[i])
					occurence[j] = occurence[it->second]+1;
				it->second = j;
			} else {
				prev.emplace(trans, j);
			}
		}
	}
/*}}}*/
}

void StringSim::Query_hashscan(const string& queryfile){
	StringSim query;
	const int VARK=K;
	const int VARQ=Q;
	const int VARTau=Tau;
	int onecount;
	int threshold=2*VARQ*VARTau;
	clock_t start, end;
	query.ReadStrcol(queryfile);
	
	start=clock();
	query.Occurencing();
	query.Indexing();
	
	size_t totalcandidate=0;
	size_t realresult=0;
	for(int i=0; i<query.stringnum; i++){
		int candidatenum=0;
		const int iK=i*VARK;
		const int s1len=query.startpos[i+1]-query.startpos[i]-VARQ+1;
		const int lenlow=min(max(minlen, s1len-VARTau), maxlen+1);
		const int lenhigh=min(maxlen, s1len+VARTau)+1;
		const int iterstart=lengthindex[lenlow];
		const int iterend=lengthindex[lenhigh];
		for(int j=iterstart; j<iterend; j++){
			int s2len=startpos[j+1]-startpos[j]-VARQ+1;
			int lengthdiff=abs(s1len-s2len);
			if(lengthdiff>VARTau){
				continue;
			}
				
			int querythreshold=threshold-lengthdiff;
			const int jK=j*VARK;

			onecount=0;
			for(int k=0; k<VARK; k++){
				onecount+=_mm_popcnt_u64(query.strindex[iK+k]^strindex[jK+k]);
				if(onecount>querythreshold)
					break;
			}
			if(onecount<=querythreshold){
				totalcandidate++;
#ifndef TestVerify

				if(EDcheck(query.strcol.data() + query.startpos[i], strcol.data() + startpos[j], s1len, s2len, VARTau)){
					realresult++;
				}

#else
				candpair.push_back(make_pair(query.strcol.data() + query.startpos[i], strcol.data() + startpos[j]));
				candpairlength.push_back(make_pair(s1len, s2len));
#endif
			}
		}
	}
	end=clock();
	cout << "Average Query Time used: " << (double)(end-start)/CLOCKS_PER_SEC*1000/query.stringnum << "ms" << endl;
	cout << "Average Candidate Num: " << (double)totalcandidate/query.stringnum << endl;
	cout << "Average Real Result Num: " << (double)realresult/query.stringnum << endl;

#ifdef TestVerify
	clock_t verifystart, verifyend;
	int tmpcount=0;
	verifystart=clock();
	for(int i=0; i<candpair.size(); i++){
		if(EDcheck(candpair[i].first, candpair[i].second, candpairlength[i].first, candpairlength[i].second))
			tmpcount++;
	}
	verifyend=clock();
	cout << "tmpcount: " << tmpcount << endl;
	cout << "candpair size: " << candpair.size() << endl;
	cout << "Average Verify Time used: " << (double)(verifyend-verifystart)/CLOCKS_PER_SEC*1000/candpair.size() << "ms" << endl;
#endif

}

