#include <bits/stdc++.h>
using namespace std;

const int xdir[] = {1, 0, -1, 0};
const int ydir[] = {0, 1, 0, -1};
const char dir[] = {'D', 'R', 'U', 'L'};

typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, int> pli;
typedef pair<ll, ll> pll;
typedef pair<int, pii> pipii;

class _1192 { // #forests, #connectedComponents
	public:
	int n, m;
	vector<string> grid;
	
	bool isGood(int x, int y) {
		if(x<0 || x>=n) return false;
		if(y<0 || y>=m) return false;
		return true;
	}
	
	void dfs(int x, int y) {
		grid[x][y] = '#';
		int _x, _y;
		for(int i=0; i<4; i++) {
			_x = x + xdir[i];
			_y = y + ydir[i];
			if(isGood(_x, _y) && grid[_x][_y] == '.') dfs(_x, _y);
		}
	}
	
	void solve() {
		cin >> n >> m;
		
		grid.assign(n, "");
		string s;
		for(int i=0; i<n; i++) {cin >> s; grid[i] = s;}
		
		int cnt = 0;
		for(int i=0; i<n; i++) {
			for(int j=0; j<m; j++) {
				if(grid[i][j] == '.') {
					cnt++;
					dfs(i, j);
				}
			}
		}
		
		cout << cnt << endl;
	}
};

class _1193 { // can use BFS or Dijsktra or A*
	public:
	vector<string> grid;
	vector<char> path;
	int n, m;
	pii start, end;
	
	bool isGood(int x, int y) {
		if(x<0 || x>=n) return false;
		if(y<0 || y>=m) return false;
		return true;
	}
	
	void getPath() {
		int x = end.first;
		int y = end.second;
		
		int tmp;
		while(grid[x][y] != 'A') {
			tmp = grid[x][y];
			path.emplace_back(dir[tmp]);
			tmp = (tmp + 2) % 4;
			x += xdir[tmp];
			y += ydir[tmp];
		}
		
		reverse(path.begin(), path.end());
	}
	
	bool BFS() {
		queue<pii> q;
		q.push(start);
		
		int _x, _y;
		while(!q.empty()) {
			pii &tmp = q.front(); q.pop();
			if(tmp == end) return true;
			
			for(int i=0; i<4; i++) {
				_x = tmp.first + xdir[i];
				_y = tmp.second + ydir[i];
				if(isGood(_x, _y) && grid[_x][_y] == '.') {
					q.emplace(_x, _y);
					grid[_x][_y] = i;
				}
			}
		}
		return false;
	}
	
	
	bool Dijsktra() {
		priority_queue<pipii, vector<pipii>, greater<pipii>> pq;
		pq.emplace(0, start);
		
		int _x, _y;
		while(!pq.empty()) {
			auto [dist, tmp] = pq.top(); pq.pop();
			if(tmp == end) return true;
			
			for(int i=0; i<4; i++) {
				_x = tmp.first + xdir[i];
				_y = tmp.second + ydir[i];
				if(isGood(_x, _y) && grid[_x][_y] == '.') {
					pq.emplace(dist + 1, pii(_x, _y));
					grid[_x][_y] = i;
				}
			}
		}
		
		return false;
	}
	
	int h_func(int x, int y) {
		return abs(end.first - x) + abs(end.second - y);
	}
	
	bool A_star() {
		vector<vector<int>> d(n, vector<int>(m, 1e9));
		priority_queue<pipii, vector<pipii>, greater<pipii>> pq;
		pq.emplace(0 + h_func(start.first, start.second), start);
		d[start.first][start.second] = 0;
		
		int _x, _y;
		while(!pq.empty()) {
			auto [dist, tmp] = pq.top(); pq.pop();
			
			if (dist > d[tmp.first][tmp.second] + h_func(tmp.first, tmp.second)) continue;
			if(tmp == end) return true;
			
			for(int i=0; i<4; i++) {
				_x = tmp.first + xdir[i];
				_y = tmp.second + ydir[i];
				
				int new_dist = d[tmp.first][tmp.second] + 1 ;
				if(isGood(_x, _y) && grid[_x][_y] == '.' && new_dist < d[_x][_y]) {
					d[_x][_y] = new_dist;
					pq.emplace(new_dist + h_func(_x, _y), pii(_x, _y));
					grid[_x][_y] = i;
				}
			}
		}
		
		return false;
	}
	
