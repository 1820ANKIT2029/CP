#include <bits/stdc++.h>
using namespace std;

/*
    Hashing: Polynomial Hash, Rolling Hash, Double Hashing
    
    1. Polynomial Hash Formula (String S of length L)
    - H(S) = (S[0] * p^(L-1) + S[1] * p^(L-2) + ... + S[L-1] * p^0) % m
    - (p = prime base like 31, m = large prime modulus like 1e9+7)
    
    2. Rolling Hash Formula for Substring S[L...R]
    - H(L, R) = (Pref[R+1] - Pref[L] * p^(R-L+1)) % m
    - Note: To handle C++ negative modulo, use: H = (H % m + m) % m
    
    3. Double Hashing Formula/Condition
    - Compute H1 using (p1, m1) and H2 using (p2, m2)
    - S1 == S2 is True iff: (H1_S1 == H1_S2) && (H2_S1 == H2_S2)
*/
class DoubleHash {
private:
    // Two distinct pairs of primes and moduli
    long long p1 = 31, m1 = 1e9 + 7;
    long long p2 = 37, m2 = 1e9 + 9;
    
    vector<long long> pref1, pref2;
    vector<long long> pow1, pow2;

public:
    DoubleHash(const string& s) {
        int n = s.size();
        pref1.assign(n + 1, 0); pref2.assign(n + 1, 0);
        pow1.assign(n + 1, 1);  pow2.assign(n + 1, 1);

        for (int i = 0; i < n; i++) {
            // Precompute the powers of our bases
            pow1[i + 1] = (pow1[i] * p1) % m1;
            pow2[i + 1] = (pow2[i] * p2) % m2;

            // Precompute the polynomial prefix hashes
            // H[i+1] = (H[i] * p + char_value) % m
            long long val = s[i] - 'a' + 1; // 1-indexed so 'a' isn't 0
            
            pref1[i + 1] = (pref1[i] * p1 + val) % m1;
            pref2[i + 1] = (pref2[i] * p2 + val) % m2;
        }
    }

    // O(1) Substring Hash Retrieval
    pair<long long, long long> get_hash(int L, int R) {
        long long len = R - L + 1;
        
        // H(L, R) = (Pref[R+1] - Pref[L] * p^(R-L+1)) % m
        long long h1 = (pref1[R + 1] - (pref1[L] * pow1[len]) % m1) % m1;
        // In C++, the modulo of a negative number is negative. We fix that:
        if (h1 < 0) h1 += m1; 

        long long h2 = (pref2[R + 1] - (pref2[L] * pow2[len]) % m2) % m2;
        if (h2 < 0) h2 += m2;

        return {h1, h2};
    }
};

int main() {
    string text = "abcbabc";
    DoubleHash hasher(text);

    // Get hashes for the two "abc" substrings
    pair<long long, long long> hash_left = hasher.get_hash(0, 2);  // "abc"
    pair<long long, long long> hash_right = hasher.get_hash(4, 6); // "abc"
    
    if (hash_left == hash_right) {
        cout << "Substrings match perfectly in O(1) time!\n";
    }

    return 0;
}

/*
    String Pattern Matching Algorithms:
    1. Knuth-Morris-Pratt (KMP) Algorithm O(n + m)
    2. Z-Algorithm O(n + m)
    3. Rabin-Karp Algorithm O(n + m)
*/

// Computes the Pi array in O(N) time
vector<int> compute_pi(const string& s) {
    int n = s.length();
    vector<int> pi(n, 0);
    
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }
    return pi;
}

// Kmp search algorithm for pattern matching
vector<int> kmp_search(const string& pattern, const string& text) {
    string combined = pattern + "#" + text;
    vector<int> pi = compute_pi(combined);
    vector<int> occurrences;
    
    int p_len = pattern.length();
    int n = combined.length();
    for (int i = p_len + 1; i < n; i++) {
        if (pi[i] == p_len) occurrences.push_back(i - 2 * p_len);
    }
    return occurrences;
}

// Z-algorithm for pattern matching
vector<int> z_function(const string& s) {
    int n = s.length();
    vector<int> z(n, 0);
    
    // [l, r] represents the rightmost matching window
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i <= r) {
            z[i] = min(r - i + 1, z[i - l]);
        }

        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
        
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

