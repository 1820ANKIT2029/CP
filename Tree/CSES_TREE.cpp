#include <bits/stdc++.h>
using namespace std;

#define FAST_IO ios::sync_with_stdio(false); cin.tie(nullptr)

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, int> pli;
typedef pair<ll, ll> pll;
typedef pair<int, pii> pipii;
typedef vector<ll> vl;
typedef pair<ll, ll> pll;

#define Left(p) ((p)<<1)+1
#define Right(p) ((p)<<1)+2

const ll MOD = 1e9 + 7;

class _1674 { // subtree children cnt
	public:
	int n;
	vector<vector<int>> AL;
	vector<int> dp;
	
	void treeDp(int u) {
		if(AL[u].size() == 0) return;
		for(int v: AL[u]) {
			treeDp(v);
			dp[u] += (dp[v] + 1);
		}
	}
	
	void solve() {
		FAST_IO;
		cin >> n;
		AL.assign(n, vector<int>());
		int a;
		for(int i=1; i<n; i++) {
			cin >> a;
			AL[a-1].push_back(i);
		}
		
		dp.assign(n, 0);
		treeDp(0);
		
		for(int x: dp) cout << x << " ";
		cout << endl;
	}
};

class _1130 { // matching in tree, two states for each node
	public:
	int n;
	vector<vector<int>> AL;
	vector<vector<int>> dp;
	
	void TreeDp(int u, int par) {
		dp[u][0] = dp[u][1] = 0;
		
		for(int v: AL[u]){
			if(v == par) continue;
			
			TreeDp(v, u);
			
			dp[u][1] = max({
				dp[u][0] + dp[v][0] + 1,
				dp[u][1] + dp[v][1],
                dp[u][1] + dp[v][0]
			});
			dp[u][0] += max(dp[v][0], dp[v][1]);
		}
	}
	
	void solve() {
		FAST_IO;
		cin >> n;
		AL.assign(n, vector<int>());
		dp.assign(n, vector<int>(2));
		int a, b;
		for(int i=1; i<n; i++) {
			cin >> a >> b;
			AL[a-1].push_back(b-1);
			AL[b-1].push_back(a-1);
		}
		
		TreeDp(0, -1);
		
		cout << max(dp[0][0], dp[0][1]) << endl;
	}
};

class _1131 { // tree diameter
	public:
	int n;
	vector<vector<int>> AL;
	vector<int> dist;
	int diameter;
	
	void treeDp(int u, int p) {
        int max_dist = 0;
		
		for(int v: AL[u]) {
			if(v == p) continue;
			
			treeDp(v, u);
			
			diameter = max(diameter, max_dist + dist[v] + 1);
			max_dist = max(max_dist, dist[v] + 1);
		}
		
		dist[u] = max_dist;
    }

	
	void solve() {
		FAST_IO;
		cin >> n;
		AL.assign(n, vector<int>());
		dist.assign(n, 0); diameter = 0;
		
		int a, b;
		for(int i=1; i<n; i++) {
			cin >> a >> b;
			AL[a-1].push_back(b-1);
			AL[b-1].push_back(a-1);
		}
		
		treeDp(0, -1);
		
		cout << diameter << endl;
	}
};

class _1132 { // All longest paths: Tree DP + Rerooting technique
	public:
	int n;
	vector<vector<int>> AL;
	vector<int> firstMax, secondMax, c;
	
	void dfs1(int u, int p) {
		firstMax[u] = 0; secondMax[u] = 0;
		
		for(int v: AL[u]) {
			if(v == p) continue;
			
			dfs1(v, u);
			if(firstMax[u] < firstMax[v] + 1) {
				secondMax[u] = firstMax[u];
				firstMax[u] = firstMax[v] + 1;
				c[u] = v;
			}
			else if(secondMax[u] < firstMax[v] + 1) {
				secondMax[u] = firstMax[v] + 1;
			}
		}
	}
	
	void dfs2(int u, int p) {
		for(int v: AL[u]) {
			if(v == p) continue;
			
			if(c[u] == v) {
				if(firstMax[v] < secondMax[u] + 1) {
					secondMax[v] = firstMax[v];
					firstMax[v] = secondMax[u] + 1;
					c[v] = u;
				}
				else {
					secondMax[v] = max(secondMax[v], secondMax[u] + 1);
				}
			}
			else {
				secondMax[v] = firstMax[v];
				firstMax[v] = firstMax[u] + 1;
				c[v] = u;
			}
			dfs2(v, u);
		}
	}
	
