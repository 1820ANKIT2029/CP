#include <bits/stdc++.h>
using namespace std;

/*
    Binary Lifting: Jump in powers of two
    LCA (Lowest Common Ancestor): Given u and v → find lowest common parent.
        Fast Methods
            - Binary lifting
            - Euler tour + RMQ
            - HLD
*/

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 200005;
const int LOG = 20;

vector<int> adj[MAXN];
int up[MAXN][LOG];   // up[v][j] = 2^j-th ancestor of v
int depth[MAXN];
int n;

// DFS to preprocess
void dfs(int v, int parent) {
    up[v][0] = parent;

    for (int j = 1; j < LOG; j++) {
        up[v][j] = up[up[v][j-1]][j-1]; // Binary Lifting
    }

    for (int to : adj[v]) {
        if (to == parent) continue;
        depth[to] = depth[v] + 1;
        dfs(to, v);
    }
}

// Lift node v by k steps
int lift(int v, int k) {
    for (int j = 0; j < LOG; j++) {
        if (k & (1 << j)) {
            v = up[v][j];
        }
    }
    return v;
}

// LCA query
int lca(int a, int b) {
    if (depth[a] < depth[b])
        swap(a, b);

    // bring to same depth
    a = lift(a, depth[a] - depth[b]);

    if (a == b) return a;

    // jump both
    for (int j = LOG - 1; j >= 0; j--) {
        if (up[a][j] != up[b][j]) {
            a = up[a][j];
            b = up[b][j];
        }
    }

    // parent is LCA
    return up[a][0];
}

int dist(int u, int v) {
    int w = lca(u, v);
    return depth[u] + depth[v] - 2 * depth[w];
}

int kth_ancestor(int v, int k) {
    return lift(v, k);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;

    for (int i = 0; i < n-1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    depth[1] = 0;
    dfs(1, 1);  // root = 1

    int q;
    cin >> q;
    while (q--) {
        int u, v;
        cin >> u >> v;
        cout << lca(u, v) << "\n";
    }
}

/*
    Heavy Light Decomposition (HLD): 
    - (question like: Sum on path(u, v), Max on path(u, v), Update node/edge, 
        Query path frequently)
    - Break tree into heavy chains so any path crosses only log(N) chains.

    Concept:
    - Heavy Child: A heavy child of a node is the child with the largest subtree size rooted at the child.
    - Light Child: A light child of a node is any child that is not the heavy child.
    - Heavy Edge: A heavy edge connects a node to its heavy child.
    - Light Edge: A light edge connects a node to any of its light children.
    - Heavy Path: A maximal path consisting only of heavy edges.

    Time Complexity: O(log^2(n)) per query

    reading: https://codeforces.com/blog/entry/81317
*/

const int MAXN = 200005;

vector<int> adj[MAXN];

int parent[MAXN], depth[MAXN], heavy[MAXN];
int head[MAXN], pos[MAXN], subtree[MAXN];
int timer;
int n;

vector<int> baseArray; // flattened tree
SegmentTree seg;       // assume implemented

// ---------- DFS: find heavy child ----------
int dfs(int u, int p) {
    parent[u] = p;
    subtree[u] = 1;
    int max_size = 0;
    heavy[u] = -1;

    for (int v : adj[u]) {
        if (v == p) continue;

        depth[v] = depth[u] + 1;
        int child_size = dfs(v, u);
        subtree[u] += child_size;

        if (child_size > max_size) {
            max_size = child_size;
            heavy[u] = v;
        }
    }
    return subtree[u];
}

// ---------- Decompose ----------
void decompose(int u, int h, vector<int>& values) {
    head[u] = h;
    pos[u] = timer++;
    baseArray[pos[u]] = values[u];

    // heavy edge first
    if (heavy[u] != -1)
        decompose(heavy[u], h, values);

    // light edges
    for (int v : adj[u]) {
        if (v == parent[u] || v == heavy[u]) continue;
        decompose(v, v, values);
    }
}

// ---------- Build HLD ----------
void build_hld(int root, vector<int>& values) {
    timer = 0;
    baseArray.assign(n, 0);
    depth[root] = 0;
    
    dfs(root, 0);
    decompose(root, root, values);

    seg = SegmentTree(n);
    seg.build(baseArray);
}

// ---------- Query path ----------
int query_path(int a, int b) {
    int res = 0;

    while (head[a] != head[b]) {
        if (depth[head[a]] < depth[head[b]])
            swap(a, b);

        res += seg.query(pos[head[a]], pos[a]);
        a = parent[head[a]];
    }

    if (depth[a] > depth[b])
        swap(a, b);

    res += seg.query(pos[a], pos[b]);
    return res;
}

// ---------- Point update ----------
void update_node(int v, int val) {
    seg.update(pos[v], val);
}

// -------------- Main ---------------
int main() {
    cin >> n;

    vector<int> values(n + 1);
    for (int i = 1; i <= n; i++)
        cin >> values[i];

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    build_hld(1, values);

    // example
    cout << query_path(4, 7) << "\n";
}

/*
==================== DSU ON TREE (SACK TECHNIQUE) ====================

Goal:
For each node u, compute some function over its subtree efficiently
(e.g., number of distinct colors, frequency counts, etc.)

Core Idea (WHY IT WORKS):
- Naive: recompute subtree data for every node → O(N^2)
- Optimization: reuse computation using "small-to-large merging"

Key Observations:
1. Each node has one "heavy child" = child with largest subtree
2. Always KEEP data of heavy child
3. MERGE smaller (light) subtrees into heavy one
4. If we discard light subtree data after use, each node is processed ~logN times

Time Complexity:
O(N log N)

When to Use:
- Subtree queries
- Frequency/counting problems
- Static trees (no updates)

=====================================================================
*/

const int MAXN = 200005;

vector<int> adj[MAXN];
int color[MAXN];

int subtree[MAXN];    // subtree size
int heavy[MAXN];      // heavy child
int ans[MAXN];        // answer for each node

unordered_map<int, int> cnt;      // frequency of colors
int distinct = 0;                 // current distinct count
int n;

// ---------- DFS to find heavy child ----------
void dfs(int u, int p) {
    subtree[u] = 1;
    heavy[u] = -1;
    int max_size = 0;

    for (int v : adj[u]) {
        if (v == p) continue;

        dfs(v, u);
        subtree[u] += subtree[v];

        if (subtree[v] > max_size) {
            max_size = subtree[v];
            heavy[u] = v;
        }
    }
}

// ---------- Add/remove subtree ----------
void add_subtree(int u, int p, int val) {
    cnt[color[u]] += val;

    if (cnt[color[u]] == 1 && val == 1) distinct++;
    if (cnt[color[u]] == 0 && val == -1) distinct--;

    for (int v : adj[u]) {
        if (v == p) continue;
        add_subtree(v, u, val);
    }
}

// ---------- DSU on Tree ----------
void dfs_sack(int u, int p, bool keep) {
    // 1. process light children
    for (int v : adj[u]) {
        if (v == p || v == heavy[u]) continue;
        dfs_sack(v, u, false);
    }

    // 2. process heavy child
    if (heavy[u] != -1)
        dfs_sack(heavy[v], v, true);

    // 3. merge light children into heavy data
    for (int v : adj[u]) {
        if (v == p || v == heavy[u]) continue;
        add_subtree(v, u, +1);
    }

    // 4. add current node
    cnt[color[u]]++;
    if (cnt[color[u]] == 1) distinct++;

    // 5. answer for this node
    ans[u] = distinct;

    // 6. cleanup if not keeping
    if (!keep) {
        add_subtree(u, p, -1);
        distinct = 0;
    }
}

// ---------- MAIN ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;

    for (int i = 1; i <= n; i++)
        cin >> color[i];

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    dfs(1, 0);
    dfs_sack(1, 0, true);

    for (int i = 1; i <= n; i++)
        cout << ans[i] << " ";
}