	void solve() {
		cin >> n >> m;
		grid.assign(n, "");
		for(int i=0; i<n; i++) cin >> grid[i];
		
		for(int i=0; i<n; i++)
			for(int j=0; j<m; j++) {
				if(grid[i][j] == 'A') 
					start = {i, j};
				if(grid[i][j] == 'B')
					end = {i, j};
			}
				
		grid[end.first][end.second] = '.';
		//cout << end.first << " " << end.second << endl;
		// algo
		bool exist = A_star();
		// path
		if(exist) {
			cout << "YES\n";
			getPath();
			//for(string &s: grid) cout << s << endl;
			cout << path.size() << endl;
			for(char c: path) cout << c;
			cout << endl;
		}
		else cout << "NO\n";
	}
};

class _1666 { // connected components
	public:
	int n, m;
	vector<vector<int>> AL;
	vector<bool> c;
	
	void dfs(int u) {
		c[u] = true;
		
		for(int v: AL[u]) {
			if(c[v]) continue;
			dfs(v);
		}
	}
	
	void solve() {
		cin >> n >> m;
		AL.assign(n, vector<int>());
		int n1, n2;
		for(int i=0; i<m; i++) {
			cin >> n1 >> n2;
			AL[n1 - 1].emplace_back(n2 - 1);
			AL[n2 - 1].emplace_back(n1 - 1);
		}
		
		
		vector<int> nodes;
		c.assign(n, false);
		for(int i=0; i<n; i++) {
			if(c[i]) continue;
			
			dfs(i);
			nodes.emplace_back(i);
		}
		
		int ans = nodes.size() - 1;
		cout << ans << endl;
		if(ans == 0) return;
		for(int i=ans; i>0; i--) {
			cout << nodes[0] + 1 << " " << nodes[i] + 1 << endl;
		}
		
	}
};

class _1667 { // Shortest path in unweighted graph (BFS) + path print
	public:
	int n, m;
	vector<vector<int>> AL;
	vector<int> p;
	vector<int> path;
	
	bool bfs(int u, int v){
		queue<int> q;
		q.push(u);
		p[u] = u;
		
		while(!q.empty()) {
			int u_ = q.front(); q.pop();
			if(u_ == v) return true;
			for(int v_: AL[u_]) {
				if(p[v_] != -1) continue;
				q.push(v_);
				p[v_] = u_;
			}
		}
		
		return false;
	}
	
	void get_path(int u, int g) {
		while(g != u) {
			path.push_back(g+1);
			g = p[g];
		}
		path.push_back(g+1);
		reverse(path.begin(), path.end());
	}
	
	void solve() {
		cin >> n >> m;
		AL.assign(n, vector<int>());
		int u, v;
		for(int i=0; i<m; i++) {
			cin >> u >> v;
			AL[u-1].push_back(v-1);
			AL[v-1].push_back(u-1);
		}
		
		p.assign(n, -1);
		bool exist = bfs(0, n-1);
		if(exist) {
			get_path(0, n-1);
			cout << path.size() << endl;
			for(int p: path) cout << p << " ";
			cout << endl;
		}
		else cout << "IMPOSSIBLE" << endl;
	}
};

