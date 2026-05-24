/*
1820ANKIT2029 (AGENTVEER)

g++ Q.cpp && ./a.exe < input.txt > output.txt && diff --color=always output.txt actualOutput.txt | less -R
*/

#include <bits/stdc++.h>
using namespace std;

#define FAST_IO ios::sync_with_stdio(false); cin.tie(nullptr)

// loop
#define FOR(i, x, n) for(int (i)=(x); (i)<(n); (i)++)
#define rFOR(i, x, n) for(int (i)=(x); (i)>=(n); (i)--)  // eg: x=n-1, x=0

// sort
#define srt(v) sort((v).begin(), (v).end())
#define srtR(v) sort(v.rbegin(), v.rend())

#define Left(p)     (p<<1)+1
#define Right(p)    (p<<1)+2

typedef long long ll;
typedef unsigned long long ull;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef pair<double, int> pdi;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<pii> vii;
typedef vector<pll> vll;
typedef vector<bool> vb;
typedef vector<vi> vvi;
typedef vector<vl> vvl;
typedef vector<string> vs;

const ll MOD = 1e9 + 7;
const ll inf = 1e18;
const double EPS = 1e-9;
const int inx[] = {0, 1, -1, 0};
const int iny[] = {1, 0, 0, -1};
const vector<string> stepDir = {"R","D","U","L"};
const int PrimeSize = 100001;

// bool cmp(pll &a, pll &b){
//     return (a.second - a.first)> (b.second - b.first);
// }

bool cmp(const pii &a, const pii &b){
    return a.first < b.first;
}

/*
long long sieve_size;
bitset<10000010> bs;                     // 10^7 limit
vector<long long> prime;

void sieve(long long upperbound) {
    sieve_size = upperbound + 1;
    bs.set();
    bs[0] = bs[1] = 0;

    for(long long i=2; i<sieve_size; i++) {
        if(bs[i]) {
            for(long long j=i*i; j<sieve_size; j+=i) bs[j] = 0;
            prime.push_back(i);
        }
    }
}
*/

double fact(double x) {
	double ans = 1;
	for(int i=1; i<=(int)x; i++) ans *= (double)i;
	return ans;
}

void solve() {
    int m, s;
	cin >> m >> s;
	
	if(m == 1 && s == 0) {
		cout << "0 0" << endl;
		return;
	}
	
	if(!(s > 0 && s <= 9 * m)) {
		cout << -1 << " " << -1 << endl;
		return;
	}
	
	string maxN(m, '0'), minN(m, '0');
	int idx = 0;
	while(s > 0) {
		maxN[idx++] = '0' + min(9, s);
		s -= min(9, s);
	}
	
	minN = maxN;
	reverse(minN.begin(), minN.end());
	
	if(minN[0] == '0') {
		minN[0] = '1';
		
		for(int i=1; i<m; i++){
			if(minN[i] != '0'){
				minN[i]--;
				break;
			}
		}
	}
	
	cout << minN << " " << maxN << endl;
	
}

int main(){
	FAST_IO;
    ll t;

    t = 1;
    //cin >> t;
    while(t--)
        solve();
}

/*
g++ Q.cpp && ./a.exe < input.txt > output.txt && diff --color=always output.txt actualOutput.txt | less -R
*/
