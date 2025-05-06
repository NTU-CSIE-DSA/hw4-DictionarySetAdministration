#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

const int MAX_LENGTH=300000;
const int QMAX=10000;

int main(int argc, char *argv[]) {
	int length=0;
	int TYPEMAX=atoi(argv[1]);
	registerValidation();
	int Q=inf.readInt(1, QMAX, "Q");
	inf.readEoln();
	set<string> st;
	for(int i=0;i<Q;i++) {
		int t=inf.readInt(1, TYPEMAX, "type");
		if(t==6) {
			ensuref(i==Q-1, "compress is not last operation\n");
		}
		if(i==0) {
			ensuref(t==1, "first operation is not insert\n");
		}
		if(t==6) {
			inf.readEoln();
			continue;
		}
		inf.readSpace();
		string str;
		str=inf.readString();
		for(int j=0;j<(int)str.size();j++) {
			ensuref(islower(str[j])!=0, "alphabet is not in lowercase\n");
		}
		ensuref((int)str.size()<=QMAX, "length of one input string is over limit\n");
		if(t==1) {
			ensuref(st.find(str)==st.end(), "inserted string is not unique\n");
			st.insert(str);
		}
		//cout<<str<<"\n";
		length+=(int)str.size();
		//inf.readEoln();
	}
	ensuref(length<=MAX_LENGTH, "total length is over limit\n");
	inf.readEof();
	return 0;
}