class _1668 { // Bipartite Check (BFS or DFS)
	public:
	int n, m;
	vector<vector<int>> AL;
	vector<char> c;
	bool bfs(int u) {
		queue<int> q;
		q.push(u);
		c[u] = 1;
		
		while(!q.empty()) {
			u = q.front(); q.pop();
			for(int v: AL[u]){
				if(c[v] != -1 && c[v] == c[u]) 
					return false;
				if(c[v] == -1) {
					q.push(v);
					c[v] = 1 - c[u];
				}
			}
		}
		return true;
	}
	void solve() {
		cin >> n >> m;
		AL.assign(n, vector<int>());
		int u, v;
		for(int i=0; i<m; i++) {
			cin >> u >> v;
			AL[u-1].push_back(v-1);
			AL[v-1].push_back(u-1);
		}
		c.assign(n, -1);
		
		bool exist = true;
		for(int i=0; i<n; i++) {
			if(c[i] != -1) continue;
			exist &= bfs(i);
			if(!exist) break;
		}
		
		if(exist) {
			for(int x: c) cout << x + 1 << " ";
			cout << endl;
		}
		else cout << "IMPOSSIBLE" << endl;
	}
};

class _1669 { // back edge (dfs)
	public:
	int n, m;
	vector<vector<int>> AL;
	vector<int> p, path;
	
	pii dfs(int u) {
		pii tmp = {-1, -1};
		for(int v: AL[u]) {
			if(p[u] == v) continue;
			if(p[v] != -1) {
				return pii(v, u);
			}
			
			p[v] = u;
			tmp = dfs(v);
			if(tmp.first != -1 && tmp.second != -1) break;
		} 
		return tmp;
	}
	
	void solve() {
		cin >> n >> m;
		AL.assign(n, vector<int>());
		int u, v;
		for(int i=0; i<m; i++) {
			cin >> u >> v;
			AL[u-1].push_back(v-1);
			AL[v-1].push_back(u-1);
		}
		
		p.assign(n, -1);
		pii tmp;
		for(int i=0; i<n; i++) {
			if(p[i] != -1) continue;
			p[i] = i;
			tmp = dfs(i);
			if(tmp.first != -1 && tmp.second != -1) break;
		}
		
		u = tmp.first;
		v = tmp.second;
		
		if(u == -1 && v == -1) {
			cout << "IMPOSSIBLE" << endl;
			return;
		}
		path.push_back(u+1);
		while(v != u) {
			path.push_back(v+1);
			v = p[v];
		}
		path.push_back(u+1);
		reverse(path.begin(), path.end());
		
		cout << path.size() << endl;
		for(int x: path) cout << x << " ";
		cout << endl;
	}
};

class _1194 { // Multi-source BFS
	public:
	int n, m;
	vector<string> grid;
	vector<char> path;
	vector<vector<char>> c;
	vector<vector<bool>> manOrmonster;
	
	bool isGood(int x, int y) {
		if(x<0 || x>=n) return false;
		if(y<0 || y>=m) return false;
		return true;
	}
	
	bool isSolution(int x, int y) {
		if(x==0 || x==n-1) return true;
		if(y==0 || y==m-1) return true;
		return false;
	}
	
	void get_path(int x, int y){
		int ind;
		while(c[x][y] != 5) {
			ind = c[x][y];
			path.push_back(dir[ind]);
			ind = (ind + 2) % 4;
			x += xdir[ind];
			y += ydir[ind];
		}
		reverse(path.begin(), path.end());
	}
	
