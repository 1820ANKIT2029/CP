/* Made by AGENTVEER */
/* JUNE 2024 */

/* standard library */
#include <bits/stdc++.h>
using namespace std;

/* Policy based data structure external library (pbds) */
#include <bits/extc++.h>
using namespace __gnu_pbds;

/* Bits manipulation */
/*
1. To multiply/divide an integer by 2
   S = S<<n; == S*(2**n)
   S = S>>n; == S/(2**n)

2. To set/turn on the j-th item (0-based indexing)
   S |= (1<<j);

3. To check if the j-th item of the set is on
   T = S & (1<<j); // if T=0 => the j-th item of the set is off, otherwise on;

4. To clear/turn off the j-th item of the set
   S &= ~(1<<j);

5. To toggle (flip the status of) the j-th item of the set
   S ^= (1<<j);

6. To get the value of the least significant bit of S that is on (first from the right)
   T = ((S) & -(S)) // LSOne(S)
   Notice that T = LSOne(S) is a power of 2, i.e., 2j.
   To get the actual index j (from the right), we can use __builtin_ctz(T) below

7. To turn on all bits in a set of size n
   S = (1<<n) - 1

8. To enumerate all proper subsets of a given a bitmask, e.g., if mask = (18)10 = (10010)2,
   then its proper subsets are {(18)10 = (10010)2, (16)10 = (10000)2, (2)10 = (00010)2},

   int mask = 18;
   for (int subset = mask; subset; subset = (mask & (subset-1)))
   cout << subset << "\n";

9. built-in bit manipulation functions
   __builtin_popcount(S) : to count how many bits that are on in S.
   __builtin_ctz(S) : to count how many trailing zeroes in S.
*/

/*
Priority Queue

    <-----Max heap------>
    -> priority_queue<int> maxHeap; // Default is max heap

    <-----Min heap------>
    -> priority_queue<int, std::vector<int>, std::greater<int>> minHeap;


    <-----Custom Comparator in Heap------->
    struct Compare {
        bool operator()(std::pair<int, int> a, std::pair<int, int> b) {
            return a.first > b.first; // Min heap based on first element
        }
    };

    int main() {
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, Compare> pq;

        pq.push({3, 100});
        pq.push({1, 200});
        pq.push({2, 300});

        std::cout << "Min Heap of Pairs: ";
        while (!pq.empty()) {
            std::cout << "(" << pq.top().first << ", " << pq.top().second << ") ";
            pq.pop();
        }

        return 0;
    }

    output: Min Heap of Pairs: (1, 200) (2, 300) (3, 100)

    #include <iostream>
    #include <queue>
    #include <vector>

    // Define a Student struct with a custom comparator
    struct Student {
        int score;
        std::string name;

        // Custom comparator: higher scores have higher priority
        bool operator<(const Student& other) const {
            return score < other.score; // Max heap based on score
        }
    };

    int main() {
        std::priority_queue<Student> maxHeap;

        maxHeap.push({95, "Alice"});
        maxHeap.push({85, "Bob"});
        maxHeap.push({90, "Charlie"});
        maxHeap.push({80, "David"});

        std::cout << "Max Heap (Student Scores using Struct):\n";
        while (!maxHeap.empty()) {
            Student top = maxHeap.top();
            std::cout << "Score: " << top.score << ", Student: " << top.name << "\n";
            maxHeap.pop();
        }

        return 0;
    }

    output: Max Heap (Student Scores using Struct):
            Score: 95, Student: Alice
            Score: 90, Student: Charlie
            Score: 85, Student: Bob
            Score: 80, Student: David



    <--------Alternative: Using make_heap with vector-------->
    #include <iostream>
    #include <vector>
    #include <algorithm>

    int main() {
        std::vector<int> heap = {10, 30, 20, 5};

        std::make_heap(heap.begin(), heap.end()); // Default is max heap

        std::cout << "Max Heap top: " << heap.front() << std::endl;

        std::pop_heap(heap.begin(), heap.end());
        heap.pop_back(); // Remove max element

        std::cout << "New Max Heap top: " << heap.front() << std::endl;

        return 0;
    }

    output: Max Heap top: 30
            New Max Heap top: 20

*/

/*
MIN-MAX PQ queue
1. Using bBST for Priority Queue ADT:
    > A balanced BST (e.g., C++ STL set/multiset) can efficiently implement a Priority Queue ADT.
    > Insertion (insert(key)) takes O(logn) time.
    > Finding the smallest key (highest priority) is achieved with begin(), and the largest key can be found using rbegin().

2. Dynamic Min-Max Priority Queue:

    > Since both the minimum and maximum elements can be easily accessed, a bBST effectively functions as a dynamic Min-Max Priority Queue,
      making it more powerful than a standard priority queue.

3. Efficient Implementation of Additional Operations:
    > The UpdateKey(oldkey, newkey) operation is handled by first removing oldkey and then inserting newkey, taking 
      O(2×logn), which simplifies to O(logn).
    > The RemoveKey(key) operation is simply remove(key), which also runs in O(logn).

*/