// Rabin-Karp algorithm for pattern matching
vector<int> rabin_karp(string const& pattern, string const& text) {
    const long long P = 31; 
    const long long M = 1e9 + 9;
    int S = pattern.size(), T = text.size();
    
    if (S > T || S == 0) return {};

    vector<int> occurrences;
    long long p_pow = 1;
    long long pattern_hash = 0, window_hash = 0;

    // pattern and initial window hash calculation
    for (int i = 0; i < S; i++) {
        pattern_hash = (pattern_hash * P + pattern[i]) % M;
        window_hash = (window_hash * P + text[i]) % M;
        if (i < S - 1) p_pow = (p_pow * P) % M;
    }

    if (pattern_hash == window_hash) occurrences.push_back(0);

    for (int i = S; i < T; i++) {
        // Remove the leftmost character from the hash
        long long removed_term = (text[i - S] * p_pow) % M;
        window_hash = (window_hash - removed_term + M) % M;
        
        // Add the new rightmost character to the hash
        window_hash = (window_hash * P + text[i]) % M;

        if (window_hash == pattern_hash) {
            // if (text.substr(i - S + 1, S) == pattern) // Optional: verify the match to avoid hash collisions
            occurrences.push_back(i - S + 1);
        }
    }
    return occurrences;
}

// Manacher's algorithm for longest palindromic substring in O(n) time
// Returns an array p where p[i] is the palindrome radius at index i
// in the modified string.
vector<int> manacher(const string& s) {
    if (s.empty()) return {};
    string t = "^#";
    for (char c : s) { t += c; t += '#'; }
    t += "$";

    int n = t.length();
    vector<int> p(n, 0);
    int center = 0, right = 0;

    for (int i = 1; i < n - 1; i++) {
        int i_mirror = 2 * center - i;

        if (right > i) p[i] = min(right - i, p[i_mirror]);

        while (t[i + 1 + p[i]] == t[i - 1 - p[i]]) p[i]++;
        if (i + p[i] > right) {
            center = i;
            right = i + p[i];
        }
    }
    
    return p;
}

/*
    Trie (Prefix Tree) for efficient string storage and retrieval
*/
struct TrieNode {
    int next[26];
    bool is_end_of_string;
    int count;

    TrieNode() {
        fill(begin(next), end(next), -1);
        is_end_of_string = false;
        count = 0;
    }
};

struct Trie {
    vector<TrieNode> tree;

    Trie() {
        tree.emplace_back();
    }

    void insert(const string& s) {
        int curr = 0;
        for (char c : s) {
            int index = c - 'a';
            if (tree[curr].next[index] == -1) {
                tree[curr].next[index] = tree.size();
                tree.emplace_back();
            }
            curr = tree[curr].next[index];
            tree[curr].count++;
        }
        tree[curr].is_end_of_string = true;
    }

    bool search(const string& s) {
        int curr = 0;
        for (char c : s) {
            int index = c - 'a';
            if (tree[curr].next[index] == -1) return false;
            curr = tree[curr].next[index];
        }
        return tree[curr].is_end_of_string;
    }
};

/*
    Bitwise Trie for efficient XOR queries
*/
struct BitTrieNode {
    int next[2];
    BitTrieNode() { next[0] = next[1] = -1; }
};

struct BitTrie {
    vector<BitTrieNode> tree;
    const int MAX_BITS = 31; // For 32-bit signed integers

    BitTrie() {
        tree.emplace_back();
    }

    void insert(int num) {
        int curr = 0;
        for (int i = MAX_BITS; i >= 0; --i) {
            int bit = (num >> i) & 1;
            if (tree[curr].next[bit] == -1) {
                tree[curr].next[bit] = tree.size();
                tree.emplace_back();
            }
            curr = tree[curr].next[bit];
        }
    }

    int get_max_xor(int num) {
        int curr = 0, max_xor = 0;
        for (int i = MAX_BITS; i >= 0; --i) {
            int bit = (num >> i) & 1;
            int opposite_bit = 1 - bit;
            
            if (tree[curr].next[opposite_bit] != -1) {
                max_xor |= (1 << i);
                curr = tree[curr].next[opposite_bit];
            } else {
                curr = tree[curr].next[bit];
            }
        }
        return max_xor;
    }
};


/*
    Aho-Corasick Automaton
    
*/
struct ACNode {
    int next[26];
    int link;      // Suffix link
    int dict_link; // Link to nearest terminal node (exit link)
    vector<int> match_ids; // IDs of patterns ending here

    ACNode() {
        fill(begin(next), end(next), -1);
        link = -1;
        dict_link = -1;
    }
};

struct AhoCorasick {
    vector<ACNode> t;

    AhoCorasick() { t.emplace_back(); }

    void insert(const string& s, int pattern_id) {
        int curr = 0;
        for (char c : s) {
            int idx = c - 'a';
            if (t[curr].next[idx] == -1) {
                t[curr].next[idx] = t.size();
                t.emplace_back();
            }
            curr = t[curr].next[idx];
        }
        t[curr].match_ids.push_back(pattern_id);
    }