	void solve() {
		cin >> n >> m;
		grid.assign(n, "");
		for(int i=0; i<n; i++) cin >> grid[i];
		
		queue<pair<char, pii>> q;
		manOrmonster.assign(n, vector<bool>(m, true));
		c.assign(n, vector<char>(m, -1));
		pii ans;
		for(int i=0; i<n; i++)
			for(int j=0; j<m; j++){
				switch (grid[i][j]){
					case 'A': 
						ans = pii(i, j);
						c[i][j] = 5;
						break;
					case 'M': 
						q.emplace('M', pii(i, j));
						manOrmonster[i][j] = false;
						c[i][j] = 6;
						break;
				}
			}
		q.emplace('A', ans);
		
		bool exist = false;
		int x, y;
		while(!q.empty()) {
			auto [character, tmp] = q.front(); q.pop();
			if(character == 'A' && isSolution(tmp.first, tmp.second)) {
				ans = tmp;
				exist = true;
				break;
			}
			
			for(int i=0; i<4; i++) {
				x = tmp.first + xdir[i]; 
				y = tmp.second + ydir[i];
				
				if(isGood(x, y) && grid[x][y] != '#' && c[x][y] == -1){
					if(character == 'A' && !manOrmonster[x][y]) continue;
					if(character == 'M') {
						manOrmonster[x][y] = false;
					}
					q.emplace(character, pii(x, y));
					c[x][y] = i;
				}
			}
		}
		
		if(!exist) {
			cout << "NO" << endl;
			return;
		}
		
		/*
		for(vector<char> &v: c){
			for(int x: v) {
				cout << to_string(x) << " ";
			}
			cout << endl;
		}
		*/
		
		cout << "YES" << endl;
		get_path(ans.first, ans.second);
		cout << path.size() << endl;
		for(char c: path) {
			cout << c;
		}
		cout << endl;
	}
};

class _1671 { // Dijkstra (SSSP)
	public:
	int n, m;
	vector<vector<pli>> AL;
	vector<ll> d;
	
	void dijkstra(int start) {
		priority_queue<pli, vector<pli>, greater<pli>> pq;
		pq.emplace(0, start);
		d[start] = 0;
		
		while(!pq.empty()) {
			auto [dist, u] = pq.top(); pq.pop();
			
			if(d[u] < dist) continue;
			
			for(auto [w, v]: AL[u]) {
				ll d_ = dist + w;
				if(d_ < d[v]) {
					pq.emplace(d_, v);
					d[v] = d_;
				}
			}
		}
	}
	
	void solve() {
		cin >> n >> m;
		AL.assign(n, vector<pli>());
		int a, b;
		ll c;
		for(int i=0; i<m; i++) {
			cin >> a >> b >> c;
			AL[a-1].emplace_back(c, b-1);
		}
		
		d.assign(n, LLONG_MAX);
		dijkstra(0);
		for(ll x: d) cout << x << " ";
		cout << endl;
	}
};

class _1672 { // Floyd-Warshall Algorithm (APSP)
	public:
	void solve() {
		int n, m, q;
		cin >> n >> m >> q;
		vector<vector<ll>> AM(n, vector<ll>(n, 1e15));
		int a, b;
		ll c;
		for(int i=0; i<m; i++) {
			cin >> a >> b >> c;
			AM[a-1][b-1] = min(AM[a-1][b-1], c);
			AM[b-1][a-1] = min(AM[b-1][a-1], c);
		}
		for(int i=0; i<n; i++) AM[i][i] = 0;
		
		for(int k=0; k<n; k++)
			for(int i=0; i<n; i++)
				for(int j=0; j<n; j++)
					AM[i][j] = min(AM[i][j], AM[i][k] + AM[k][j]);
				
		for(int i=0; i<q; i++) {
			cin >> a >> b;
			if(AM[a-1][b-1] == 1e15) cout << -1 << endl;
			else cout << AM[a-1][b-1] << endl;
		}
	}
};

class _1673 { // Bellman-Ford Algorithm (SSSP with negative weight) + dfs
	public:
	int n, m;
	vector<vector<pli>> AL;
	vector<ll> d;
	vector<bool> visited;
	
	bool dfs(int u, int g) {
		if(u == g) return true;
		visited[u] = true;
		
		bool ans = false;
		for(auto &[w, v]: AL[u]) {
			if(visited[v]) continue;
			ans = dfs(v, g);
			if(ans) break;
		}
		
		return ans;
	}
	