	void solve() {
		FAST_IO;
		cin >> n;
		AL.assign(n, vector<int>());
		firstMax.assign(n, 0);
		secondMax.assign(n, 0);
		c.assign(n, 0);
		
		int a, b;
		for(int i=1; i<n; i++) {
			cin >> a >> b;
			AL[a-1].push_back(b-1);
			AL[b-1].push_back(a-1);
		}
		
		dfs1(0, -1);
		dfs2(0, -1);
		
		for(int x: firstMax) cout << x << " ";
		cout << endl;
	}
};

class _1133 { // all path sum: Tree DP + re-rooting technique
	public:
	int n;
	vector<vector<int>> AL;
	vector<long long> size, ans;
	
	void dfs1(int u, int p, long long depth) {
		ans[0] += depth;
		
		for(int v: AL[u]) {
			if(v == p) continue;
			
			dfs1(v, u, depth + 1);
			size[u] += size[v];
		}
	}
	
	void dfs2(int u, int p) { // root shift from u to v
		for(int v: AL[u]) {
			if(v == p) continue;
			
			// get closer to size[v] number of nodes and away from (n - size[v]) nodes
			ans[v] = ans[u] - size[v] + (n - size[v]);
			dfs2(v, u);
		}
	}
	
	void solve() {
		FAST_IO;
		cin >> n;
		AL.assign(n, vector<int>());
		ans.assign(n, 0);
		size.assign(n, 1);
		
		int a, b;
		for(int i=1; i<n; i++) {
			cin >> a >> b;
			AL[a-1].push_back(b-1);
			AL[b-1].push_back(a-1);
		}
		
		dfs1(0, -1, 0);
		dfs2(0, -1);
		
		for(long long x: ans) cout << x << " ";
		cout << endl;
	}
};

class _1678 { // kth ancestor: binary lifting
	public:
	int n, q;
	vector<vector<int>> AL;
	vector<vector<int>> bl; // binary lifting
	
	void dfs(int u, int p) {
		bl[u][0] = p;
		
		for(int i=1; i<20; i++) 
			bl[u][i] = bl[bl[u][i-1]][i-1];
		
		for(int v: AL[u]) {
			if(v == p) continue;
			
			dfs(v, u);
		}
	}
	
	int compute(int x, int k) {
		for(int i=0; i<20; i++) {
			if(k & (1<<i))
				x = bl[x][i];
		}
		
		if(x == 0) return -1;
		return x;
	}
	
	void solve() {
		FAST_IO;
		cin >> n >> q;
		AL.assign(n+1, vector<int>());
		bl.assign(n+1, vector<int>(20, 0));
		
		int a;
		for(int i=2; i<=n; i++) {
			cin >> a;
			AL[i].push_back(a);
			AL[a].push_back(i);
		}
		
		dfs(1, 0);
		
		int x, k;
		while(q--) {
			cin >> x >> k;
			
			cout << compute(x, k) << endl;
		} 
	}
};

class _1688 { // common ancestor: LCA
	public:
	int n, q;
	vector<vector<int>> AL;
	vector<vector<int>> bl;
	vector<int> d;
	
	void dfs(int u, int p, int depth) {
		bl[u][0] = p;
		
		d[u] = depth;
		for(int i=1; i<20; i++) 
			bl[u][i] = bl[bl[u][i-1]][i-1];
		
		for(int v: AL[u]) {
			if(v == p) continue;
			
			dfs(v, u, depth + 1);
		}
	}
	
	int LCA(int a, int b) {
		if(d[b] > d[a])
			swap(a, b);
		
		int up = d[a] - d[b];
		for(int i=0; i<20; i++)
			if(up & (1<<i))
				a = bl[a][i];
		
		if(a == b) return a;
		
		for(int i=19; i>=0; i--) {
			if(bl[a][i] != bl[b][i]) {
				a = bl[a][i];
				b = bl[b][i];
			}
		}
		
		return bl[a][0];
	}
	
