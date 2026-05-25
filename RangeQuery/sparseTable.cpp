#include <bits/stdc++.h>
using namespace std;

#define FAST_IO ios::sync_with_stdio(false); cin.tie(nullptr)

/*
Sparse Table : a data structure that allows for O(1) range queries on static arrays. 
It is specifically designed for idempotent operations.

Idempotent operations: Minimun, Maximum, GCD, Bitwise OR/AND

1. Preprocessing (Building the Table): O(N log N)
2. Query: O(1)

Any range [l, r] can be covered by two overlapping powers of 2
min(l, r) = min(
    min(l, l+2^k-1),
    min(r-2^k+1, r)
)

st[i][k] = minimum of subarray starting at i with length 2^k
st[i][0] → single element
st[i][1] → range length 2
st[i][2] → range length 4

Variants: Replace min with
    1. Max -> max()
    2. GCD -> __gcd()
    3. AND -> &
    4. OR  -> |
*/

class SparseTableMin {
public:
    int n, log_size;
    vector<int> a;
    vector<vector<int>> st;
    vector<int> lg;

    SparseTableMin(int size, vector<int> &x) : n(size), a(x), lg(size + 1) {
        log_size = floor(log2(n)) + 1; 
        st.assign(n, vector<int>(log_size)); 

        build_log();
        build_sparse_table();
    }

    void build_log() {
        lg[1] = 0;
        for (int i = 2; i <= n; i++)
            lg[i] = lg[i / 2] + 1;
    }

    void build_sparse_table() {
        for (int i = 0; i < n; i++)
            st[i][0] = a[i];

        for (int k = 1; k < log_size; k++) {
            for (int i = 0; i + (1 << k) <= n; i++) {
                st[i][k] = min(
                    st[i][k - 1],
                    st[i + (1 << (k - 1))][k - 1]
                );
            }
        }
    }

    int query(int l, int r) {
        int len = r - l + 1;
        int k = lg[len];
        return min(
            st[l][k],
            st[r - (1 << k) + 1][k]
        );
    }
};

template <typename T>
struct SparseTable {
    int n, max_log;
    vector<vector<T>> st;
    
    // CHANGE THIS: to max, gcd, etc.
    T merge(const T& a, const T& b) { return min(a, b); }

    SparseTable(const vector<T>& arr) {
        n = arr.size();
        max_log = 32 - __builtin_clz(n);
        st.assign(max_log, vector<T>(n));
        for (int i = 0; i < n; i++) st[0][i] = arr[i];
        
        for (int j = 1; j < max_log; j++) {
            for (int i = 0; i + (1 << j) <= n; i++) {
                st[j][i] = merge(st[j - 1][i], st[j - 1][i + (1 << (j - 1))]);
            }
        }
    }

    T query(int L, int R) { // 0-indexed, inclusive [L, R]
        int j = 31 - __builtin_clz(R - L + 1);
        return merge(st[j][L], st[j][R - (1 << j) + 1]);
    }
};

int main() {
    FAST_IO;

    int n, q, l, r;
    cin >> n >> q;

    vector<int> x(n);
    for(int i = 0; i < n; i++)
        cin >> x[i];

    SparseTableMin sp(n, x);
    
    while(q--) {
        cin >> l >> r;
        cout << sp.query(l-1, r-1) << "\n";
    }
}