	void solve() { 
		cin >> n >> m;
		AL.assign(n, vector<pli>());
		int a, b;
		ll c;
		for(int i=0; i<m; i++) {
			cin >> a >> b >> c;
			AL[a-1].emplace_back(c, b-1);
		}
		
		d.assign(n, -1e15);
		d[0] = 0;
		for(int i=0; i<n-1; i++) {
			bool ismodified = false;
			for(int u=0; u<n; u++) {
				if(d[u] != -1e15) {
					for(auto [w, v]: AL[u]){
						if(d[u] + w <= d[v]) continue;
						
						d[v] = d[u] + w;
						ismodified = true;
					}
				}
			}
			
			if(!ismodified) break;
		}
		
		bool positiveCycle = false;
		
		//if(d[0] != 0) positiveCycle = true;
		
		for(int u=0; u<n; u++) {
			if(positiveCycle) break;
			
			if(d[u] != -1e15){
				for(auto &[w, v]: AL[u]){
					
					if(d[v] < d[u] + w){
						visited.assign(n, false);
						bool x = dfs(0, u);
						visited.assign(n, false);
						bool y = dfs(u, n-1);
						if(x && y)
							positiveCycle = true;
					}
				}
			}
		}	
						
		if(positiveCycle) cout << -1 << endl;
		else cout << d[n-1] << endl;
	}
	
};

class _1195 { // two Dijkstra used: one normal and second with condition
	public:
	int n, m;
	vector<vector<pli>> AL;
	vector<ll> d, max_;
	
	void solve() {
		cin >> n >> m;
		AL.assign(n, vector<pli>());
		int a, b;
		ll c;
		for(int i=0; i<m; i++) {
			cin >> a >> b >> c;
			AL[a-1].emplace_back(c, b-1);
		}
		
		d.assign(n, 1e15); max_.assign(n, 1e15);
		priority_queue<pli, vector<pli>, greater<pli>> pq;
		pq.emplace(0, 0);
		d[0] = 0; max_[0] = 0;
		while(!pq.empty()) {
			auto [dist, u] = pq.top(); pq.pop();
			
			if(dist > d[u]) continue;
			
			for(auto &[w, v]: AL[u]) {
				if(d[u] + w < d[v]) {
					d[v] = d[u] + w;
					pq.emplace(d[v], v);
				}
			}
		}
		pq = priority_queue<pli, vector<pli>, greater<pli>>();
		pq.emplace(0, 0);
		while(!pq.empty()) {
			auto [dist, u] = pq.top(); pq.pop();
			
			if(dist > max_[u]) continue;
			
			for(auto &[w, v]: AL[u]) {
				if((max_[u] + w) < max_[v] || (d[u] + w / 2) < max_[v]) {
					max_[v] = min(max_[u] + w, d[u] + w / 2);
					pq.emplace(max_[v], v);
				}
			}
		}
		//cout << d[n-1] << " " << max_[n-1] << endl;
		cout << max_[n-1] << endl;
	}
};

class _1197 { // Bellman-Ford Algorithm (SSSP with negative weight)
	public:
	int n, m;
	vector<vector<pli>> AL;
	vector<ll> d;
	vector<int> p, path;
	unordered_set<int> mp;
	
	void get_path(int u) {
		path.push_back(u); mp.insert(u);
		u = p[u];
		while(mp.count(u) == 0){
			path.push_back(u); mp.insert(u);
			u = p[u];
		}
		path.push_back(u);
		reverse(path.begin(), path.end());
		while(path.back() != path[0]) path.pop_back();
	}
	
	void components(int u) { 
		p[u] = 0;
		for(auto &[w, v]: AL[u]) {
			if(p[v] == -1) components(v);
		}
	}
	
