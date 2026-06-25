#include <bits/stdc++.h>
using namespace std;

#define INF 1e9 + 2

enum {UNVISITED = -1, EXPLORED = -2, VISITED = -3};

vector<vector<pair<int, int>>> AL; // Adjecent List

int main() {
	cout << "Graph algo" << endl;
}

// DFS
vector<char> dfs_c;

void DFS(int u) {
	dfs_c[u] = VISITED;
	for(auto &[v, w]: AL[u]) {
		if(dfs_c[v] == UNVISITED) DFS(v);
	}
}

// BFS
void BFS() {
	// inside main function
	vector<int> d(V, INF);  // V: number of nodes, INF: 1e9 + 7;
	queue<int> q;
	q.push(s);  // s: starting node
	while(!q.empty()) {
		int u = q.front(); q.pop();
		
		for(auto &[v, w]: AL[u]) {
			if(d[v] != INF) continue;
			d[v] = d[u] + 1;
			q.push(v);
		} 
	}
}

// modify DFS and this function to get nodes in one forest
int connected_components() {
	int numCC = 0;
	for(int i=0; i<V; i++) {  // V: number of nodes
		if(dfs_c[i] == UNVISITED) {
			DFS(i);
			numCC++;
		}
	}
	
	return numCC;
}

// flood fill
int dr[] = { 1, 1, 0,-1,-1,-1, 0, 1};  // the order is:
int dc[] = { 0, 1, 1, 1, 0,-1,-1,-1};  // S/SE/E/NE/N/NW/W/SW

int floodfill(int r, int c, char c1, char c2) { // returns the size of CC
	if ((r < 0) || (r >= R)) return 0;          // outside grid
	if ((c < 0) || (c >= C)) return 0;          // outside grid
	if (grid[r][c] != c1) return 0;             // does not have color c1
	int ans = 1;
	
	grid[r][c] = c2;                            // to avoid cycling
	for (int d = 0; d < 8; ++d)
		ans += floodfill(r+dr[d], c+dc[d], c1, c2);
	
	return ans;
}

// Topological sort

// dfs version
vector<int> ts;

void toposort(int u) {
	dfs_c[u] = VISITED;
	for(auto &[v, w]: AL[u]){
		if(dfs_c == UNVISITED) toposort(v);
	}
	
	ts.push_back(u);
}

void TS() {
	// inside main function
	for(int i=0; i<V; i++) {
		if(dfs_c[i] == UNVISITED) toposort(i);
	}
	
	reverse(ts.begin(), ts.end());
}

// Kahn's Algorithms (toposort)
void kahn() {
	int V = AL.size();
	vector<int> in_deg(V);
	for(int u=0; u<V; u++) {
		for(auto &[v, w]: AL[u]) in_deg[v]++;
	}
	
	// insert only 0 in-degree node
	priority_queue<int, vector<int>, greater<int>> pq;    // min heap
	for(int u=0; u<V; u++)	
		if(in_deg[u] == 0)
			pq.push(u);
		
	while(!pq.empty()) {
		int u = pq.top(); pq.pop();
		for(auto &[v, w]: AL[u]) {
			in_deg[v]--; 
			if(in_deg[v] == 0) pq.push(v); // insert only 0 in-degree node
		}
	}
}


// Bipartite Graph Check
bool isBipartite() {
	int s = 0;
	queue<int> q; q.push(s);
	vector<int> color(n, INF); 
	color[s]  = 0;
	bool isBipartiteG = true;
	
	while(!q.empty()) {
		int u = q.front(); q.pop();
		
		for(auto &[v, w]: AL[u]) {
			if(color[v] == INF) {
				color[v] = 1 - color[u];
				q.push(v);
			}
			else if (color[v] == color[u]) {
				isBipartiteG = false;
				break;
			}
		}
	}
	
	return isBipartiteG;
}

// cycle detection in directed graph
// cycle detectin in indirected graph, there is no need for visited, explored, unvisited state, just use (0/1 or black/white) state
// change cycleCheck function according to need

vector<int> dfs_parent;

