#include <bits/stdc++.h>
using namespace std;

/* 
    Digit DP
    => a technique used to count numbers in a given range [L, R] 
    that satisfy a certain property based on their digits. 
    The numbers L and R can be very large (up to 10^18), 
    so simple iteration is impossible.

    time: O(D * S * 10)
    space: O(D * S)

    D = number of digits in N
    S = total number of combinations of all extra states.
*/

string num;
long long dp[20][...];

long long solve(int pos, bool tight, bool started, ...) {
    if (pos == num.size()) {
        // Check final condition
    }
    if (!tight && memo exists) return memo;

    int limit = tight ? num[pos] - '0' : 9;
    long long ans = 0;
    for (int d = 0; d <= limit; d++) {
        bool newStarted = started || (d != 0);
        bool newTight = tight && (d == limit);

        ans += solve(pos + 1, newTight, newStarted, updated_states...);
    }

    if (!tight) memo = ans;
    return ans;
}

/*
    Variant 1 — Accumulative State

    Q: Count numbers ≤ N whose digit sum = K
*/

string s;
long long dp[20][170];
int K;

long long dfs(int pos, bool tight, bool started, int sum) {
    if (pos == s.size()) return started && sum == K;
    if (!tight && dp[pos][sum] != -1) return dp[pos][sum];

    int limit = tight ? s[pos]-'0' : 9;
    long long ans = 0;
    for(int d=0; d<=limit; d++) {
        bool ns = started || d;
        ans += dfs(pos+1, tight && d==limit, ns, ns ? sum+d : sum);
    }

    if(!tight) dp[pos][sum] = ans;
    return ans;
}

int main() {
    memset(dp, -1, sizeof(dp));
    long long N=100;
    s = to_string(N); K=2;
    cout << dfs(0, 1, 0, 0);
}

/*
    Variant 2 — Previous Digit

    Q: Count numbers without consecutive equal digits.
*/

string s;
long long dp[20][11];

long long dfs(int pos, bool tight, bool started, int prev) {
    if(pos==s.size()) return 1;

    if(!tight && dp[pos][prev] != -1 && started)
        return dp[pos][prev];

    int limit = tight?s[pos]-'0':9;
    long long ans = 0;
    for(int d=0; d<=limit; d++) {
        bool ns = started || d;

        if(ns && started && d==prev) continue;
        ans += dfs(pos+1, tight&&d==limit, ns, ns?d:10);
    }

    if(!tight && started) dp[pos][prev] = ans;
    return ans;
}

/* 
    Variant 3 — Flag State

    Q: Count numbers containing digit 7.
*/

string s;
long long dp[20][2];

long long dfs(int pos, bool tight, bool started, bool seen) {
    if(pos == s.size()) return started && seen;

    if(!tight && dp[pos][seen]!=-1) return dp[pos][seen];

    int limit = tight?s[pos]-'0':9;
    long long ans = 0;
    for(int d=0; d<=limit; d++) {
        bool ns = started||d;
        bool newSeen = seen;

        if(ns && d==7) newSeen = true;
        ans += dfs(pos+1, tight&&d==limit, ns, newSeen);
    }

    if(!tight) dp[pos][seen] = ans;
    return ans;
}

/*
    Variant 4 — Combined State

    Q: Count numbers
        digit sum = K
        no consecutive equal digits
*/

long long dfs( int pos, bool tight, bool started, int sum, int prev) {
    if(pos==s.size()) return started && sum==K;

    int limit = tight?s[pos]-'0':9; 
    long long ans=0;
    for(int d=0;d<=limit;d++) {
        bool ns=started||d;

        if(ns && started && d==prev) continue;
        ans += dfs(pos+1, tight&&d==limit, ns, ns?sum+d:sum, ns?d:10);
    }

    return ans;
}

/*
    Variant 5 — Bitmask State

    Q: Count numbers whose digits are all unique.
*/

string s;
long long dp[20][1<<10];

long long dfs(int pos,bool tight,bool started,int mask) {
    if(pos==s.size()) return 1;

    if(!tight && dp[pos][mask]!=-1) return dp[pos][mask];

    int limit = tight?s[pos]-'0':9;
    long long ans = 0;

    for(int d=0; d<=limit; d++) {
        bool ns = started||d;

        if(!ns) {
            ans+=dfs(pos+1, tight&&d==limit, 0, mask);
            continue;
        }

        if(mask&(1<<d)) continue;
        ans+=dfs(pos+1, tight&&d==limit, 1, mask|(1<<d));
    }

    if(!tight) dp[pos][mask] = ans;
    return ans;
}