	void solve() {
		cin >> n >> m;
		AL.assign(n, vector<pli>());
		int a, b;
		ll c;
		for(int i=0; i<m; i++) {
			cin >> a >> b >> c;
			AL[a-1].emplace_back(c, b-1);
		}
		
		if(n == 1 && m == 1) {
			if(AL[0][0].first >= 0) cout << "NO" << endl;
			else {
				cout << "YES" << endl;
				cout << "1 1" << endl;
			}
			return;
		}
		
		d.assign(n, 1e15); p.assign(n, -1);
		
		for(int u=0; u<n; u++)
			if(p[u] == -1) {
				d[u] = 0;
				components(u);
			}
			
		p.assign(n, -1);
		for(int i=0; i<n-1; i++) {
			bool ismodified = false;
			for(int u=0; u<n; u++) 
				if(d[u] != 1e15) 
					for(auto [w, v]: AL[u]){
						if(d[u] + w >= d[v]) continue;
						
						d[v] = d[u] + w;
						p[v] = u;
						ismodified = true;
					}
			
			if(!ismodified) break;
		}
		
		bool negativeCycle = false;
		
		for(int u=0; u<n; u++) {
			if(negativeCycle) break;
			
			if(d[u] != 1e15){
				for(auto &[w, v]: AL[u]){
					if(d[v] > d[u] + w){
						negativeCycle = true;
						get_path(u);
						break;
					}
				}
			}
		}	
						
		if(negativeCycle) {
			cout << "YES" << endl;
			for(int x: path) cout << x + 1 << " ";
			cout << endl;
		}
		else cout << "NO" << endl;
	}
};

class _1196_ { // k shortest path: modified Dijkstra with multiset for each node to track k shortest path  
	public:
	int n, m, k;
	vector<vector<pli>> AL;
	vector<multiset<ll>> d;

	bool canProcess(int w, int u) {
		if(d[u].size() < k) return true;
		if(*d[u].rbegin() >= w) return true;
		return false;
	}
	
	void insertDist(ll w, int u) {
		if(d[u].size() < k) d[u].insert(w);
		else {
			d[u].erase(--d[u].end());
			d[u].insert(w);
		}
	}
	
	void solve() {
		cin >> n >> m >> k;
		AL.assign(n, vector<pli>());
		int a, b;
		ll c;
		for(int i=0; i<m; i++) {
			cin >> a >> b >> c;  
			AL[a-1].emplace_back(c, b-1);
		}
		
		priority_queue<pli, vector<pli>, greater<pli>> pq;
		d.assign(n, multiset<ll>{1000000000000000LL});
		
		pq.emplace(0, 0);
		d[0].insert(0);
		while(!pq.empty()) {
			auto [w, u] = pq.top(); pq.pop();
			
			if(!canProcess(w, u)) continue;
			
			for(auto &[w_, v]: AL[u]) {
				
				if(w + w_ < *d[v].rbegin()) {
					insertDist(w + w_, v);
					pq.emplace(w + w_, v);
				}
			}
		}
		
		for(ll x: d[n-1]) cout << x << " ";
		cout << endl;
	}
};

class _1196 {
	public:
	int n, m, k;
	vector<vector<pli>> AL;
	vector<priority_queue<ll>> d;

	bool canProcess(int w, int u) {
		if(d[u].size() < k) return true;
		if(d[u].top() >= w) return true;
		return false;
	}
	
	void insertDist(ll w, int u) {
		if(d[u].size() < k) d[u].push(w);
		else {
			if(d[u].top() <= w) return;
			d[u].pop();
			d[u].push(w);
		}
	}
	
	void solve() {
		cin >> n >> m >> k;
		AL.assign(n, vector<pli>());
		int a, b;
		ll c;
		for(int i=0; i<m; i++) {
			cin >> a >> b >> c;  
			AL[a-1].emplace_back(c, b-1);
		}
		
		priority_queue<pli, vector<pli>, greater<pli>> pq;
		d.assign(n, priority_queue<ll>());
		for(int i=0; i<n; i++) d[i].push(1e15);
		
		pq.emplace(0, 0);
		insertDist(0, 0);
		ll dist;
		while(!pq.empty()) {
			auto [w, u] = pq.top(); pq.pop();
			
			if(!canProcess(w, u)) continue;
			
			for(auto &[w_, v]: AL[u]) {
				dist = w + w_;
				if(dist < d[v].top()) {
					insertDist(dist, v);
					pq.emplace(dist, v);
				}
			}
		}
		
		vector<ll> ans(k);
		int ind = k - 1;
		while(!d[n-1].empty()) {
			ans[ind--] = d[n-1].top();
			d[n-1].pop();
		}
		for(ll x: ans) cout << x << " ";
		cout << endl;
	}
};

