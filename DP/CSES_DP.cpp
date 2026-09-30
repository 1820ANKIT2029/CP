#include <bits/stdc++.h>
using namespace std;

#define FAST_IO ios::sync_with_stdio(false); cin.tie(nullptr)

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, int> pli;
typedef pair<ll, ll> pll;
typedef pair<int, pii> pipii;

const ll MOD = 1e9 + 7;

class _1633 { // coin change type
	public:
	int n;
	vector<int> dp;
	
	int Memo(int n) {
		if(n == 0) return 1;
		if(dp[n] != -1) return dp[n];
		
		int ans = 0;
		for(int i=1; i<=6; i++) {
			if(i > n) continue;
			ans += Memo(n - i);
			ans %= MOD;
		}
		
		return dp[n] = ans;
	}
	
	int Tabu(int n) {
		for(int i=1; i<=n; i++) {
			for(int j=1; j<=6; j++) {
				if(j > i) continue;
				dp[i] += dp[i - j];
				dp[i] %= MOD;
			}
		}
		
		return dp[n];
	}
	
	void solve() {
		FAST_IO;
		cin >> n;
		dp.assign(n + 1, -1); // -1 for Memo and 0 for Tabu
		dp[0] = 1;
		
		cout << Memo(n) << endl;
	}
};

class _1634 { // coin change
	public:
	int n, x;
	vector<int> c;
	vector<int> dp;
	
	int Memo(int amt) {
		if (amt == 0) return 0;
        if (amt < 0) return 1e9;

        if (dp[amt] != -1) return dp[amt];

        int ans = 1e9;
        for (int coin : c) {
            ans = min(ans, Memo(amt - coin) + 1);
        }

        return dp[amt] = ans;
	}
	
	int Tabu() {
		for(int amt = 1; amt <= x; amt++) {
			for(int coin: c) {
				if(amt - coin < 0) continue;
				dp[amt] = min(dp[amt], dp[amt - coin] + 1);
			}
		}
		
		return dp[x];
	}
	
	void solve() {
		FAST_IO;
		cin >> n >> x;
		c.assign(n, 0);
		for(int i=0; i<n; i++) cin >> c[i];
		
		dp.assign(x+1, 1e9); // -1 for Memo and 1e9 for Tabu
		dp[0] = 0;
		
		int ans = Tabu();
		if(ans >= 1e9) cout << -1 << endl;
		else cout << ans << endl;
		
	}
};

class _1635 { // coin change
	public:
	int n, x;
	vector<int> c;
	vector<ll> dp;
	
	ll Memo(int amt) {
		if(amt == 0) return 1;
		if(amt < 0) return 0;
		
		if(dp[amt] != -1) return dp[amt];
		
		ll ans = 0;
		for(int coin: c) {
			ans += Memo(amt - coin);
			ans %= MOD;
		}
		
		return dp[amt] = ans;
	}
	
	int Tabu() {
		for(int amt=1; amt<=x; amt++) {
			for(int coin: c) {
				if(amt - coin < 0) continue;
				dp[amt] += dp[amt - coin];
				dp[amt] %= MOD;
			}
		}
		
		return dp[x];
	}
	
	void solve() {
		FAST_IO;
		cin >> n >> x;
		c.assign(n, 0);
		for(int &i: c) cin >> i;
		
		dp.assign(x+1, 0); // -1 for Memo and 0 for Tabu
		dp[0] = 1;
		
		cout << Tabu() << endl;
	}
};

class _1636 { // coin change
	public:
	int n, x;
	vector<int> c;
	vector<vector<ll>> dp;
	vector<ll> dp2;
	
	ll Memo(int ind, int amt) {
		if(amt == 0) return 1;
		if(amt < 0 || ind >= n) return 0;
		
		if(dp[ind][amt] != -1) return dp[ind][amt];
		
		ll ans = 0;
		ans += Memo(ind + 1, amt);
        ans += Memo(ind, amt - c[ind]);
		
		return dp[ind][amt] = ans % MOD;
	}
	
