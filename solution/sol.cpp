#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define rep(i, n) for (ll i = 0; i < n; i++)
const int N = 3e5 + 5;
const int W = 26;
int n, ch[N][W], cnt[N], prefixsum[N], pos;
priority_queue<int> pq[N];

void Insert(string &s)
{
    int now = 0;
    prefixsum[0]++;
    for (auto to : s)
    {
        if (ch[now][to - 'a'] == 0)
            ch[now][to - 'a'] = ++pos;
        now = ch[now][to - 'a'];
        prefixsum[now]++;
    }
    cnt[now]++;
}

bool Check(string &s)
{
    int now = 0;
    for (auto to : s)
    {
        if (ch[now][to - 'a'] == 0)
            return false;
        now = ch[now][to - 'a'];
    }
    if(cnt[now]!=0) return true;
    else return false;
}

int PrefixSearch(string &s)
{
    int now = 0;
    for (auto to : s)
    {
        if (ch[now][to - 'a'] == 0)
            return 0;
        now = ch[now][to - 'a'];
    }
    return prefixsum[now];
}

string LCP(string &s)
{
    string ret = "";
    int now = 0;
    for (auto to : s)
    {
        if (ch[now][to - 'a'] == 0)
        {
            break;
        }
        ret += to;
        now = ch[now][to - 'a'];
    }
    bool flag = true;
    while (flag)
    {
        if (cnt[now] != 0)
            return ret;
        flag = 0;
        rep(i, 26)
        {
            if (ch[now][i] != 0)
            {
                flag = 1;
                ret += ('a' + i);
                now = ch[now][i];
                break;
            }
        }
    }
    return ret;
}

ll Score(string &s)
{
    ll ret = 0;
    int pre = prefixsum[0], now = 0, dep = 0;
    for (auto to : s)
    {
        if (ch[now][to - 'a'] == 0)
            break;
        now = ch[now][to - 'a'];
        if (prefixsum[now] != pre)
        {
            ret += (dep * (pre - prefixsum[now]));
        }
        dep++;
        pre = prefixsum[now];
    }
    ret += dep * prefixsum[now];
    return ret;
}

void Compress(int now, int dep)
{
    rep(i, 26)
    {
        if (ch[now][i])
        {
            int nxt = ch[now][i];
            Compress(nxt, dep + 1);
            while (!pq[nxt].empty())
            {
                pq[now].push(pq[nxt].top());
                pq[nxt].pop();
            }
        }
    }
    if (!pq[now].empty() && now != 0 && cnt[now] == 0)
    {
        pq[now].pop();
        pq[now].push(dep);
    }
    else
        pq[now].push(dep);
}

signed main()
{
    int Q;
    cin >> Q;
    rep(i, Q)
    {
        int ty;
        cin >> ty;
        if (ty == 1)
        {
            string s;
            cin >> s;
            Insert(s);
        }
        else if (ty == 2)
        {
            string s;
            cin >> s;
            if (Check(s))
                cout << "YES\n";
            else
                cout << "NO\n";
        }
        else if (ty == 3)
        {
            string s;
            cin >> s;
            cout << PrefixSearch(s) << "\n";
        }
        else if (ty == 4)
        {
            string s;
            cin >> s;
            cout << LCP(s) << "\n";
        }
        else if (ty == 5)
        {
            string s;
            cin >> s;
            cout << Score(s) << "\n";
        }
        else if (ty == 6)
        {
            Compress(0, 0);
            int ans = 0;
            while (!pq[0].empty())
            {
                ans += pq[0].top();
                pq[0].pop();
            }
            cout << ans << "\n";
        }
    }
}