	void solve() {
		FAST_IO;
		cin >> n >> q;
		AL.assign(n+1, vector<int>());
		bl.assign(n+1, vector<int>(20, 0));
		d.assign(n+1, 0);
		
		int a, b;
		for(int i=2; i<=n; i++) {
			cin >> a;
			AL[i].push_back(a);
			AL[a].push_back(i);
		}
		
		dfs(1, 0, 0);
		
		while(q--) {
			cin >> a >> b;
			
			cout << LCA(a, b) << endl;
		}
	}
};

class _1135 { // distance b/w two nodes
	public:
	int n, q;
	vector<vector<int>> AL;
	vector<vector<int>> bl;
	vector<int> d;
	
	void dfs(int u, int p, int depth) {
		bl[u][0] = p;
		
		d[u] = depth;
		for(int i=1; i<20; i++) 
			bl[u][i] = bl[bl[u][i-1]][i-1];
		
		for(int v: AL[u]) {
			if(v == p) continue;
			
			dfs(v, u, depth + 1);
		}
	}
	
	int LCA(int a, int b) {
		if(d[b] > d[a])
			swap(a, b);
		
		int up = d[a] - d[b];
		for(int i=0; i<20; i++)
			if(up & (1<<i))
				a = bl[a][i];
		
		if(a == b) return a;
		
		for(int i=19; i>=0; i--) {
			if(bl[a][i] != bl[b][i]) {
				a = bl[a][i];
				b = bl[b][i];
			}
		}
		
		return bl[a][0];
	}
	
	
	void solve() {
		FAST_IO;
		cin >> n >> q;
		AL.assign(n+1, vector<int>());
		bl.assign(n+1, vector<int>(20, 0));
		d.assign(n+1, 0);
		
		int a, b, c;
		for(int i=1; i<n; i++) {
			cin >> a >> b;
			AL[a].push_back(b);
			AL[b].push_back(a);
		}
		
		dfs(1, 0, 0);
		
		while(q--) {
			cin >> a >> b;
			
			c = LCA(a, b);
			cout << d[a] + d[b] - 2 * d[c] << endl;
		}
	}
};

class _1136 { // counting paths: Binary Lefting + LCA + lazy update
	public:
	int n, m;
	vector<vector<int>> AL, bl;
	vector<int> ans, dist;
	
	void dfs1(int u, int p) {
		bl[u][0] = p;
		
		for(int i=1; i<20; i++)
			bl[u][i] = bl[bl[u][i-1]][i-1];
		
		for(int v: AL[u]) {
			if(v == p) continue;
			dist[v] = dist[u] + 1;
			dfs1(v, u);
		}
	}
	
	int lca(int a, int b) {
		if(dist[a] < dist[b]) swap(a, b);
		int up = dist[a] - dist[b];
		for(int i=0; i<20; i++) 
			if(up & (1<<i)) a = bl[a][i];
		
		if(a == b) return a;
		
		for(int i=19; i>=0; i--) 
			if(bl[a][i] != bl[b][i]) {
				a = bl[a][i]; b = bl[b][i];
			}
			
		return bl[a][0];
	}
	
	void dfs2(int u, int p) {
		for(int v: AL[u]) {
			if(v == p) continue;
			
			dfs2(v, u);
			ans[u] += ans[v];
		}
	}
	
	void solve() {
		FAST_IO;
		cin >> n >> m;
		AL.assign(n+1, vector<int>());
		bl.assign(n+1, vector<int>(20, 0));
		ans.assign(n+1, 0);
		dist.assign(n+1, 0);
		
		int a, b;
		for(int i=1; i<n; i++) {
			cin >> a >> b;
			AL[a].push_back(b);
			AL[b].push_back(a);
		}
		
		dist[1] = 1;
		dfs1(1, 0);
		
		while(m--) {
			cin >> a >> b;
			int l = lca(a, b);
			ans[a]++;
			ans[b]++;
			ans[l]--;
			if(l != 1) ans[bl[l][0]]--;
		}
		
		dfs2(1, 0);
		
		for(int i=1; i<=n; i++) cout << ans[i] << " ";
		cout << endl;
	}
};

class segmentTreeRURQLazy {
    /*
        Segment Tree Range Update Range Query Lazy Propagation
    */

    int size;
    vl operations, values;

    ll NEUTRAL_ELEMENT = 0LL;
    ll NO_OPERATION = LLONG_MAX - 1;