    void build_automaton() {
        queue<int> q;
        t[0].link = 0;
        t[0].dict_link = 0;

        for (int c = 0; c < 26; ++c) {
            int child = t[0].next[c];
            if (child != -1) {
                t[child].link = 0;
                t[child].dict_link = (t[child].match_ids.empty() ? 0 : child);
                q.push(child);
            } else {
                t[0].next[c] = 0; 
            }
        }

        // BFS to set suffix and dictionary links
        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int c = 0; c < 26; ++c) {
                int child = t[u].next[c];
                if (child != -1) {
                    int fail = t[u].link;
                    t[child].link = t[fail].next[c];

                    int l = t[child].link;
                    t[child].dict_link = t[l].match_ids.empty() ? t[l].dict_link : l;
                    
                    q.push(child);
                } else {
                    t[u].next[c] = t[t[u].link].next[c];
                }
            }
        }
    }

    // Example usage: Count total occurrences of all dictionary words
    long long count_matches(const string& text) {
        long long matches = 0;
        int curr = 0;
        for (char c : text) {
            curr = t[curr].next[c - 'a'];
            
            int temp = t[curr].match_ids.empty() ? t[curr].dict_link : curr;
            while (temp != 0) {
                matches += t[temp].match_ids.size();
                temp = t[temp].dict_link;
            }
        }
        return matches;
    }
};

struct SuffixArray {
    string s;
    int n;
    vector<int> p, c, lcp;

    // O(N log N) Construction
    SuffixArray(string text) {
        s = text + "$"; 
        n = s.length();
        p.assign(n, 0);
        c.assign(n, 0);
        build_sa();
        build_lcp();
    }

    void build_sa() {
        vector<pair<char, int>> a(n);
        for (int i = 0; i < n; i++) a[i] = {s[i], i};
        sort(a.begin(), a.end());

        for (int i = 0; i < n; i++) p[i] = a[i].second;
        c[p[0]] = 0;
        for (int i = 1; i < n; i++) {
            c[p[i]] = c[p[i - 1]] + (a[i].first != a[i - 1].first);
        }

        int k = 0;
        while ((1 << k) < n) {
            vector<pair<pair<int, int>, int>> b(n);
            for (int i = 0; i < n; i++) {
                b[i] = {{c[i], c[(i + (1 << k)) % n]}, i};
            }
            sort(b.begin(), b.end());

            for (int i = 0; i < n; i++) p[i] = b[i].second;
            c[p[0]] = 0;
            for (int i = 1; i < n; i++) {
                c[p[i]] = c[p[i - 1]] + (b[i].first != b[i - 1].first);
            }
            k++;
        }
    }

    // Kasai's Algorithm for LCP in O(N)
    void build_lcp() {
        lcp.assign(n, 0);
        int k = 0;
        for (int i = 0; i < n - 1; i++) {
            int pi = c[i];
            int j = p[pi - 1];
            // LCP of suffix i and j
            while (s[i + k] == s[j + k]) k++;
            lcp[pi] = k;
            if (k > 0) k--;
        }
    }
};


struct Node {
    int next[26]; // Trie transitions (edges)
    int len;      // Length of the palindrome
    int link;     // Suffix link
    long long cnt; // Frequency of this palindrome

    Node() {
        fill(begin(next), end(next), 0);
        len = 0;
        link = 0;
        cnt = 0;
    }
};

struct Eertree {
    vector<Node> tree;
    string s;
    int sz;   
    int last;

    Eertree(int max_len) {
        tree.resize(max_len + 2);
        
        tree[0].len = 0;
        tree[0].link = 1;
        
        tree[1].len = -1;
        tree[1].link = 1;
        
        sz = 2;
        last = 0;
    }

    int get_link(int u) {
        int pos = s.length() - 1;
        while (pos - 1 - tree[u].len < 0 || s[pos - 1 - tree[u].len] != s[pos]) 
            u = tree[u].link;
        
        return u;
    }

    void add_letter(char c) {
        s += c;
        int curr = get_link(last);
        int c_idx = c - 'a';

        if (!tree[curr].next[c_idx]) {
            int new_node = sz++;
            tree[new_node].len = tree[curr].len + 2;

            if (tree[new_node].len == 1) tree[new_node].link = 0;
            else {
                int fail = get_link(tree[curr].link);
                tree[new_node].link = tree[fail].next[c_idx];
            }
            tree[curr].next[c_idx] = new_node;
        }

        last = tree[curr].next[c_idx];
        tree[last].cnt++;
    }

    void count_occurrences() {
        for (int i = sz - 1; i >= 2; i--)
            tree[tree[i].link].cnt += tree[i].cnt;
        
    }

    int get_distinct_count() {
        return sz - 2;
    }
};