void cycleCheck(int u) {
	dfs_num[u] = EXPLORED;
	for (auto &[v, w] : AL[u]) {
		// case 1: EXPLORED->UNVISITED
		if (dfs_num[v] == UNVISITED) {
			printf("Tree Edge\n");
			dfs_parent[v] = u;                     // a tree edge u->v
			cycleCheck(v);
		}
		// case 2: EXPLORED->EXPLORED (cycle)
		else if (dfs_num[v] == EXPLORED) {
			if (v == dfs_parent[u])
				printf("Bidirectional Edge\n");   // a trivial cycle
			else
				printf("Back Edge (Cycle)\n");    // a non trivial cycle
		}
		// case 3: EXPLORED->VISITED
		else if (dfs_num[v] == VISITED)
			printf("Forward/Cross Edge\n");       // rare application
	}
	dfs_num[u] = VISITED;
}

void cycle() {
	dfs_num.assign(V, UNVISITED);   // V: numbe of nodes
	dfs_parent.assign(V, -1);
	for (int u = 0; u < V; ++u)
		if (dfs_num[u] == UNVISITED)
			cycleCheck(u);
}


/* 
	Finding Articulation Points and Bridges (Undirected Graph)
	1. naive app. :- remove one nodes (or edge) and check if components increased or not
	
	2. two more DFS attributes: 
		a. Discovery Time (Disc[u]): 
			The time (step number) when the DFS first visits vertex u.
		b. Lowest Back Edge Time (Low[u]): 
			The minimum discovery time reachable from u (including u itself)
			by following zero or more tree edges and at most one back edge.
		
		Identifying Bridges: 
			An edge (u, v) in the DFS tree is a bridge if and only if Low[v] > Disc[u].
		
		Identifying Points:
			2 case
*/

vector<int> Disc, Low, parent, articulation_vertex;
int dfsNumberCounter, dfsRoot, rootChildren;

void articulationPointAndBridge(int u) {
	Disc[u] = dfsNumberCounter++;
	Low[u] = Disc[u];
	// Low[u] <= Disc[u]
	for (auto &[v, w] : AL[u]) {
		if (Disc[v] == UNVISITED) {                      // a tree edge
			parent[v] = u;
			if (u == dfsRoot) ++rootChildren;            // special case, root
			 
			articulationPointAndBridge(v);
			
			if (Low[v] >= Disc[u])                // for articulation point
				articulation_vertex[u] = 1;
				
			if (Low[v] > Disc[u])                 // for bridge
				printf(" (%d, %d) is a bridge\n", u, v);
				
			Low[u] = min(Low[u], Low[v]);    // subtree, always update
		}
		else if (v != parent[u]) {                   // if a non-trivial cycle
			Low[u] = min(Low[u], Disc[v]);
		}
	}
}

int runarticulationPointAndBridge(){
	// inside int main()
	// UNVISITED = -1
	Disc.assign(V, UNVISITED); Low.assign(V, 0);
	parent.assign(V, -1); articulation_vertex.assign(V, 0);
	dfsNumberCounter = 0;
	
	printf("Bridges:\n");
	for (int u = 0; u < V; ++u) {
		if (Disc[u] == UNVISITED) {
			dfsRoot = u; rootChildren = 0;
			articulationPointAndBridge(u);
			articulation_vertex[dfsRoot] = (rootChildren > 1); // special case
		}
	}
		
	printf("Articulation Points:\n");
	for (int u = 0; u < V; ++u)
		if (articulation_vertex[u])
			printf(" Vertex %d\n", u);
}


// Strongly Connected Components (Directed Graph)

// Kosaraju's algorithm
// maintain two Graph: G (AL) and G_T (AL_T)

void Kosaraju(int u, int pass) { // pass = 1 (original), 2 (transpose)
	dfs_num[u] = 1;
	vii &neighbor = (pass == 1) ? AL[u] : AL_T[u]; // by ref to avoid copying
	for (auto &[v, w] : neighbor)
		if (dfs_num[v] == UNVISITED)
			Kosaraju(v, pass);
	S.push_back(u);                                // similar to toposort
}

void runKosaraju() {
	// inside int main()
	S.clear();                                     // first pass
	dfs_num.assign(N, UNVISITED);                  // record the post-order
	for (int u = 0; u < N; ++u)                    // of the original graph
		if (dfs_num[u] == UNVISITED)
			Kosaraju(u, 1);
		
	numSCC = 0;                                    // second pass
	dfs_num.assign(N, UNVISITED);                  // explore the SCCs
	for (int i = N-1; i >= 0; --i)                 // based on the
		if (dfs_num[S[i]] == UNVISITED)            // first pass result
			++numSCC, Kosaraju(S[i], 2);           // on transposed graph
			
	printf("There are %d SCCs\n", numSCC);
}