    ll modify_op(ll a, ll b){
        if(b == NO_OPERATION) return a;
        return b;
    }

    ll query_op(ll a, ll b){
        return a + b;
    }

    void apply_mod_op(ll &a, ll b){
        a = modify_op(a, b);
    }

    void propagate(int p, int L, int R){
        if(L == R) return;

        int mid = L +  ((R-L)>>1);
        apply_mod_op(operations[Left(p)], operations[p]);
        apply_mod_op(values[Left(p)], operations[p]);
        apply_mod_op(operations[Right(p)], operations[p]);
        apply_mod_op(values[Right(p)], operations[p]);
        operations[p] = NO_OPERATION;
    }

    void modify(int p, int L, int R, int l, int r, ll val){
        propagate(p, L, R);
        if(L > r || R < l) return;
        if(L >= l && R<=r){
            apply_mod_op(operations[p], val);
            apply_mod_op(values[p], val);
            return;
        }

        int mid = L + ((R-L)>>1);
        modify(Left(p), L, mid, l, r, val);
        modify(Right(p), mid+1, R, l, r, val);

        values[p] = query_op(values[Left(p)], values[Right(p)]);
        apply_mod_op(values[p], operations[p]);
    }

    ll query(int p, int L, int R, int l, int r){
        propagate(p, L, R);
        if(L > r || R < l) return NEUTRAL_ELEMENT;
        if(L >= l && R<=r) return values[p];

        int mid = L + ((R-L)>>1);
        ll a = query(Left(p), L, mid, l, r);
        ll b = query(Right(p), mid+1, R, l, r);
        ll ans = query_op(a, b);
        apply_mod_op(ans, operations[p]);
        return ans;
    }

public:
    segmentTreeRURQLazy(int n){
        size = 1;
        while(size < n) size *= 2;
        operations.assign(2 * size, NEUTRAL_ELEMENT);
        values.assign(2 * size, NEUTRAL_ELEMENT);
    }

    void modify(int l, int r, ll v){modify(0, 0, size-1, l, r, v);}
    ll query(int l, int r){return query(0, 0, size-1, l, r);}

};

class _1137 { // euler tour + segment tree
	public:
	int n, q, timer;
	vector<vector<int>> AL;
	vector<int> par;
	vector<long long> v;
	
	vector<int> start, end;
	
	void EulerTour(int u, int p) {
		start[u] = timer++;
		for(int v: AL[u]) {
			if(v == p) continue;
			
			EulerTour(v, u);
		}
		end[u] = timer;
	}
	
	void solve() {
		FAST_IO;
		cin >> n >> q;
		timer = 0;
		AL.assign(n, vector<int>());
		v.assign(n, 0LL);
		
		int a, b;
		for(int i=0; i<n; i++) cin >> v[i];
		for(int i=1; i<n; i++) {
			cin >> a >> b;
			AL[a-1].push_back(b-1);
			AL[b-1].push_back(a-1);
		}
		
		start.resize(n);
		end.resize(n);
		EulerTour(0, -1);

		segmentTreeRURQLazy st(n);
		for(int i=0; i<n; i++) {
			st.modify(start[i], start[i], v[i]);
		}
		
		long long x;
		while(q--) {
			cin >> a >> b;
			if(a == 1) {
				cin >> x;
				st.modify(start[b-1], start[b-1], x);
			}
			else {
				cout << st.query(start[b-1], end[b-1]-1) << endl;
			}
		}
	}
};

class segmentTreeRURQLazy1 { 
    /*
        Segment Tree Range Update Range Query Lazy Propagation
    */

    int size;
    vl operations, values;

    ll NEUTRAL_ELEMENT = 0LL;
    ll NO_OPERATION = 0LL;

    ll modify_op(ll a, ll b){
        return a + b;
    }

    ll query_op(ll a, ll b){
        return a + b;
    }

    void apply_mod_op(ll &a, ll b){
        a = modify_op(a, b);
    }

    void propagate(int p, int L, int R){
        if(L == R) return;

        int mid = L +  ((R-L)>>1);
        apply_mod_op(operations[Left(p)], operations[p]);
        apply_mod_op(values[Left(p)], operations[p]);
        apply_mod_op(operations[Right(p)], operations[p]);
        apply_mod_op(values[Right(p)], operations[p]);
        operations[p] = NO_OPERATION;
    }