#include <bits/stdc++.h>
using namespace std;

template<typename T>
class MinMaxPQ_BST {
    multiset<T> s;   // balanced BST allowing duplicates

public:
    MinMaxPQ_BST() = default;

    // Insert a key: O(log n)
    void insert(const T &key) {
        s.insert(key);
    }

    // Return true if empty
    bool empty() const {
        return s.empty();
    }

    // Get min (highest priority) - throws if empty
    const T& get_min() const {
        if (s.empty()) throw runtime_error("get_min on empty PQ");
        return *s.begin();
    }

    // Get max - throws if empty
    const T& get_max() const {
        if (s.empty()) throw runtime_error("get_max on empty PQ");
        return *s.rbegin();
    }

    // Remove and return min - O(log n)
    T pop_min() {
        if (s.empty()) throw runtime_error("pop_min on empty PQ");
        auto it = s.begin();
        T val = *it;
        s.erase(it);
        return val;
    }

    // Remove and return max - O(log n)
    T pop_max() {
        if (s.empty()) throw runtime_error("pop_max on empty PQ");
        // rbegin is not erase-able directly; convert to base iterator
        auto it = prev(s.end());
        T val = *it;
        s.erase(it);
        return val;
    }

    // Remove one occurrence of key, return true if removed: O(log n)
    bool remove_key(const T &key) {
        auto it = s.find(key); // finds one instance (if any)
        if (it == s.end()) return false;
        s.erase(it);
        return true;
    }

    // UpdateKey(oldkey, newkey): remove one oldkey and insert newkey
    // returns true if oldkey existed and was replaced; O(log n)
    bool update_key(const T &oldkey, const T &newkey) {
        auto it = s.find(oldkey);
        if (it == s.end()) return false;
        s.erase(it);        // O(log n)
        s.insert(newkey);   // O(log n)
        return true;
    }

    // Size for convenience
    size_t size() const { return s.size(); }

    // Debug: print contents in sorted order
    void debug_print() const {
        cout << "Contents (sorted): ";
        for (const auto &x : s) cout << x << ' ';
        cout << '\n';
    }
};

int main() {
    MinMaxPQ_BST<int> pq;

    pq.insert(5);
    pq.insert(1);
    pq.insert(10);
    pq.insert(5); // duplicate

    pq.debug_print(); // Contents (sorted): 1 5 5 10

    cout << "min: " << pq.get_min() << '\n'; // 1
    cout << "max: " << pq.get_max() << '\n'; // 10

    cout << "pop_min -> " << pq.pop_min() << '\n'; // removes 1
    pq.debug_print(); // 5 5 10

    cout << "remove_key(5) -> " << pq.remove_key(5) << '\n'; // true (one 5 removed)
    pq.debug_print(); // 5 10

    cout << "update_key(10 -> 2) -> " << pq.update_key(10, 2) << '\n'; // true
    pq.debug_print(); // 2 5

    cout << "pop_max -> " << pq.pop_max() << '\n'; // removes 5
    pq.debug_print(); // 2

    cout << "pop_min -> " << pq.pop_min() << '\n'; // removes 2
    cout << "empty? " << boolalpha << pq.empty() << '\n'; // true
    return 0;
}

/*
Contents (sorted): 1 5 5 10
min: 1
max: 10
pop_min -> 1
Contents (sorted): 5 5 10
remove_key(5) -> 1
Contents (sorted): 5 10
update_key(10 -> 2) -> 1
Contents (sorted): 2 5
pop_max -> 5
Contents (sorted): 2
pop_min -> 2
empty? true

*/


/* Union-Find Disjoint Sets (UFDS) */
typedef vector<int> vi;
class UnionFind{
private:
    vi p, rank, setSize; // vi p is the key part
    int numSets;

public:
    UnionFind(int N){
        p.assign(N, 0);
        for (int i = 0; i < N; ++i)
            p[i] = i;
        rank.assign(N, 0);    // optional speedup
        setSize.assign(N, 1); // optional feature
        numSets = N;          // optional feature
    }

    int findSet(int i) { 
       return (p[i] == i) ? i : (p[i] = findSet(p[i])); 
    }

    bool isSameSet(int i, int j) { 
       return findSet(i) == findSet(j); 
    }

    int numDisjointSets() {         // optional
       return numSets; 
    }

    int sizeOfSet(int i) {         // optional
       return setSize[findSet(i)]; 
    }

    void unionSet(int i, int j){
        if (isSameSet(i, j)) return;           // i and j are in same set
        int x = findSet(i), y = findSet(j);    // find both rep items
        if (rank[x] > rank[y])
            swap(x, y);                        // keep x ’shorter’ than y
        p[x] = y;                              // set x under y
        if (rank[x] == rank[y])
            ++rank[y];                        // optional speedup
        setSize[y] += setSize[x];             // combine set sizes at y
        --numSets;                           // a union reduces numSets
    }
};


// simple union find implementation
class unionfind(){
   vector<int> p;
   