// Tarjan's Algorithm

int dfsNumberCounter, numSCC;                // global variables
vi dfs_num, dfs_low, visited;
stack<int> St;

void tarjanSCC(int u) {
	dfs_low[u] = dfs_num[u] = dfsNumberCounter;      // dfs_low[u]<=dfs_num[u]
	dfsNumberCounter++;                              // increase counter
	St.push(u);                                      // remember the order
	visited[u] = 1;
	for (auto &[v, w] : AL[u]) {
		if (dfs_num[v] == UNVISITED)
		tarjanSCC(v);
		if (visited[v])                              // condition for update
		dfs_low[u] = min(dfs_low[u], dfs_low[v]);
	}
	
	if (dfs_low[u] == dfs_num[u]) {                  // a root/start of an SCC
		++numSCC;                                    // when recursion unwinds
		while (1) {
			int v = St.top(); St.pop();
			visited[v] = 0;
			if (u == v) break;
		}
	}
}

void runtarjanSCC(){
	// inside int main()
	dfs_num.assign(V, UNVISITED); dfs_low.assign(V, 0); visited.assign(V, 0);
	while (!St.empty()) St.pop();
	dfsNumberCounter = numSCC = 0;
	for (int u = 0; u < V; ++u)
		if (dfs_num[u] == UNVISITED)
			tarjanSCC(u);
}


/*
	Multi-source BFS
	Time Complexity: O(V + E)
	Space Complexity: O(V)
*/

vector<int> multiSourceBFS(int V, vector<vector<int>>& AL, vector<int>& sources) {
    vector<int> dist(V, -1);
    queue<int> q;

    // Push all sources and mark distance as 0
    for (int src : sources) {
        q.push(src); dist[src] = 0;
    }
    while (!q.empty()) {
        int u = q.front(); q.pop();

        for (int v : AL[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }

    return dist;
}

/*
	0-1 BFS
	use: Graphs where transitions cost either nothing (e.g., moving freely within a component) 
		or a fixed unit (e.g., breaking a wall, changing a direction)

	Time Complexity: O(V + E)
	Space Complexity: O(V)
*/

vector<int> zeroOneBFS(int V, int start, vector<vector<pair<int, int>>>& AL) {
    vector<int> dist(V, INT_MAX);
    deque<int> dq;

    dq.push_front(start);
    dist[start] = 0;

    while (!dq.empty()) {
        int u = dq.front(); dq.pop_front();

        for (auto& [v, w] : AL[u]) {
            // Relaxation step
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                
                // 0-weight edges to the front, 1-weight edges to the back
                if (w == 0) dq.push_front(v);
                else dq.push_back(v);
            }
        }
    }

    return dist;
}

/*
	Bidirectional BFS
	use: Point-to-point shortest path queries in huge search spaces
		(e.g., Rubik's cube states, social network connections like "degrees of separation")
	
	Time Complexity: O(b^(d/2))
	Space Complexity: O(b^(d/2))
*/
vector<vector<int>>& AL;

// function to process one layer of the BFS
bool processLevel(queue<int>& q, vector<int>& my_visited, vector<int>& other_visited) {
    int size = q.size();
    while (size--) {
        int u = q.front(); q.pop();

        for (int v : AL[u]) {
            if (!my_visited[v]) {
                my_visited[v] = my_visited[u] + 1;
                q.push(v);
            }
            
            if (other_visited[v]) return true; 
        }
    }
    return false;
}

int bidirectionalBFS(int V, int start, int target) {
    if (start == target) return 0;

    vector<int> dist_start(V, 0), dist_target(V, 0);
    queue<int> q_start, q_target;

    q_start.push(start);
    dist_start[start] = 1; 

    q_target.push(target);
    dist_target[target] = 1;

    int total_distance = 0;
    while (!q_start.empty() && !q_target.empty()) {
        total_distance++;
        if (processLevel(q_start, dist_start, dist_target))
            return total_distance;
        
        total_distance++;
        if (processLevel(q_target, dist_target, dist_start))
            return total_distance;
    }

    return -1;
}