	ll Tabu() {
		for (int i = 0; i <= n; i++) dp[i][0] = 1;

		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= x; j++) {
				// Option 1: Exclude the i-th coin (look at the row above)
				int exclude = dp[i - 1][j];

				// Option 2: Include the i-th coin (look at the same row, current sum - coin value)
				int include = 0;
				if (j >= c[i - 1]) {
					include = dp[i][j - c[i - 1]];
				}

				dp[i][j] = (exclude + include) % MOD;
			}
		}

		return dp[n][x];
	}
	
	ll Tabu_optimized() {
		dp2[0] = 1;
		
		for (int i = 0; i < n; i++) {
			for (int j = c[i]; j <= x; j++) {
				dp2[j] = (dp2[j] + dp2[j - c[i]]) % MOD;
			}
		}
		
		return dp2[x];
	}
	
	void solve() {
		FAST_IO;
		cin >> n >> x;
		c.resize(n);
		for(int &i: c) cin >> i;
		
		//dp.assign(n+1, vector<ll> (x+1, 0)); // -1 for Memo and 0 for Tabu
		dp2.assign(x + 1, 0);
		
		cout << Tabu_optimized() << endl;
	}
};

class _1637 { // subtruct Digits from num to make zero
	public:
	int n;
	vector<ll> dp;
	
	ll Memo(int x) {
		if(x == 0) return 0;
		
		if(dp[x] != -1) return dp[x];
		
		int tmp = x;
		int r;
		ll ans = 1e9;
		while(tmp) {
			r = tmp % 10;
			tmp /= 10;
			if(r == 0) continue;
			ans = min(ans, Memo(x - r) + 1);
		}
		
		return dp[x] = ans;
	}
	
	ll Tabu() {
		int r, tmp;
		for(int num=1; num<=n; num++) {
			tmp = num;
			while(tmp) {
				r = tmp % 10; tmp /= 10;
				dp[num] = min(dp[num], dp[num - r] + 1);
			}
		}
		
		return dp[n];
	}
	
	void solve() {
		FAST_IO;
		cin >> n;
		dp.assign(n+1, 1e9); // -1 for Memo and 1e9 for Tabu
		dp[0] = 0;
		
		cout << Tabu() << endl;
	}
};

class _1638 { // path count in 2d grid with trap
	public:
	int n;
	vector<string> grid;
	vector<vector<ll>> dp;
	
	ll Memo(int x, int y) {
		if(x == 1 && y == 1 && grid[0][0] != '*') return 1;
		if(x < 1 || y < 1 || y > n || x > n) return 0;
		if(grid[x-1][y-1] == '*') return 0;
		
		if(dp[x][y] != -1) return dp[x][y];
		
		ll ans = 0;
		ans += Memo(x-1, y);
		ans += Memo(x, y-1);
		ans %= MOD;
		
		return dp[x][y] = ans;
	}
	
	ll Tabu() {
		bool b1 = true, b2 = true;
		for(int i=1; i<=n; i++) {
			if(grid[i-1][0] == '*')
				b1 = false;
			if(grid[0][i-1] == '*')
				b2 = false;
			if(b1) dp[i][1] = 1; 
			if(b2) dp[1][i] = 1;
		}
		
		for(int i=2; i<=n; i++) {
			for(int j=2; j<=n; j++) {
				if(grid[i-1][j-1] == '*') continue;
				dp[i][j] = (dp[i-1][j] + dp[i][j-1]) % MOD;
			}
		}
		
		return dp[n][n];
	}
	
	void solve() {
		FAST_IO;
		cin >> n;
		grid.resize(n);
		for(string &s: grid) cin >> s;
		
		dp.assign(n+1, vector<ll>(n+1, 0)); // -1 for Memo and 0 for Tabu
		
		cout << Tabu() << endl;
	}
};

class _1158 { // knapsack 1/0
	public:
	int n, x;
	vector<int> h, s;
	vector<int> dp1, dp2; // for Tabu
	vector<vector<int>> dp; // for Memo
	
	int Memo(int ind, int amt) {
		if(ind == n || amt == 0) return 0;
		
		if(dp[ind][amt] != -1) return dp[ind][amt];
		
		int ans = 0;
		if(amt >= h[ind])
			ans = max(ans, s[ind] + Memo(ind + 1, amt - h[ind]));
		ans = max(ans, Memo(ind + 1, amt));
		
		return dp[ind][amt] = ans;
	}
	