   int find(int u){
      if(p[u] == u) return u;
      return p[u] = find(p[u]);
   }
   
   void Union(int u, int v){
      int a = find(u);
      int b = find(v);
      
      p[a] = b;
   }
   
   bool sameSet(int u, int v){
      if(find(u) == find(v)) return true;
      return false;
   }
}

int EXAMPLE(){
    UnionFind UF(5);                      // create 5 disjoint sets
    printf("%d\n", UF.numDisjointSets()); // 5
    UF.unionSet(0, 1);
    printf("%d\n", UF.numDisjointSets()); // 4
    UF.unionSet(2, 3);
    printf("%d\n", UF.numDisjointSets()); // 3
    UF.unionSet(4, 3);
    printf("%d\n", UF.numDisjointSets());                 // 2
    printf("isSameSet(0, 3) = %d\n", UF.isSameSet(0, 3)); // 0 (false)
    printf("isSameSet(4, 3) = %d\n", UF.isSameSet(4, 3)); // 1 (true)
    for (int i = 0; i < 5; ++i)                           // 1 for {0, 1} and 3 for {2, 3, 4}
        printf("findSet(%d) = %d, sizeOfSet(%d) = %d\n",
               i, UF.findSet(i), i, UF.sizeOfSet(i));
    UF.unionSet(0, 3);
    printf("%d\n", UF.numDisjointSets()); // 1
    for (int i = 0; i < 5; ++i)           // 3 for {0, 1, 2, 3, 4}
        printf("findSet(%d) = %d, sizeOfSet(%d) = %d\n",
               i, UF.findSet(i), i, UF.sizeOfSet(i));
    return 0;
}

/*
Fenwick (Binary Indexed) Tree, Range Update Point Query (RUPQ) Fenwick Tree and
Range Update Range Query (RURQ) Fenwick Tree
*/
#define LSOne(S) ((S) & -(S)) // the key operation

typedef long long ll; // for extra flexibility
typedef vector<ll> vll;
typedef vector<int> vi;

class FenwickTree{ // index 0 is not used
private:
    vll ft; // internal FT is an array
public:
    FenwickTree(int m) { ft.assign(m + 1, 0); } // create an empty FT

    void build(const vll &f){
        int m = (int)f.size() - 1; // note f[0] is always 0
        ft.assign(m + 1, 0);
        for (int i = 1; i <= m; ++i)
        {                                  // O(m)
            ft[i] += f[i];                 // add this value
            if (i + LSOne(i) <= m)         // i has parent
                ft[i + LSOne(i)] += ft[i]; // add to that parent
        }
    }

    FenwickTree(const vll &f) { build(f); } // create FT based on f

    FenwickTree(int m, const vi &s){ // create FT based on s
        vll f(m + 1, 0);
        for (int i = 0; i < (int)s.size(); ++i) // do the conversion first
            ++f[s[i]];                          // in O(n)
        build(f);                               // in O(m)
    }

    ll rsq(int j){ // returns RSQ(1, j)
        ll sum = 0;
        for (; j; j -= LSOne(j))
            sum += ft[j];
        return sum;
    }

    ll rsq(int i, int j) { return rsq(j) - rsq(i - 1); } // inc/exclusion

    // updates value of the i-th element by v (v can be +ve/inc or -ve/dec)
    void update(int i, ll v){
        for (; i < (int)ft.size(); i += LSOne(i))
            ft[i] += v;
    }

    int select(ll k){ // O(log m)
        int p = 1;
        while (p * 2 < (int)ft.size())
            p *= 2;
        int i = 0;
        while (p){
            if (k > ft[i + p]){
                k -= ft[i + p];
                i += p;
            }
            p /= 2;
        }
        return i + 1;
    }
};

class RUPQ{               // RUPQ variant
private:
    FenwickTree ft;       // internally use PURQ FT
public:
    RUPQ(int m) : ft(FenwickTree(m)) {}
    void range_update(int ui, int uj, ll v){
        ft.update(ui, v);      // [ui, ui+1, .., m] +v
        ft.update(uj + 1, -v); // [uj+1, uj+2, .., m] -v
    } // [ui, ui+1, .., uj] +v
    ll point_query(int i) { return ft.rsq(i); } // rsq(i) is sufficient
};

class RURQ{           // RURQ variant
private:              // needs two helper FTs
    RUPQ rupq;        // one RUPQ and
    FenwickTree purq; // one PURQ
public:
    RURQ(int m) : rupq(RUPQ(m)), purq(FenwickTree(m)) {} // initialization
    void range_update(int ui, int uj, ll v){
        rupq.range_update(ui, uj, v);  // [ui, ui+1, .., uj] +v
        purq.update(ui, v * (ui - 1)); // -(ui-1)*v before ui
        purq.update(uj + 1, -v * uj);  // +(uj-ui+1)*v after uj
    }
    ll rsq(int j){
        return rupq.point_query(j) * j - // optimistic calculation
               purq.rsq(j);              // cancelation factor
    }
    ll rsq(int i, int j) { return rsq(j) - rsq(i - 1); } // standard
};