class _1678 { 
	public:
	int n, m;
	vector<vector<int>> AL;
	vector<int> p, path;
	vector<char> c; // 0 -> unvisited, 1 -> explored, 2 -> visited
	
	pii dfs(int u) {	
		c[u] = 1;
		
		pii tmp = {-1, -1};
		for(int v: AL[u]) {
			if(c[v] == 0) {
				p[v] = u;
				tmp = dfs(v);
			}
			else if (c[v] == 1) {
				tmp = pii(v, u);
			}
			if(tmp.first != -1 && tmp.second != -1) break;
		} 
		
		c[u] = 2;
		return tmp;
	}
	
	void solve() {
		cin >> n >> m;
		AL.assign(n, vector<int>());
		int u, v;
		for(int i=0; i<m; i++) {
			cin >> u >> v;
			AL[u-1].push_back(v-1);
		}
		
		p.assign(n, -1);
		c.assign(n, 0);	
		pii tmp;
		for(int i=0; i<n; i++) {
			if(c[i] != 0) continue;
			tmp = dfs(i);
			if(tmp.first != -1 && tmp.second != -1) break;
		}
		
		u = tmp.first;
		v = tmp.second;
		
		if(u == -1 && v == -1) {
			cout << "IMPOSSIBLE" << endl;
			return;
		}
		
		path.push_back(u+1);
		while(v != u) {
			path.push_back(v+1);
			v = p[v];
		}
		path.push_back(u+1);
		reverse(path.begin(), path.end());
		
		cout << path.size() << endl;
		for(int x: path) cout << x << " ";
		cout << endl;
	}
};

class _1679 { // topological sort
	public:
	int n, m;
	vector<vector<int>> AL;
	vector<char> c;
	vector<int> path;
	int ind;
	
	bool topologicalsort(int u) {
		c[u] = 1;
		bool possible = true;
		
		for(int v: AL[u]){
			if(c[v] == 0) possible = topologicalsort(v);
			else if (c[v] == 1) possible = false;
			
			if(!possible) break;
		}
		c[u] = 2;
		path[ind++] = u;
		return possible;
	}
	
	void solve() {
		cin >> n >> m;
		AL.assign(n, vector<int>());
		int u, v;
		for(int i=0; i<m; i++) {
			cin >> u >> v;
			AL[u-1].push_back(v-1);
		}
		
		c.assign(n, 0);
		path.assign(n, -1); ind = 0;
		bool possible = true;
		for(int u=0; u<n; u++) {
			if(c[u] != 0) continue;
			possible = topologicalsort(u);
			if(!possible) break;
		}
		
		if(!possible) {
			cout << "IMPOSSIBLE" << endl;
			return;
		}
		
		reverse(path.begin(), path.end());
		for(int x: path) cout << x + 1 << " ";
		cout << endl;
	}
};

class _1680 { // dynamic programming in a directed acyclic graph
	public:
	int n, m;
	vector<vector<int>> AL;
	vector<int> d, p;
	bool exist;
	
	int dfs(int u) {
		if(u == n-1) {exist = true; return 1;}
		if(d[u] != -1) return d[u];
		
		int max_ = 0, node = -1;
		for(int v: AL[u]) {
			int tmp = dfs(v);
			if(tmp == 0) continue;
			tmp++;
			if(tmp > max_) {
				node = v;
				max_ = tmp;
			}
		}
		p[u] = node;
		return d[u] = max_;
	}
	
