#include <bits/stdc++.h>

using namespace std;

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