int example(){
    vll f = {0, 0, 1, 0, 1, 2, 3, 2, 1, 1, 0}; // index 0 is always 0
    FenwickTree ft(f);
    printf("%lld\n", ft.rsq(1, 6));  // 7 => ft[6]+ft[4] = 5+2 = 7
    printf("%d\n", ft.select(7));    // index 6, rsq(1, 6) == 7, which is >= 7
    ft.update(5, 1);                 // update demo
    printf("%lld\n", ft.rsq(1, 10)); // now 12
    printf("=====\n");
    RUPQ rupq(10);
    RURQ rurq(10);
    rupq.range_update(2, 9, 7); // indices in [2, 3, .., 9] updated by +7
    rurq.range_update(2, 9, 7); // same as rupq above
    rupq.range_update(6, 7, 3); // indices 6&7 are further updated by +3 (10)
    rurq.range_update(6, 7, 3); // same as rupq above
    // idx = 0 (unused) | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |10
    // val = -          | 0 | 7 | 7 | 7 | 7 |10 |10 | 7 | 7 | 0
    for (int i = 1; i <= 10; i++)
        printf("%d -> %lld\n", i, rupq.point_query(i));
    printf("RSQ(1, 10) = %lld\n", rurq.rsq(1, 10)); // 62
    printf("RSQ(6, 7) = %lld\n", rurq.rsq(6, 7));   // 20
    return 0;
}

/* SegmentTree */
typedef vector<int> vi;

class SegmentTree{
private:
    int n;          // n = (int)A.size()
    vi A, st, lazy; // the arrays

    int l(int p) { return p << 1; }       // go to left child
    int r(int p) { return (p << 1) + 1; } // go to right child

    int conquer(int a, int b){
        if (a == -1) return b; // corner case
        if (b == -1) return a;
        return min(a, b); // RMQ
    }

    void build(int p, int L, int R){ // O(n)
        if (L == R) st[p] = A[L]; // base case
        else{
            int m = (L + R) / 2;
            build(l(p), L, m);
            build(r(p), m + 1, R);
            st[p] = conquer(st[l(p)], st[r(p)]);
        }
    }

    void propagate(int p, int L, int R){
        if (lazy[p] != -1){                        // has a lazy flag
            st[p] = lazy[p];                       // [L..R] has same value
            if (L != R)                            // not a leaf
                lazy[l(p)] = lazy[r(p)] = lazy[p]; // propagate downwards
            else                                   // L == R, a single index
                A[L] = lazy[p];                    // time to update this
            lazy[p] = -1;                          // erase lazy flag
        }
    }

    int RMQ(int p, int L, int R, int i, int j){    // O(log n)
        propagate(p, L, R);                        // lazy propagation
        if (i > j) return -1;                      // infeasible
        if ((L >= i) && (R <= j)) return st[p];    // found the segment
        int m = (L + R) / 2;
        return conquer(RMQ(l(p), L, m, i, min(m, j)),
                       RMQ(r(p), m + 1, R, max(i, m + 1), j));
    }

    void update(int p, int L, int R, int i, int j, int val){ // O(log n)
        propagate(p, L, R);                     // lazy propagation
        if (i > j) return;
        if ((L >= i) && (R <= j)){              // found the segment
            lazy[p] = val;                      // update this
            propagate(p, L, R);                 // lazy propagation
        }
        else{
            int m = (L + R) / 2;
            update(l(p), L, m, i, min(m, j), val);
            update(r(p), m + 1, R, max(i, m + 1), j, val);
            int lsubtree = (lazy[l(p)] != -1) ? lazy[l(p)] : st[l(p)];
            int rsubtree = (lazy[r(p)] != -1) ? lazy[r(p)] : st[r(p)];
            st[p] = conquer(lsubtree, rsubtree);
        }
    }

public:
    SegmentTree(int sz) : n(sz), A(n), st(4 * n), lazy(4 * n, -1) {}

    SegmentTree(const vi &initialA) : SegmentTree((int)initialA.size()){
        A = initialA;
        build(1, 0, n - 1);
    }

    void update(int i, int j, int val) { update(1, 0, n - 1, i, j, val); }

    int RMQ(int i, int j) { return RMQ(1, 0, n - 1, i, j); }
};