	int Tabu() {
		dp1.assign(x+1, 0);
		dp2.assign(x+1, 0);
		
		for(int i=0; i<n; i++) {
			for(int amt=1; amt<=x; amt++) {
				if(h[i] > amt) continue;
				dp2[amt] = max(dp1[amt], s[i] + dp1[amt - h[i]]);
			}
			
			dp1 = dp2;
		}
		
		//dp[i][amt] = max(dp[i-1][amt], s[i-1] + dp[i-1][amt - h[i-1]]);
		return dp1[x];
	}
	
	void solve() {
		FAST_IO;
		cin >> n >> x;
		h.resize(n); s.resize(n);
		for(int &i: h) cin >> i;
		for(int &i: s) cin >> i;
		
		// dp.assign(n+1, vector<int>(x+1, -1)); // for Memo
		
		cout << Tabu() << endl;
	}
};

class _1746 { // Prefix DP
	public:
	int n, m;
	vector<int> x;
	vector<vector<ll>> dp;
	
	int Tabu() {
		if(x[0] == 0){
			for(int i=1; i<=m; i++) 
				dp[0][i] = 1;
		}
		else {
			dp[0][x[0]] = 1;
		}
		
		for(int i=1; i<n; i++) {
			for(int j=1; j<=m; j++) {
				if(x[i] != 0 && x[i] != j) continue;
				
				dp[i][j] = dp[i-1][j];
				if(j > 1) dp[i][j] = (dp[i][j] + dp[i-1][j-1]) % MOD;
				if (j < m) dp[i][j] = (dp[i][j] + dp[i-1][j+1]) % MOD;
			}
		}
		int ans = 0;
		for (int v = 1; v <= m; v++) {
			ans = (ans + dp[n-1][v]) % MOD;
		}

		return ans;
	}
	
	void solve() {
		FAST_IO;
		cin >> n >> m;
		x.resize(n+1);
		for(int &i: x) cin >> i;
		
		dp.assign(n+1, vector<ll>(m+1, 0));
		
		cout << Tabu() << endl;
	}
};

class _2413 { // Counting Towers 
	public:
	int t, n;
	vector<ll> dp[2]; 
	// State 0 (Joined): The top level is a single block of width 2.
	// State 1 (Separated): The top level consists of two independent blocks, each of width 1.
	
	void Tabu() {
		dp[0][1] = 1;
		dp[1][1] = 1;
		
		for(int i=2; i<=1e6; i++) {
			dp[0][i] = (2 * dp[0][i-1] + dp[1][i-1]) % MOD;
			dp[1][i] = (dp[0][i-1] + 4 * dp[1][i-1]) % MOD;
		}
	}
	
	void solve() {
		FAST_IO;
		cin >> t;
		
		dp[0].resize(1e6 + 1);
		dp[1].resize(1e6 + 1);
		Tabu();
		
		while(t--) {
			cin >> n;
			
			cout << (dp[0][n] + dp[1][n]) % MOD << endl;
		}
	}
};

class _1639 { // Edit distance
	public:
	string s1, s2;
	int n, m;
	vector<int> dp1, dp2;
	
	int Tabu() {
		for (int j = 0; j <= m; j++) dp1[j] = j;
		
		for(int i=1; i<=n; i++) {
			dp2[0] = i;
			for(int j=1; j<=m; j++) {
				if(s1[i-1] == s2[j-1]) 
					dp2[j] = dp1[j-1];
				else {
					dp2[j] = min(dp1[j], min(dp2[j-1], dp1[j-1])) + 1;
				}
			}
			
			dp1 = dp2;
		}
		
		return dp1[m];
	}
	
	void solve() {
		FAST_IO;
		cin >> s1;
		cin >> s2;
		
		n = s1.size(); m = s2.size();
		dp1.assign(m+1, 0);
		dp2.assign(m+1, 0);
		
		cout << Tabu() << endl;
	}
};