    void modify(int p, int L, int R, int l, int r, ll val){
        propagate(p, L, R);
        if(L > r || R < l) return;
        if(L >= l && R<=r){
            apply_mod_op(operations[p], val);
            apply_mod_op(values[p], val);
            return;
        }

        int mid = L + ((R-L)>>1);
        modify(Left(p), L, mid, l, r, val);
        modify(Right(p), mid+1, R, l, r, val);

        values[p] = query_op(values[Left(p)], values[Right(p)]);
        apply_mod_op(values[p], operations[p]);
    }

    ll query(int p, int L, int R, int l, int r){
        propagate(p, L, R);
        if(L > r || R < l) return NEUTRAL_ELEMENT;
        if(L >= l && R<=r) return values[p];

        int mid = L + ((R-L)>>1);
        ll a = query(Left(p), L, mid, l, r);
        ll b = query(Right(p), mid+1, R, l, r);
        ll ans = query_op(a, b);
        apply_mod_op(ans, operations[p]);
        return ans;
    }

public:
    segmentTreeRURQLazy1(int n){
        size = 1;
        while(size < n) size *= 2;
        operations.assign(2 * size, NEUTRAL_ELEMENT);
        values.assign(2 * size, NEUTRAL_ELEMENT);
    }

    void modify(int l, int r, ll v){modify(0, 0, size-1, l, r, v);}
    ll query(int l, int r){return query(0, 0, size-1, l, r);}

};

class _1138 { // euler tour + segment tree
	public:
	int n, q, timer;
	vector<vector<int>> AL;
	vector<long long> v;
	vector<int> start, end;
	
	void EulerTour(int u, int p) {
		start[u] = timer++;
		for(int v: AL[u]) {
			if(v == p) continue;
			
			EulerTour(v, u);
		}
		end[u] = timer;
	}
	
	void solve() {
		FAST_IO;
		cin >> n >> q;
		timer = 0;
		AL.assign(n, vector<int>());
		v.assign(n, 0LL);
		
		int a, b;
		for(int i=0; i<n; i++) cin >> v[i];
		for(int i=1; i<n; i++) {
			cin >> a >> b;
			AL[a-1].push_back(b-1);
			AL[b-1].push_back(a-1);
		}
		
		start.resize(n);
		end.resize(n);
		EulerTour(0, -1);
		
		segmentTreeRURQLazy1 st(n);
		for(int i=0; i<n; i++) {
			st.modify(start[i], end[i]-1, v[i]);
		}
		
		long long x;
		while(q--) {
			cin >> a >> b;
			if(a == 1) {
				cin >> x;
				st.modify(start[b-1], end[b-1]-1, x - v[b-1]);
				v[b-1] = x;
			}
			else {
				cout << st.query(start[b-1], start[b-1]) << endl;
			}
		}
		
	}
};

/*
class segmentTreeMax {
	int n, size;
	vector<int> maxArr;
	
	void build(int p, int L, int R, vector<int> &a) {
		if(L == R) {
			if(L < n) maxArr[p] = a[L];
            return;
		}
		
		int mid = L + ((R - L) >> 1);
		build(Left(p), L, mid, a);
		build(Right(p), mid+1, R, a);
		
		maxArr[p] = max(maxArr[Left(p)], maxArr[Right(p)]);
	}
	
	void pointUpdate(int p, int L, int R, int idx, int v) {
		if(L == R) {maxArr[p] = v; return;}
		
		int mid = L + ((R-L)>>1);
		if(idx <= mid) pointUpdate(Left(p), L, mid, idx, v);
		else pointUpdate(Right(p), mid+1, R, idx, v);
		
		maxArr[p] = max(maxArr[Left(p)], maxArr[Right(p)]);
	}
	
	int rangeQuery(int p, int L, int R, int l, int r) {
		if(L >= l && R<=r) return maxArr[p];
        if(R < l || L > r) return INT_MIN;

        int mid = L + ((R-L)>>1);

        return max(rangeQuery(Left(p), L, mid, l, r), rangeQuery(Right(p), mid+1, R, l, r));
	}
	
public:
	segmentTreeMax() {}
	segmentTreeMax(vector<int> &a) {
		n = a.size();
		size = 1;
		while(size < n) size *= 2;
		
		maxArr.assign(2*size, INT_MIN);
		build(0, 0, size-1, a);
	}
	
	void pointUpdate(int idx, int v) {
		pointUpdate(0, 0, size-1, idx, v);
	}
	
	int rangeQuery(int l, int r) {
		return rangeQuery(0, 0, size-1, l, r);
	}
};
*/ // TLE