int example(){
    vi A = {18, 17, 13, 19, 15, 11, 20, 99}; // make n a power of 2
    SegmentTree st(A);

    printf("              idx    0, 1, 2, 3, 4, 5, 6, 7\n");
    printf("              A is {18,17,13,19,15,11,20,oo}\n");
    printf("RMQ(1, 3) = %d\n", st.RMQ(1, 3)); // 13
    printf("RMQ(4, 7) = %d\n", st.RMQ(4, 7)); // 11
    printf("RMQ(3, 4) = %d\n", st.RMQ(3, 4)); // 15

    st.update(5, 5, 77); // update A[5] to 77
    printf("              idx    0, 1, 2, 3, 4, 5, 6, 7\n");
    printf("Now, modify A into {18,17,13,19,15,77,20,oo}\n");
    printf("RMQ(1, 3) = %d\n", st.RMQ(1, 3)); // remains 13
    printf("RMQ(4, 7) = %d\n", st.RMQ(4, 7)); // now 15
    printf("RMQ(3, 4) = %d\n", st.RMQ(3, 4)); // remains 15

    st.update(0, 3, 30); // update A[0..3] to 30
    printf("              idx    0, 1, 2, 3, 4, 5, 6, 7\n");
    printf("Now, modify A into {30,30,30,30,15,77,20,oo}\n");
    printf("RMQ(1, 3) = %d\n", st.RMQ(1, 3)); // now 30
    printf("RMQ(4, 7) = %d\n", st.RMQ(4, 7)); // remains 15
    printf("RMQ(3, 4) = %d\n", st.RMQ(3, 4)); // remains 15

    st.update(3, 3, 7); // update A[3] to 7
    printf("              idx    0, 1, 2, 3, 4, 5, 6, 7\n");
    printf("Now, modify A into {30,30,30, 7,15,77,20,oo}\n");
    printf("RMQ(1, 3) = %d\n", st.RMQ(1, 3)); // now 7
    printf("RMQ(4, 7) = %d\n", st.RMQ(4, 7)); // remains 15
    printf("RMQ(3, 4) = %d\n", st.RMQ(3, 4)); // now 7

    return 0;
}

/* Graph Related common questions solution */

// DFS

/*
// basic flags
enum {
    UNVISITED = -1,
    VISITED = -2
};

vi dfs_num;                                    // initially all UNVISITED
void dfs(int u){                               // normal usage
    dfs_num[u] = VISITED;                      // mark u as visited
    for (auto &[v, w] : AL[u])                 // C++17 style, w ignored
        if (dfs_num[v] == UNVISITED)           // to avoid cycle
            dfs(v);                            // recursively visits v
}
*/

#define GRAPHSIZE 20
enum{
    w = 0,
    g = 1,
    b = 2
};

int n, time_counter;
int c[GRAPHSIZE], Parent[GRAPHSIZE], discovery_time[GRAPHSIZE], finishing_time[GRAPHSIZE];
vector<vector<int>> Adj(GRAPHSIZE);

void DFS_VISIT(int v){
    c[v] = g;
    time_counter = time_counter + 1;
    discovery_time[v] = time_counter;
    for (int i = 0; i < Adj[v].size(); i++){
        if (c[Adj[v][i]] == w){
            Parent[Adj[v][i]] = v;
            DFS_VISIT(Adj[v][i]);
        }
    }
    time_counter = time_counter + 1;
    finishing_time[v] = time_counter;
    c[v] = b;
}

void DFS(){
    for (int i = 0; i < n; i++){
        c[i] = w;
        discovery_time[i] = -1;
        finishing_time[i] = -1;
        Parent[i] = -1;
    }
    time_counter = 0;
    for (int i = 0; i < n; i++){
        if (c[i] == w) DFS_VISIT(i);
    }
}

int main()
{
    int temp, s;
    cin >> n;
    for (int i = 0; i < n; i++){
        cin >> s;
        for (int j = 0; j < s; j++){
            cin >> temp;
            Adj[i].push_back(temp);
        }
    }

    DFS();
    for (int i = 0; i < n; i++){
        cout << Parent[i] << " ";
    }
    cout << endl;
}

/*
$ ./a.exe
9
1
1
2
2 3
2
1 3
3
1 2 4
1
3
0
2
7 8
1
6
1
6
-1 0 1 2 3 -1 -1 6 6
*/

// BFS

#define GRAPHSIZE 20
enum{
    w = 0,
    g = 1,
    b = 2
};

int n, time_counter;
int c[GRAPHSIZE], Parent[GRAPHSIZE], d[GRAPHSIZE];
vector<vector<int>> Adj(GRAPHSIZE);

void BFS_VISIT(int v){
    queue<int> q;
    int node;
    d[v] = 0;
    q.push(v); // enqueue v
    c[v] = g;
    while (!q.empty()){
        int node = q.front();
        q.pop(); // dequeue
        for (int neighbor : Adj[node]){
            if (c[neighbor] == w){
                q.push(neighbor);
                c[neighbor] = g;
                Parent[neighbor] = node;
                d[neighbor] = d[node] + 1;
            }
        }
        c[node] = b;
    }
}

void BFS(){
    for (int i = 0; i < n; i++){
        c[i] = w;
        Parent[i] = -1;
        d[i] = INT32_MAX;
    }

    for (int i = 0; i < n; i++)
        if (c[i] == w) BFS_VISIT(i);
}

int main(){
    int temp, s;
    cin >> n;
    for (int i = 0; i < n; i++){
        cin >> s;
        for (int j = 0; j < s; j++){
            cin >> temp;
            Adj[i].push_back(temp);
        }
    }

    BFS();
    for (int i = 0; i < n; i++){
        cout << Parent[i] << " ";
    }
    cout << endl;
}