class _3403 { // LIS
	public:
	int n, m;
	vector<int> a, b;
	vector<vector<int>> dp;
	vector<vector<int>> path;
	vector<int> pathVector;
	int xdir[3] = {-1, 0, -1};
	int ydir[3] = {-1, -1, 0};
	
	int Memo(int x, int y) {
		if(x <= 0 || y <= 0) return 0;
		
		if(dp[x][y] != -1) return dp[x][y];
		
		int ans = 0;
		if(a[x-1] == b[y-1]) {
			ans = Memo(x-1, y-1) + 1;
			path[x][y] = 0;
		}
		else {
			int a = Memo(x-1, y); 
			ans = Memo(x, y-1);
			if(a > ans) {
				ans = a;
				path[x][y] = 2;
			}
			else path[x][y] = 1;
		}
		
		return dp[x][y] = ans;
	}
	
	int Tabu() {
		for(int i=1; i<=n; i++) {
			for(int j=1; j<=m; j++) {
				if(a[i-1] == b[j-1]) {
					dp[i][j] = dp[i-1][j-1] + 1;
					path[i][j] = 0;
				}
				else {
					if(dp[i-1][j] > dp[i][j-1]) path[i][j] = 2;
					else path[i][j] = 1;
					dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
				}
			}
		}
		
		return dp[n][m];
	}
	
	void print_path(int size) {
		pathVector.resize(size);
		int ind = size - 1;
		int x = n, y = m;
		int tmp;
		while(path[x][y] != -1) {
			tmp = path[x][y];
			if(tmp == 0) pathVector[ind--] = a[x-1];
			x += xdir[tmp];
			y += ydir[tmp];
		}
		
		for(int x: pathVector) cout << x << " ";
		cout << endl;
	}
	
	void solve() {
		FAST_IO;
		cin >> n >> m;
		a.resize(n); b.resize(m);
		for(int &x: a) cin >> x;
		for(int &x: b) cin >> x;
		
		dp.assign(n+1, vector<int>(m+1, 0)); // -1 for Memo and 0 for Tabu
		path.assign(n+1, vector<int>(m+1, -1));
		
		int ans = Tabu();
		cout << ans << endl;
		print_path(ans);
	}
};

class _1744 { // Rectangle Cutting 
	public:
	int n, m;
	vector<vector<int>> dp;
	
	int Memo(int x, int y) {
		if(x == y) return 0;
		if(dp[x][y] != -1) return dp[x][y];
		
		int ans = 1e9;
		for(int i=1; i<x; i++) 
			ans = min(ans, Memo(x - i, y) + Memo(i, y) + 1);
		for(int j=1; j<y; j++) 
			ans = min(ans, Memo(x, y - j) + Memo(x, j) + 1);
		
		return dp[x][y] = ans;
	}
	
	int Tabu() {
		for(int x=1; x<=n; x++) {
			for(int y=1; y<=m; y++) {
				if(x == y) {
					dp[x][y] = 0;
					continue;
				}
				
				for(int k=1; k<x; k++) 
					dp[x][y] = min(dp[x][y], dp[x-k][y] + dp[k][y] + 1);
				for(int k=1; k<y; k++) 
					dp[x][y] = min(dp[x][y], dp[x][y-k] + dp[x][k] + 1);
			}
		}
		
		return dp[n][m];
	}
	
	void solve() {
		FAST_IO;
		cin >> n >> m;
		
		dp.assign(n+1, vector<int>(m+1, 1e9)); // -1 for Memo and 1e9 for Tabu
		
		cout << Tabu() << endl;
	}
};

class _1745 {
	public:
	int n;
	vector<int> coins;
	const int MAX_SUM = 100001;
	bitset<100001> dp;
	
	void solve() {
		FAST_IO;
		cin >> n;
		int coin;
		
		dp[0] = 1;
		
		for (int i = 0; i < n; i++) {
			cin >> coin;
	
			dp |= (dp << coin);
		}
		
		vector<int> result;
		for (int i = 1; i < MAX_SUM; i++) {
			if (dp[i]) {
				result.push_back(i);
			}
		}
		
		cout << result.size() << "\n";
		int size = result.size(); 
		for (int i = 0; i < size; i++) {
			cout << result[i] << (i == size - 1 ? "" : " ");
		}
		cout << endl;
		
	}
};

