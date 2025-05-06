#include <bits/stdc++.h>
#include <assert.h>
#include <string.h>
#include "testlib.h"
using namespace std;

const int MAX_LENGTH = 300000;
const int QMAX = 10000;

string genstring(int length, int alphabet)
{
	string ret = "";
	for (int i = 0; i < length; i++)
	{
		char c = 'a' + rnd.next(0, alphabet - 1);
		ret += c;
	}
	return ret;
}

int main(int argc, char *argv[])
{
	registerGen(argc, argv, 1);
	int rnd_seed = atoi(argv[1]);
	int TYPE_MAX = 3;
	string gentype = argv[2];
	int Q = (atoi(argv[3]) == 0 ? rnd.next(1, QMAX) : atoi(argv[3]));
	int len = (atoi(argv[4]) == 0 ? rnd.next(1, QMAX) : atoi(argv[4]));
	int insert_ratio = (atoi(argv[5]) == 0 ? 5 : atoi(argv[5]));
	int alphabet_size = (atoi(argv[6]) == 0 ? 26 : atoi(argv[6]));
	set<string> st;
	vector<string> inserted;
	vector<string> prefix_vec;
	if (gentype == "random")
	{
		cout << Q << "\n";
		string prefix = "";
		int insert_count = Q * insert_ratio / 10;
		int expect = 0;
		for (int i = 0; i < insert_count; i++)
		{
			if (expect == 0)
			{
				int str_len = rnd.next(1, len * 2 / 3);
				prefix = genstring(str_len, alphabet_size);
				prefix_vec.push_back(prefix);
				expect = rnd.next(1, min(insert_count - i, insert_count / (Q - insert_count)));
			}
			int extra_len = len - (int)prefix.size();
			expect--;
			string suffix = "";
			suffix = genstring(extra_len, alphabet_size);
			string now = prefix + suffix;
			while (st.find(now) != st.end())
			{
				suffix = genstring(extra_len, alphabet_size);
				now = prefix + suffix;
			}
			cout << "1 " << now << "\n";
			inserted.push_back(now);
			st.insert(now);
			int dif = rnd.next(0, 10);
			if (dif >= 3 && (int)prefix.size() > 1)
				prefix = prefix.substr(0, (int)prefix.size() - 1);
			else if (dif <= 2 && extra_len > 1)
				prefix = prefix + suffix[0];
		}
		int insert_sz = inserted.size(), pv_sz = prefix_vec.size();
		for (int i = insert_count; i < Q; i++)
		{
			int type = rnd.next(1, 6);
			if (type == 2)
			{
				int ans = rnd.next(0, 1);
				string ask = "";
				if (ans == 1)
				{
					ask = inserted[rnd.next(0, insert_sz - 1)];
				}
				else
				{
					ask = prefix_vec[rnd.next(0, pv_sz - 1)];
				}
				cout << "2 " << ask << "\n";
			}
			else if (type == 3)
			{
				int id = rnd.next(0, pv_sz - 1);
				string ask = prefix_vec[id];
				cout << "3 " << ask << "\n";
				if ((int)prefix_vec[id].size() > 1)
					prefix_vec[id] = prefix_vec[id].substr(0, (int)prefix_vec[id].size() - 1);
			}
			else if(type==4)
			{
				int ans = rnd.next(0, 1), id = rnd.next(0, pv_sz - 1);
				string ask = "";
				if (ans == 1)
				{
					ask = inserted[rnd.next(0, insert_sz - 1)];
				}
				else
				{
					ask = prefix_vec[id];
				}
				cout << "4 " << ask << "\n";
			}
			else {
				int ans=rnd.next(0, 1);
				string ask="";
				if(ans==1) {
					ask = inserted[rnd.next(0, insert_sz - 1)];
				}
				else {
					ask=genstring(len, alphabet_size);
				}
				cout<<"5 "<<ask<<"\n";
			}
		}
		// generate a prefix
		// insert prefix+suffix as 1
		// 3 prefix
		// 3 short_prefix
		// 50% 2 existed
		// 50% 2 non-existed
		// 4 prefix+suffix
		// edge: prefix + suffix
	}
	else if (gentype == "tle")
	{
		// match at very behind, more 2 (including late miss), 3 prefix should be longer
		cout << Q << "\n";
		string prefix = "";
		int insert_count = Q * insert_ratio / 10;
		int expect = 0;
		for (int i = 0; i < insert_count; i++)
		{
			if (expect == 0)
			{
				int str_len = rnd.next(1, len * 2 / 3);
				prefix = genstring(str_len, alphabet_size);
				prefix_vec.push_back(prefix);
				expect = rnd.next(1, min(insert_count - i, insert_count / (Q - insert_count)));
			}
			int extra_len = len - (int)prefix.size();
			expect--;
			string suffix = "";
			suffix = genstring(extra_len, alphabet_size);
			string now = prefix + suffix;
			while (st.find(now) != st.end())
			{
				suffix = genstring(extra_len, alphabet_size);
				now = prefix + suffix;
			}
			cout << "1 " << now << "\n";
			inserted.push_back(now);
			st.insert(now);
			now[(int)now.size() - 1] = (now[(int)now.size() - 1] == 'z' ? 'a' : now[(int)now.size() - 1] + 1);
			if (expect > 0 && st.find(now) != st.end())
			{
				inserted.push_back(now);
				st.insert(now);
				expect--;
			}
			int dif = rnd.next(0, 10);
			if (dif >= 3 && (int)prefix.size() > 1)
				prefix = prefix.substr(0, (int)prefix.size() - 1);
			else if (dif <= 2 && extra_len > 1)
				prefix = prefix + suffix[0];
		}
		int insert_sz = inserted.size(), pv_sz = prefix_vec.size();
		for (int i = insert_count; i < Q; i++)
		{
			int type = rnd.next(1, (rnd_seed%2==1?1:5));
			if (type == 2)
			{
				int ans = rnd.next(0, 2);
				string ask = "";
				if (ans != 0)
				{
					ask = inserted[rnd.next(0, insert_sz - 1)];
				}
				else
				{
					string tmp = inserted[rnd.next(0, insert_sz - 1)];
					tmp[(int)tmp.size() - 1] = ((tmp[(int)tmp.size() - 1] == 'z' || tmp[(int)tmp.size() - 1] == 'y') ? tmp[(int)tmp.size() - 1] - 24 : tmp[(int)tmp.size() - 1] + 2);
					ask = tmp;
				}
				cout << "4 " << ask << "\n";
			}
			else
			{
				int ans = rnd.next(0, 2);
				string ask = "";
				if (ans != 0)
				{
					ask = inserted[rnd.next(0, insert_sz - 1)];
				}
				else
				{
					ask=genstring(len, alphabet_size);
				}
				cout << "5 " << ask << "\n";
			}
		}
	}
}