/*
$ ./a.exe
9
1
1
2
2 3
2
1 3
3
1 2 4
1
3
0
2
7 8
1
6
1
6
-1 0 1 1 3 -1 -1 6 6
*/

// Finding Connected Components (Undirected Graph)
/*
DFS_VISIT(int v){
    ...
}
DFS(){
    int numCC = 0;             <------ additional
    for(int i=0; i<n; i++){
        ...
    }
    time_counter = 0;
    for(int i=0; i<n; i++){
        if(c[i] == w){
            numCC++;           <------ additional
            DFS_VISIT(i);
        }
    }
    printf("There are %d connected components\n", numCC);      <------ additional
}
*/

// Flood Fill (Implicit 2D Grid Graph) like ms paint bucket color freature
/*
# UVa 00469 - Wetlands of Florida

int dr[] = {1, 1, 0, -1, -1, -1, 0, 1};                 // the order is:
int dc[] = {0, 1, 1, 1, 0, -1, -1, -1};                 // S/SE/E/NE/N/NW/W/SW
int floodfill(int r, int c, char c1, char c2){          // returns the size of CC
    if ((r < 0) || (r >= R)) return 0;                  // outside grid, part 1
    if ((c < 0) || (c >= C)) return 0;                  // outside grid, part 2
    if (grid[r][c] != c1) return 0;                     // does not have color c1
    int ans = 1;                                        // (r, c) has color c1
    grid[r][c] = c2;                                    // to avoid cycling
    for (int d = 0; d < 8; ++d)
        ans += floodfill(r + dr[d], c + dc[d], c1, c2); // the code is neat as
    return ans;                                         // we use dr[] and dc[]
}

// inside int main()
// read the grid as a global 2D array + read (row, col) query coordinates
printf("%d\n", floodfill(row, col, ‘W’, ‘.’)); // count size of wet area
LLLLLLLLL         LLLLLLLLL
LLWWLLWLL         LL..LLWLL     // The size of CC
LWWLLLLLL (R2,C1) L..LLLLLL     // with one ‘W’
LWWWLWWLL         L...L..LL     // at (R2, C1) is 12
LLLWWWLLL ======> LLL...LLL
LLLLLLLLL         LLLLLLLLL     // Notice that all these
LLLWWLLWL         LLLWWLLWL     // connected ‘W’s are
LLWLWLLLL         LLWLWLLLL     // replaced with ‘.’s
LLLLLLLLL         LLLLLLLLL     // after floodfill
*/

// Topological Sort (Directed Acyclic Graph)
/*
void toposort(int u){
    dfs_num[u] = VISITED;
    for (auto &[v, w] : AL[u])
        if (dfs_num[v] == UNVISITED)
            toposort(v);
    ts.push_back(u);                   // this is the only change
}
// inside int main()
dfs_num.assign(V, UNVISITED);         // global variable
ts.clear();                           // global variable
for (int u = 0; u < V; ++u)           // same as finding CCs
    if (dfs_num[u] == UNVISITED)
        toposort(u);
reverse(ts.begin(), ts.end());        // reverse ts or
for (auto &u : ts)                    // simply read the content
    printf(" %d", u);                 // of ts backwards
printf("\n");
// For the sample graph in Figure 4.3, the output is like this:
// 7 6 0 1 2 5 3 4


Kahn’s Algorithm

// enqueue vertices with zero in-degree into a min (priority) queue pq
priority_queue<int, vi, greater<int>> pq;                // min priority queue
for (int u = 0; u < N; ++u)                        +------ maintain in_degree array of all node
    if (in_degree[u] == 0)                    <----+     // next to be processed
        pq.push(u);                                      // smaller index first
while (!pq.empty()) {                                    // Kahn’s algorithm
    int u = pq.top(); pq.pop();                          // process u here
    for (auto &v : AL[u]) {
        --in_degree[v];                                  // virtually remove u->v
        if (in_degree[v] > 0) continue;                  // not a candidate, skip
        pq.push(v);                                      // enqueue v in pq
    }
}
*/

// Bipartite Graph Check (Undirected Graph)

/*
// inside int main()
int s = 0;
queue<int> q; q.push(s);
vi color(n, -1); color[s] = 0;
bool isBipartite = true;                                // add a Boolean flag
while (!q.empty() && isBipartite){                      // as with original BFS
    int u = q.front(); q.pop();
    for (auto &v : AL[u]){
        if (color[v] == -1){                            // don’t record distances
            color[v] = color[u] + 1;                    // just record two colors
            q.push(v);
        }
        else if (color[v] == color[u]){                 // u & v have same color
            isBipartite = false;                        // a coloring conflict :(
            break;                                      // optional speedup
        }
    }
}
*/

// Cycle Check (Directed Graph)