/*
    Tree Rerooting DP

    Time Complexity: O(n)
*/

const int MAXN = 200005;

vector<int> adj[MAXN];
int subSize[MAXN];
long long downDist[MAXN]; // sum inside subtree
long long ans[MAXN];      // final answer for each node
int n;

// ---------- First DFS ----------
void dfs1(int v, int p) {
    subSize[v] = 1;
    downDist[v] = 0;

    for (int to : adj[v]) {
        if (to == p) continue;

        dfs1(to, v);

        subSize[v] += subSize[to];
        downDist[v] += downDist[to] + subSize[to];
    }
}

// ---------- Reroot DFS ----------
void dfs2(int v, int p) {
    for (int to : adj[v]) {
        if (to == p) continue;

        // move root v -> to
        ans[to] = ans[v]
                  - subSize[to]           // nodes getting closer
                  + (n - subSize[to]);    // nodes getting farther

        dfs2(to, v);
    }
}

// ---------- MAIN ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;

    for (int i = 0; i < n-1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // root at 1
    dfs1(1, 0);

    ans[1] = downDist[1];

    dfs2(1, 0);

    for (int i = 1; i <= n; i++)
        cout << ans[i] << " ";
}

/*
    Centroid Decomposition

    Centroid: A node whose removal splits the tree into components each of size ≤ N/2.
    Height of centroid tree = O(log N)

    It recursively:
        1. finds centroid
        2. marks it as removed
        3. decomposes remaining components
    This builds a centroid tree.
*/

const int MAXN = 200005;

vector<int> adj[MAXN];
bool removed[MAXN];
int subtree[MAXN];
int parentCentroid[MAXN];

int n;

// ---------- compute subtree sizes ----------
int dfs_size(int v, int p) {
    subtree[v] = 1;

    for (int to : adj[v]) {
        if (to == p || removed[to]) continue;
        subtree[v] += dfs_size(to, v);
    }

    return subtree[v];
}

// ---------- find centroid ----------
int dfs_centroid(int v, int p, int sz) {
    for (int to : adj[v]) {
        if (to == p || removed[to]) continue;

        if (subtree[to] > sz / 2)
            return dfs_centroid(to, v, sz);
    }

    return v;
}

// ---------- build centroid tree ----------
void build_centroid(int v, int p) {
    int sz = dfs_size(v, -1);
    int centroid = dfs_centroid(v, -1, sz);

    parentCentroid[centroid] = p;
    removed[centroid] = true;

    for (int to : adj[centroid]) {
        if (!removed[to]) {
            build_centroid(to, centroid);
        }
    }
}

// ---------- MAIN ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;

    for (int i = 0; i < n-1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    build_centroid(1, 0);

    // parentCentroid[] now stores centroid tree
}
