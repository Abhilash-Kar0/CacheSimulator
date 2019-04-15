#include<bits/stdc++.h>

using namespace std;
int asso1;
struct cacheBlock{
long long tag;
bool valid;
bool dirty;
	cacheBlock(long long t,bool v,bool d): tag(t),valid(v),dirty(d)
	{
	}
};
int log2(int n){
	int cnt=0;
	while(n>1){
		n/=2;
		cnt++;
	}
	if(n!=1)
		return -1;
	return cnt;
}

int random(int mod){
	return rand()%mod;
}

long parseStringtoInt(string s){			///finish
	return 0;
}
long long parsebinToInt(string s){
	if(s=="")
	return 0;
	long long res = s[0]-48;			//finish
	for(int i=1;i<s.size();i++){
		res*=2;
		res+=(s[i]-48);
	}
	return res;
}
string parsehexTobin(string s){
	string res = "";
	for(int i = 0 ; i < 8 ; i++)
	{
		char digit = s[i];
		int ascii = digit;
		switch(ascii)
		{
			case 48: res += "0000" ; break;
			case 49: res += "0001" ; break;
			case 50: res += "0010" ; break;
			case 51: res += "0011" ; break;
			case 52: res += "0100" ; break;
			case 53: res += "0101" ; break;
			case 54: res += "0110" ; break;
			case 55: res += "0111" ; break;
			case 56: res += "1000" ; break;
			case 57: res += "1001" ; break;
			case 65: res += "1010" ; break;
			case 66: res += "1011" ; break;
			case 67: res += "1100" ; break;
			case 68: res += "1101" ; break;
			case 69: res += "1110" ; break;
			case 70: res += "1111" ; break; 
		}
	}
	return res;
}
class Cache{
public:
	vector<vector<cacheBlock>> tagArray;
	int num_sets;
	int associativity;
	int num_of_blocks;
	int replacePol;
	int block_Size;
	int bitsnS,bitsBlock;
	
	
	
	int NumberofCacheAccesses=0;
	int NumberofReadAccesses=0;
	int NumberofWriteAccesses=0;
	int NumberofCacheMisses=0;
	int NumberofCompulsoryMisses=0;
	int NumberofCapacityMisses=0;
	int NumberofConflictMisses=0;
	int NumberofReadMisses=0;
	int NumberofWriteMisses=0;
	int NumberofDirtyBlocksEvicted=0;

	int NumberofHits=0;

	unordered_set<long long> AllBlocks;



	Cache(int nS, int ass , int nB, int rep, int bs ): num_sets(nS),associativity(ass), num_of_blocks(nB),replacePol(rep),block_Size(bs)
	{
		
		for(int i=0;i<num_sets;i++){
			vector<cacheBlock> v;
			tagArray.push_back(v);
		}
		bitsnS=log2(num_sets);
		bitsBlock=log2(block_Size);
		//fout<<bitsBlock<<" "<<bitsnS<<" test"<<endl;
	}

	void read_instruction(int Rpol1){
		ifstream fin;
		fin.open("input.txt");
		long cSize,bSize,asso,Rpol;
		fin>>cSize>>bSize>>asso>>Rpol;
		string s;
		while(!fin.eof()){
		fin>>s;
		if(fin.eof())
			break;
		//fout<<s<<"s"<<endl;
		//for(int i= 0;i<100;i++){
		//fin>>s;
		//fout.flush();
		string bs = parsehexTobin(s);
		//fout<<bs<<"bs ";
		string tagstr = bs.substr(1,32-bitsnS-bitsBlock-1);
		long long tagInt=parsebinToInt(tagstr);
		int setI=parsebinToInt(bs.substr(32-bitsnS-bitsBlock,bitsnS));
		//fout<<setI<<" instru is okay"<<endl;
		long long tag_set = parsebinToInt(bs.substr(1,32-bitsBlock-1));
		NumberofCacheAccesses++;
		if(bs[0]=='0')
		read(tagInt,setI,tag_set);
		else
		write(tagInt,setI,tag_set);
		}
		fin.close();
		print(Rpol1);
		
	}
	