/*
void cycleCheck(int u){                                 // check edge properties
    dfs_num[u] = EXPLORED;                              // color u as EXPLORED
    for (auto &[v, w] : AL[u]){                         // C++17 style, w ignored
        printf("Edge (%d, %d) is a ", u, v);
        if (dfs_num[v] == UNVISITED){                   // EXPLORED->UNVISITED
            printf("Tree Edge\n");
            dfs_parent[v] = u;                          // a tree edge u->v
            cycleCheck(v);
        }
        else if (dfs_num[v] == EXPLORED){               // EXPLORED->EXPLORED
            if (v == dfs_parent[u])                     // differentiate them
                printf("Bidirectional Edge\n");         // a trivial cycle
            else
                printf("Back Edge (Cycle)\n");          // a non trivial cycle
        }
        else if (dfs_num[v] == VISITED)                 // EXPLORED->VISITED
            printf("Forward/Cross Edge\n");             // rare application
    }
    dfs_num[u] = VISITED;                               // color u as VISITED/DONE
}
// inside int main()
dfs_num.assign(V, UNVISITED);
dfs_parent.assign(V, -1);
for (int u = 0; u < V; ++u)
    if (dfs_num[u] == UNVISITED)
        cycleCheck(u);
*/

// Finding Articulation Points and Bridges (Undirected Graph)

// Finding Strongly Connected Components (Directed Graph)

// Suffix Trie OR Trie
struct vertex{
    char alphabet;
    bool exist;
    vector<vertex *> child;
    vertex(char a) : alphabet(a), exist(false) { child.assign(26, NULL); }
};

class Trie{   // this is TRIE
private:      // NOT Suffix Trie
    vertex *root;

public:
    Trie() { root = new vertex('!'); }

    void insert(string word){ // insert a word into trie
        vertex *cur = root;
        for (int i = 0; i < (int)word.size(); ++i){ // O(n)
            int alphaNum = word[i] - 'A';
            if (cur->child[alphaNum] == NULL) // add new branch if NULL
                cur->child[alphaNum] = new vertex(word[i]);
            cur = cur->child[alphaNum];
        }
        cur->exist = true;
    }

    bool search(string word){ // true if word in trie
        vertex *cur = root;
        for (int i = 0; i < (int)word.size(); ++i){ // O(m)
            int alphaNum = word[i] - 'A';
            if (cur->child[alphaNum] == NULL) // not found
                return false;
            cur = cur->child[alphaNum];
        }
        return cur->exist; // check exist flag
    }

    bool startsWith(string prefix){ // true if match prefix
        vertex *cur = root;
        for (int i = 0; i < (int)prefix.size(); ++i){
            int alphaNum = prefix[i] - 'A';
            if (cur->child[alphaNum] == NULL) // not found
                return false;
            cur = cur->child[alphaNum];
        }
        return true; // reach here, return true
    }
};

typedef pair<int, int> ii;
typedef vector<int> vi;

class SuffixArray{
private:
    vi RA; // rank array

    void countingSort(int k){                                       // O(n)
        int maxi = max(300, n);             // up to 255 ASCII chars
        vi c(maxi, 0);                      // clear frequency table
        for (int i = 0; i < n; ++i)         // count the frequency
            ++c[i + k < n ? RA[i + k] : 0]; // of each integer rank
        for (int i = 0, sum = 0; i < maxi; ++i){
            int t = c[i];
            c[i] = sum;
            sum += t;
        }
        vi tempSA(n);
        for (int i = 0; i < n; ++i) // sort SA
            tempSA[c[SA[i] + k < n ? RA[SA[i] + k] : 0]++] = SA[i];
        swap(SA, tempSA); // update SA
    }

    void constructSA(){ // can go up to 400K chars
        SA.resize(n);
        iota(SA.begin(), SA.end(), 0); // the initial SA
        RA.resize(n);
        for (int i = 0; i < n; ++i)
            RA[i] = T[i]; // initial rankings
        for (int k = 1; k < n; k <<= 1){ // repeat log_2 n times
            // this is actually radix sort
            countingSort(k); // sort by 2nd item
            countingSort(0); // stable-sort by 1st item
            vi tempRA(n);
            int r = 0;
            tempRA[SA[0]] = r;          // re-ranking process
            for (int i = 1; i < n; ++i) // compare adj suffixes
                tempRA[SA[i]] =         // same pair => same rank r; otherwise, increase r
                    ((RA[SA[i]] == RA[SA[i - 1]]) && (RA[SA[i] + k] == RA[SA[i - 1] + k])) ? r : ++r;
            swap(RA, tempRA); // update RA
            if (RA[SA[n - 1]] == n - 1)
                break; // nice optimization
        }
    }

    void computeLCP(){
        vi Phi(n);
        vi PLCP(n);
        PLCP.resize(n);
        Phi[SA[0]] = -1;            // default value
        for (int i = 1; i < n; ++i) // compute Phi in O(n)
            Phi[SA[i]] = SA[i - 1]; // remember prev suffix
        for (int i = 0, L = 0; i < n; ++i){ // compute PLCP in O(n)
            if (Phi[i] == -1){
                PLCP[i] = 0;
                continue;
            } // special case
            while ((i + L < n) && (Phi[i] + L < n) && (T[i + L] == T[Phi[i] + L]))
                ++L; // L incr max n times
            PLCP[i] = L;
            L = max(L - 1, 0); // L dec max n times
        }
        LCP.resize(n);
        for (int i = 0; i < n; ++i) // compute LCP in O(n)
            LCP[i] = PLCP[SA[i]];   // restore PLCP
    }

public:
    const char *T; // the input string
    const int n;   // the length of T
    vi SA;         // Suffix Array
    vi LCP;        // of adj sorted suffixes