	void solve() {
		cin >> n >> m;
		AL.assign(n, vector<int>());
		int a, b;
		for(int i=0; i<m; i++) {
			cin >> a >> b;
			AL[a-1].push_back(b-1);
		}
		
		d.assign(n, -1); p.assign(n, -1);
		exist = false;
		dfs(0);
		
		if(!exist) {
			cout << "IMPOSSIBLE" << endl;
			return;
		}
	
		cout << d[0] << endl;
		a = 0;
		while(a != n-1) {
			cout << a + 1 << " ";
			a = p[a];
		}
		cout << n << " ";
		cout << endl;
	}
};

class _1681 { // count number of path (DAG) with dp
	public:
	const ll MOD = 1e9 + 7;
	int n, m;
	vector<vector<int>> AL;
	vector<ll> dp;
	
	ll dfs(int u) {
		if(u >= n) return 0;
		if(dp[u] != -1) return dp[u];
		
		ll ans = 0;
		for(int v: AL[u]) {
			ans += dfs(v);
			ans %= MOD;
		}
		
		return dp[u] = ans;
	}
	
	void solve() {
		cin >> n >> m;
		AL.assign(n, vector<int>());
		int a, b;
		for(int i=0; i<m; i++) {
			cin >> a >> b;
			AL[a-1].push_back(b-1);
		}
		
		dp.assign(n, -1);
		dp[n-1] = 1;
		dfs(0);
		
		cout << dp[0] << endl;
	}
};

class _1202 { // Dijkstra with Dp
	public:
	const ll MOD = 1e9 + 7;
	int n, m;
	vector<vector<pli>> AL;
	vector<ll> dist, dp, max_, min_;
	
	void dijkstra(int x) {
		dist.assign(n, 1e15); dp.assign(n, 0); min_.assign(n, 0); max_.assign(n, 0);
		
		priority_queue<pli, vector<pli>, greater<pli>> pq;
		pq.emplace(0, x);
		dist[x] = 0; dp[x] = 1;
		ll tmp;
		while(!pq.empty()) {
			auto [w, u] = pq.top(); pq.pop();
			
			if(w > dist[u]) continue;
			
			for(auto &[w_, v]: AL[u]) {
				tmp = w + w_;
				if(tmp == dist[v]) {
					dp[v] = (dp[v] + dp[u]) % MOD;
					min_[v] = min(min_[v], min_[u] + 1);
					max_[v] = max(max_[v], max_[u] + 1);
					continue;
				}
				if(tmp < dist[v]) {
					dist[v] = tmp;
					pq.emplace(dist[v], v);
					dp[v] = dp[u];
					min_[v] = min_[u] + 1;
					max_[v] = max_[u] + 1;
				}
			}
		}
	}
	
	void solve() {
		cin >> n >> m;
		AL.assign(n, vector<pli>());
		int a, b;
		ll c;
		for(int i=0; i<m; i++) {
			cin >> a >> b >> c;
			AL[a-1].emplace_back(c, b-1);
		}
		
		dijkstra(0);
		
		cout << dist[n-1] << " " << dp[n-1] << " " << min_[n-1] << " " << max_[n-1] << endl;
		
	}
};

class _1750 { // Kth ancestor (binary lifting)
	public:
	int n, q;
	vector<int> p;
	vector<vector<int>> dp;
	
	void solve() {
		ios_base::sync_with_stdio(false);
		cin.tie(NULL);
		cin >> n >> q;
		dp.assign(30, vector<int>(n+1, -1));
		int tmp;
		for(int i=1; i<=n; i++) { 
			cin >> dp[0][i];
		}
		for(int i=1; i<30; i++) {
			for(int j=1; j<=n; j++) {
				dp[i][j] = dp[i-1][dp[i-1][j]];
			}
		}
		
		int a, b;
		for(int i=0; i<q; i++) {
			cin >> a >> b;
			
			for(int j=0; j<30; j++) {
				if(b & (1<<j)) a = dp[j][a];
			}
			
			cout << a << '\n';
		}
	}
};

/* templete
class _QuesNum {
	public:
	void solve() {
		
	}
};
*/
void solve() {
	_1671 a;
	a.solve();
}

int main() {
	long long t = 1;
	//cin >> t;
	while(t--) solve();
}