	void print(int Rpol1){
		ofstream fout;
		fout.open("output.txt");
		
		fout<<num_of_blocks*block_Size<<endl;
		fout<<block_Size<<endl;
		fout<<asso1<<endl;
		fout<<Rpol1<<endl;
		
		fout<<NumberofCacheAccesses<<endl;
		fout<< NumberofReadAccesses<<endl;
		fout<< NumberofWriteAccesses<<endl;
		fout<< NumberofCacheMisses<<endl;
		fout<< NumberofCompulsoryMisses<<endl;
		fout<< NumberofConflictMisses<<endl;
		fout<< NumberofCapacityMisses<<endl;
		
		fout<< NumberofReadMisses<<endl;
		fout<< NumberofWriteMisses<<endl;
		fout<< NumberofDirtyBlocksEvicted<<endl;
		//fout<<"NumberofHits "<<NumberofHits<<endl;
		fout.close();
	}
	void read(long long tagInt,int setI,long long tag_set){
		NumberofReadAccesses++;
		bool Compulsory=false;
		if(!AllBlocks.count(tag_set)){
			NumberofCompulsoryMisses++;
			Compulsory=true;
		}
		AllBlocks.insert(tag_set);
		for(int i=0;i<tagArray[setI].size();i++){
			if(tagArray[setI][i].tag==tagInt){
				if(replacePol==0){
					;//random replacement plolicy
				}
				if(replacePol==1){
					cacheBlock c1=tagArray[setI][i];
					tagArray[setI].erase(tagArray[setI].begin()+i);
					tagArray[setI].push_back(c1);
				}
				if(replacePol==2){				//Pseudo-LRU
					cacheBlock c1=tagArray[setI][i];
					tagArray[setI].erase(tagArray[setI].begin()+i);
					tagArray[setI].push_back(c1);
				}
				NumberofHits++;						
				return;
			}
		}
		NumberofCacheMisses++;
		NumberofReadMisses++;
		if(!Compulsory && num_sets==1 && tagArray[setI].size() == num_of_blocks )//for fully associative
			NumberofCapacityMisses++;
		if(!Compulsory && tagArray[setI].size()==associativity && !(num_sets==1))
			NumberofConflictMisses++;
		
		cacheBlock c1(tagInt,1,0);
		if(tagArray[setI].size()<associativity){
			tagArray[setI].push_back(c1);
			
		}
		else{
			if(replacePol==0){
				int i = random(associativity);
				if(tagArray[setI][i].dirty==1)
					NumberofDirtyBlocksEvicted++;
				tagArray[setI][i] = c1;
			}
			if(replacePol==1){
				if(tagArray[setI][0].dirty==1)
					NumberofDirtyBlocksEvicted++;
				tagArray[setI].erase(tagArray[setI].begin()+0);
				tagArray[setI].push_back(c1);
			}
			if(replacePol==2){
				int i = random(associativity-1);
				if(tagArray[setI][i].dirty==1)
					NumberofDirtyBlocksEvicted++;
				tagArray[setI].erase(tagArray[setI].begin()+i);
				tagArray[setI].push_back(c1);
			}
		}
	}



	void write(long long tagInt,int setI,long long tag_set){
		NumberofWriteAccesses++;
		bool Compulsory=false;
		if(!AllBlocks.count(tag_set)){
			NumberofCompulsoryMisses++;
			Compulsory=true;
		}
		AllBlocks.insert(tag_set);
		for(int i=0;i<tagArray[setI].size();i++){
			if(tagArray[setI][i].tag==tagInt){
				if(replacePol==0){
					tagArray[setI][i].dirty=1;
					;//random replacement plolicy
				}
				if(replacePol==1){
					tagArray[setI][i].dirty=1;
					cacheBlock c1=tagArray[setI][i];
					tagArray[setI].erase(tagArray[setI].begin()+i);
					tagArray[setI].push_back(c1);				}
				if(replacePol==2){				//Pseudo-LRU
					tagArray[setI][i].dirty = 1;
					cacheBlock c1=tagArray[setI][i];
					tagArray[setI].erase(tagArray[setI].begin()+i);
					tagArray[setI].push_back(c1);
				}
				NumberofHits++;						
				return;
			}
		}
		
		
		NumberofCacheMisses++;
		NumberofWriteMisses++;
		if(!Compulsory && tagArray[setI].size()== num_of_blocks && num_sets==1)//for fully associative
			NumberofCapacityMisses++;
		if(!Compulsory &&tagArray[setI].size()==associativity && !(num_sets==1))
			NumberofConflictMisses++;
		
		cacheBlock c1(tagInt,1,1);
		if(tagArray[setI].size()<associativity){
			tagArray[setI].push_back(c1);
			//fout<<"ok, 1"<<endl;
		}
		else{
			if(replacePol==0){
				int i = random(associativity);
				if(tagArray[setI][i].dirty==1)
					NumberofDirtyBlocksEvicted++;
				tagArray[setI][i] = c1;
			}
			if(replacePol==1){
				if(tagArray[setI][0].dirty==1)
					NumberofDirtyBlocksEvicted++;
				tagArray[setI].erase(tagArray[setI].begin()+0);
				tagArray[setI].push_back(c1);
			}
			if(replacePol==2){
				int i = random(associativity-1);
				if(tagArray[setI][i].dirty==1)
					NumberofDirtyBlocksEvicted++;
				tagArray[setI].erase(tagArray[setI].begin()+i);
				tagArray[setI].push_back(c1);
			}
		}
	}
	

	
};

int main(){
//	fout<<argv[1];
//	string s1[5];
//	long cSize,bSize,asso,Rpol;
//	for(int i=0;i<argc-1;i++){
//		s1[i]=argv[i];
//	}
//	cSize=parseStringtoInt(s1[1]);
//	bSize=parseStringtoInt(s1[2]);
//	asso=parseStringtoInt(s1[3]);
//	Rpol=parseStringtoInt(s1[4]);
//	inputFile=argv[5];
	ifstream fin;
	//ofstream fout;
	fin.open("input.txt");
	
	long cSize,bSize,asso,Rpol,Rpol1;
	fin>>cSize>>bSize>>asso1>>Rpol1;
	asso=asso1;
	if(asso1==0)
	asso=cSize/bSize;
	Rpol=Rpol1;
	if(asso==1)
	Rpol=1;
	Cache c((cSize/bSize)/asso,asso,cSize/bSize,Rpol,bSize);
	fin.close();
	c.read_instruction(Rpol1);
	
	
	return 0;
}