    SuffixArray(const char *initialT, const int _n) : T(initialT), n(_n){
        constructSA(); // O(n log n)
        computeLCP();  // O(n)
    }

    ii stringMatching(const char *P){                           // in O(m log n)
        int m = (int)strlen(P); // usually, m < n
        int lo = 0, hi = n - 1; // range = [0..n-1]
        while (lo < hi){                                         // find lower bound
            int mid = (lo + hi) / 2;              // this is round down
            int res = strncmp(T + SA[mid], P, m); // P in suffix SA[mid]?
            (res >= 0) ? hi = mid : lo = mid + 1; // notice the >= sign
        }
        if (strncmp(T + SA[lo], P, m) != 0)
            return {-1, -1}; // if not found
        ii ans;
        ans.first = lo;
        hi = n - 1; // range = [lo..n-1]
        while (lo < hi){ // now find upper bound
            int mid = (lo + hi) / 2;
            int res = strncmp(T + SA[mid], P, m);
            (res > 0) ? hi = mid : lo = mid + 1; // notice the > sign
        }
        if (strncmp(T + SA[hi], P, m) != 0)
            --hi; // special case
        ans.second = hi;
        return ans; // returns (lb, ub)
    } // where P is found

    ii LRS(){ // (LRS length, index)
        int idx = 0, maxLCP = -1;
        for (int i = 1; i < n; ++i) // O(n), start from i = 1
            if (LCP[i] > maxLCP)
                maxLCP = LCP[i], idx = i;
        return {maxLCP, idx};
    }

    ii LCS(int split_idx){ // (LCS length, index)
        int idx = 0, maxLCP = -1;
        for (int i = 1; i < n; ++i)
        { // O(n), start from i = 1
            // if suffix SA[i] and suffix SA[i-1] came from the same string, skip
            if ((SA[i] < split_idx) == (SA[i - 1] < split_idx))
                continue;
            if (LCP[i] > maxLCP)
                maxLCP = LCP[i], idx = i;
        }
        return {maxLCP, idx};
    }
};

const int MAX_N = 450010; // can go up to 450K chars

char T[MAX_N];
char P[MAX_N];
char LRS_ans[MAX_N];
char LCS_ans[MAX_N];

int main(){
    freopen("sa_lcp_in.txt", "r", stdin);
    scanf("%s", &T);        // read T
    int n = (int)strlen(T); // count n
    T[n++] = '$';           // add terminating symbol
    SuffixArray S(T, n);    // construct SA+LCP

    printf("T = '%s'\n", T);
    printf(" i SA[i] LCP[i]   Suffix SA[i]\n");
    for (int i = 0; i < n; ++i)
        printf("%2d    %2d    %2d    %s\n", i, S.SA[i], S.LCP[i], T + S.SA[i]);

    // String Matching demo, we will try to find P in T
    strcpy(P, "A");
    auto [lb, ub] = S.stringMatching(P);
    if ((lb != -1) && (ub != -1))
    {
        printf("P = '%s' is found SA[%d..%d] of T = '%s'\n", P, lb, ub, T);
        printf("They are:\n");
        for (int i = lb; i <= ub; ++i)
            printf("  %s\n", T + S.SA[i]);
    }
    else
        printf("P = '%s' is not found in T = '%s'\n", P, T);

    // LRS demo, find the LRS of T
    auto [LRS_len, LRS_idx] = S.LRS();
    strncpy(LRS_ans, T + S.SA[LRS_idx], LRS_len);
    printf("The LRS is '%s' with length = %d\n", LRS_ans, LRS_len);

    // LCS demo, find the LCS of (T, P)
    strcpy(P, "CATA");
    int m = (int)strlen(P);
    strcat(T, P);       // append P to T
    strcat(T, "#");     // add '#' at the back
    n = (int)strlen(T); // update n

    // reconstruct SA of the combined strings
    SuffixArray S2(T, n); // reconstruct SA+LCP
    int split_idx = n - m - 1;
    printf("T+P = '%s'\n", T);
    printf(" i SA[i] LCP[i] From  Suffix SA[i]\n");
    for (int i = 0; i < n; ++i)
        printf("%2d    %2d    %2d    %2d    %s\n",
               i, S2.SA[i], S2.LCP[i], S2.SA[i] < split_idx ? 1 : 2, T + S2.SA[i]);

    auto [LCS_len, LCS_idx] = S2.LCS(split_idx);
    strncpy(LCS_ans, T + S2.SA[LCS_idx], LCS_len);
    printf("The LCS is '%s' with length = %d\n", LCS_ans, LCS_len);

    return 0;
}