class segmentTreeMax {
	int n;
	vector<int> tree;
	
public:
	segmentTreeMax() {}
	segmentTreeMax(vector<int> &a) {
		n = a.size();
		tree.assign(2 * n, INT_MIN);
        
		for (int i = 0; i < n; i++) 
            tree[n + i] = a[i];
            
		for (int i = n - 1; i > 0; --i) 
            tree[i] = max(tree[i << 1], tree[i << 1 | 1]);
	}
	
	void pointUpdate(int idx, int v) {
		for (tree[idx += n] = v; idx > 1; idx >>= 1)
			tree[idx >> 1] = max(tree[idx], tree[idx ^ 1]);
	}
	
	int rangeQuery(int l, int r) {
		int res = INT_MIN;
		for (l += n, r += n + 1; l < r; l >>= 1, r >>= 1) {
			if (l & 1) res = max(res, tree[l++]);
			if (r & 1) res = max(res, tree[--r]);
		}
		return res;
	}
};

class HLD {
	int timer;
	vector<int> baseArray, parent, subtree, heavy, depth, head, pos;
	segmentTreeMax seg;
	
	void dfs(int u, int p, vector<vector<int>> &AL) {
		parent[u] = p;
		subtree[u] = 1;
		int max_size = 0;
		heavy[u] = -1;
		
		for(int v: AL[u]) {
			if(v == p) continue;
			
			depth[v] = depth[u] + 1;
			dfs(v, u, AL);
			subtree[u] += subtree[v];
			
			if(subtree[v] > max_size) {
				max_size = subtree[v];
				heavy[u] = v;
			}
		}
	}
	
	// labeler
	void decompose(int u, int h, vector<int> &values, vector<vector<int>> &AL) {
		head[u] = h;
		pos[u] = timer++;
		baseArray[pos[u]] = values[u];
		
		if(heavy[u] != -1) decompose(heavy[u], h, values, AL);
		
		for(int v: AL[u]) {
			if(v == parent[u] || v == heavy[u]) continue;
			decompose(v, v, values, AL);
		}
	}
	
public: 
	HLD(int n, vector<vector<int>> &AL, vector<int> &values) {
		timer = 0;
		baseArray.assign(n, 0);
		parent.assign(n+1, 0);
		subtree.resize(n+1);
		heavy.resize(n+1);
		depth.resize(n+1);
		head.resize(n+1);
		pos.resize(n+1);
		depth[1] = 0;
		
		dfs(1, 0, AL);
		decompose(1, 1, values, AL);
		
		seg = segmentTreeMax(baseArray);
	}
	
	void updateNode(int u, int val) {
		seg.pointUpdate(pos[u], val);
	}
	
	int queryPath(int a, int b) {
		int ans = INT_MIN;
		
		while(head[a] != head[b]) {
			if(depth[head[a]] < depth[head[b]]) swap(a, b);
			
			ans = max(ans, seg.rangeQuery(pos[head[a]], pos[a]));
			a = parent[head[a]];
		}
		
		if(depth[a] > depth[b]) swap(a, b);
		ans = max(ans, seg.rangeQuery(pos[a], pos[b]));
		return ans;
	}
};

class _2134 {
	public:
	int n, q;
	vector<vector<int>> AL;
	vector<int> v;
	
	void solve() {
		FAST_IO;
		cin >> n >> q;
		AL.assign(n+1, vector<int>());
		v.resize(n+1);

		v[0] = 0;
		for(int i=1; i<=n; i++) cin >> v[i];
		
		int a, b, c;
		for(int i=1; i<n; i++) {
			cin >> a >> b;
			AL[a].push_back(b);
			AL[b].push_back(a);
		}
		
		HLD hld(n, AL, v);
		
		while(q--) {
			cin >> a >> b >> c;
			if(a == 1) {
				hld.updateNode(b, c);
			}
			else {
				cout << hld.queryPath(b, c) << " ";
			}
		}
		cout << endl;
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
	
	_2134 a;
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