class _1097 { Interval DP
	public:
	int n;
	vector<ll> x;
	vector<vector<ll>> dp;
	
	// maximize score difference of p1 and p2 (game theory)
	
	// p1 + p2 = sum
	// p1 - p2 = dp[0][n-1]
	// 2 * p1 = sum + dp[0][n-1]
	// p1 = (sum + dp[0][n-1]) / 2
	
	void solve() {
		FAST_IO;
		cin >> n;
		x.resize(n);
		for(ll &i: x) cin >> i;
		
		ll sum = accumulate(x.begin(), x.end(), 0LL);
		dp.assign(n, vector<ll>(n));
		
		for (int length = 1; length <= n; length++) {
			for (int i = 0; i <= n - length; i++) {
				int j = i + length - 1;
				if (i == j) {
					dp[i][j] = x[i];
				} else {
					dp[i][j] = max(x[i] - dp[i+1][j], x[j] - dp[i][j-1]);
				}
			}
		}
		
		cout << (sum + dp[0][n-1]) / 2 << endl;
		
	}
};

class _1093 { // two sets of equal sum
	public:
	int n;
	ll a, b;
	vector<int> dp;
	ll sum;
	
	ll Memo(int ind) {
		if(ind == n + 1) {
			if(a == b) return 1;
			return 0;
		}
		
		ll ans = 0;
		a += ind;
		ans += Memo(ind + 1);
		a -= ind;
		
		b += ind;
		ans += Memo(ind + 1);
		b -= ind;
		
		ans %= MOD;
		
		return ans;
	}
	
	ll Tabu() {
		dp.assign(sum + 1, 0);
		dp[0] = 1;
		
		for(int i=1; i<n; i++) {
			for (int j = sum; j >= i; j--) {
				dp[j] = (dp[j] + dp[j - i]) % MOD;
			}
		}
		
		return dp[sum];
	}
	
	void solve() {
		FAST_IO;
		cin >> n;
		
		sum = (n * (n + 1)) >> 1;
		if(sum & 1) {
			cout << 0 << endl;
			return;
		}
		
		sum = sum >> 1;
	
		// a = 0; b = 0;
		// cout << Memo(1) / 2 << endl;
		
		cout << Tabu() << endl;
	}
};

class _3314 {  // dp + monotonic stack (next larger number)
	public:
	int n;
	vector<int> h;
	vector<int> L, R, indices, dp;
	
	void solve() {
		FAST_IO;
		cin >> n;
		h.resize(n);
		for(int &i: h) cin >> i;
		
		L.assign(n, -1);
		R.assign(n, -1);
		stack<int> st;
		
		for(int i=0; i<n; i++) {
			while(!st.empty() && h[st.top()] <= h[i]) st.pop();
			if(!st.empty()) L[i] = st.top();
			st.push(i);
		}
		
		while(!st.empty()) st.pop();
		
		for (int i = n - 1; i >= 0; i--) {
			while (!st.empty() && h[st.top()] <= h[i]) st.pop();
			if (!st.empty()) R[i] = st.top();
			st.push(i);
		}
		
		/*
		for(int X: L) cout << X << " ";
		cout << endl;
		for(int X: R) cout << X << " ";
		cout << endl;
		*/
		
		indices.resize(n);
		for (int i = 0; i < n; i++) indices[i] = i;
		sort(indices.begin(), indices.end(), [&](int a, int b) {
			return h[a] > h[b];
		});
		
		dp.assign(n, 1);
		int max_mountains = 0;

		for (int i : indices) {
			int best_prev = 0;
			if (L[i] != -1) best_prev = max(best_prev, dp[L[i]]);
			if (R[i] != -1) best_prev = max(best_prev, dp[R[i]]);
			
			dp[i] = 1 + best_prev;
			max_mountains = max(max_mountains, dp[i]);
		}

		cout << max_mountains << endl;
	}
};

class _1145 { // LIS
	public:
	int n;
	vector<int> nums;
	
	int LIS() {  // DP approach
		if (n == 0) return 0;
		
		vector<int> dp(n, 1);
		int maxLength = 1;

		for (int i = 1; i < n; i++) {
			for (int j = 0; j < i; j++) {
				if (nums[j] < nums[i]) {
					dp[i] = max(dp[i], dp[j] + 1);
				}
			}
			maxLength = max(maxLength, dp[i]);
		}
		return maxLength;
	}
	
	int LIS_Optimal() { // Binary search approach
		if (n == 0) return 0;

		vector<int> tails;
		for (int x : nums) {
			auto it = lower_bound(tails.begin(), tails.end(), x);
			
			if (it == tails.end()) 
				tails.push_back(x);
			else 
				*it = x;
		}
		return tails.size();
	}
	
	void solve() {
		FAST_IO;
		cin >> n;
		nums.resize(n);
		
		for(int &i: nums) cin >> i;
		
		cout << LIS_Optimal() << endl;
	}
};

class _1140 { // Weighted Interval Scheduling problem
	public:
	int n;
	vector<vector<ll>> proj;
	
	ll Tabu() { // n^2 -> TLE
		sort(proj.begin(), proj.end(), [](vector<ll> &a, vector<ll> &b) {
			if(a[0] == b[0]) return a[1] < b[1];
			return a[0] < b[0];
		});
		
		vector<ll> dp(n+1, 0);
		
		ll max_ = -1;
		for(int i=0; i<n; i++) {
			dp[i] = proj[i][2];
			for(int j=i-1; j>=0; j--) {
				if(proj[j][1] >= proj[i][0]) continue;

				dp[i] = max(dp[i], dp[j] + proj[i][2]);
			}
			
			max_ = max(max_, dp[i]);
		}
		
		return max_;
	}
	
	ll Tabu_optimal() {
		sort(proj.begin(), proj.end(), [](vector<ll> &a, vector<ll> &b) {
			return a[1] < b[1];
		});
		
		vector<ll> dp(n+1, 0), endTime(n);
		for(int i=0; i<n; i++) endTime[i] = proj[i][1];
		
		for(int i=1; i<=n; i++) {
			auto it = lower_bound(endTime.begin(), endTime.end(), proj[i-1][0]);
			int j = distance(endTime.begin(), it); // 0-index, so no need to j-1
			
			dp[i] = max(dp[i-1], dp[j] + proj[i-1][2]);
			
		}
		
		return dp[n];
	}
	
	void solve() {
		FAST_IO;
		cin >> n;
		proj.assign(n, vector<ll>(3, 0));
		
		for(vector<ll> &v: proj)
			for(ll &i: v) 
				cin >> i;
		
		cout << Tabu_optimal() << endl;
	}
};

class _1653 {
	public:
	int n, x;
	vector<int> w;
	
	int Tabu() {
		// dp[mask] = {min_rides, min_weight_of_last_ride}
		vector<pair<int, int>> dp(1 << n);
		
		dp[0] = {1, 0}; // 1 ride started with 0 weight
		for (int mask = 1; mask < (1 << n); mask++) {
			dp[mask] = {n + 1, 0};

			for (int i = 0; i < n; i++) {
				if (mask & (1 << i)) {
					pair<int, int> prev = dp[mask ^ (1 << i)];
					pair<int, int> current;

					if (prev.second + w[i] <= x)
						current = {prev.first, prev.second + w[i]};
					else
						current = {prev.first + 1, w[i]};
					
					// Keep the lexicographically smallest pair
					if (current < dp[mask])
						dp[mask] = current;
				}
			}
		}
		
		return dp[(1 << n) - 1].first;
	}
	
	void solve() {
		FAST_IO;
		cin >> n >> x;
		w.resize(n);
		for(int &i: w) cin >> i;
		
		cout << Tabu() << endl;
	}
};


/* templete
class _QuesNum {
	public:
	void solve() {
		FAST_IO;
		
	}
};
*/
void solve() {
	
	//auto start = chrono::high_resolution_clock::now();
	
	_3314 a;
	a.solve();
	
	/*
	auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time taken: " << duration.count() << " ms\n";
	*/
}

int main() {
	long long t = 1;
	//cin >> t;
	while(t--) solve();
}
