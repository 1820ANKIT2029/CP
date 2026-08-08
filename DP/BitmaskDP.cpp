#include <bits/stdc++.h>
using namespace std;

/* 
    Bitmask DP
    => a technique used to solve problems involving subsets of a set, 
    where the state can be represented as a bitmask. 
    Each bit in the mask represents whether an element is included in the subset or not.

    time: O(2^n * n)
    space: O(2^n)

    n = number of elements in the set
    int N = 1 << n; // total subsets
    mask & (1<<i) // is i in set?
    mask | (1<<i) // add i
    mask ^ (1<<i) // remove i
    mask & ~(1<<i) // remove i
    __builtin_popcount(mask) // size of set
    int lsb = mask & -mask; // lowest set bit
    Iterate submask of mask: for(int sub=mask; sub; sub=(sub-1) & mask)
*/

/*
    Variant 1: Assignment DP

    Q: Given a cost matrix of size n x n, find the minimum cost to assign n jobs to n workers 
       such that each worker is assigned exactly one job and each job is assigned to exactly one worker.

    State: dp[mask] = min cost to do jobs set = mask. k = popcount(mask) = next person index.
    Recurrence: dp[mask | 1<<j] = min(dp[mask | 1<<j], dp[mask] + cost[k][j]) for j not in mask.
*/

int solve(int n, vector<vector<int>>& cost) { // O(n * 2^n)
    int N = 1 << n;
    vector<int> dp(N, INT_MAX);
    dp[0] = 0; // no jobs assigned, cost is 0

    for (int mask = 0; mask < N; ++mask) {
        int k = __builtin_popcount(mask); // next worker index
        for (int j = 0; j < n; ++j) {
            if (!(mask & (1 << j))) { // if job j is not assigned
                int new_mask = mask | (1 << j);
                dp[new_mask] = min(dp[new_mask], dp[mask] + cost[k][j]);
            }
        }
    }

    return dp[N - 1]; // all jobs assigned
}

/*
    Variant 2: Pairing / Matching DP
    Q: Count the number of ways to pair up n people (n is even) such that 
    each person is paired with exactly one other person.

    state: dp[mask] = number of ways to pair up people in set = mask.
    recurrence: dp[mask] = sum(dp[mask ^ (1<<i) ^ (1<<j)]) for all i, j in mask, i < j.
*/

int solve(int n) {
    int N = 1 << n;
    vector<int> dp(N, 0);
    dp[0] = 1; // base case: no people left to pair

    for (int mask = 0; mask < N; ++mask) {
        int first_unpaired = -1;
        for (int i = 0; i < n; ++i) {
            if (!(mask & (1 << i))) { // if person i is unpaired
                first_unpaired = i;
                break;
            }
        }
        if (first_unpaired == -1) continue; // all paired

        for (int j = first_unpaired + 1; j < n; ++j) {
            if (!(mask & (1 << j))) { // if person j is unpaired
                int new_mask = mask | (1 << first_unpaired) | (1 << j);
                dp[new_mask] += dp[mask];
            }
        }
    }

    return dp[N - 1]; // all people paired
}

/*
    Variant 3: TSP Family

    Q: Given a distance matrix of size n x n, find the minimum cost to visit all cities 
       starting and ending at city 0 (Traveling Salesman Problem).
    Q: Given a distance matrix of size n x n, find the minimum cost to visit all cities 
       starting at city 0 and ending at city n-1 (Hamiltonian Path).
    
    state: dp[mask][i] = min cost to visit cities in set = mask, ending at city i.
    recurrence: dp[mask | (1<<j)][j] = min(dp[mask | (1<<j)][j], dp[mask][i] + dist[i][j])
    base case: dp[1<<0][0] = 0 (starting at city 0)
    final answer: min(dp[(1<<n)-1][i] + dist[i][0]) for all i (returning to city 0)
*/

int solve(int n, vector<vector<int>>& dist) { // O(n^2 * 2^n)
    int N = 1 << n;
    vector<vector<int>> dp(N, vector<int>(n, INT_MAX));
    dp[1][0] = 0; // starting at city 0

    for (int mask = 1; mask < N; ++mask) {
        for (int i = 0; i < n; ++i) {
            if (!(mask & (1 << i))) continue; // if city i is not in mask
            for (int j = 0; j < n; ++j) {
                if (mask & (1 << j)) continue; // if city j is already in mask
                int new_mask = mask | (1 << j);
                dp[new_mask][j] = min(dp[new_mask][j], dp[mask][i] + dist[i][j]);
            }
        }
    }

    int ans = INT_MAX;
    for (int i = 1; i < n; ++i) {
        ans = min(ans, dp[N - 1][i] + dist[i][0]); // returning to city 0
    }

    return ans;
}

// Shortest Superstring: Find the shortest string that contains all given strings as substrings.
int solve(int n, vector<string>& words) { // O(n^2 * 2^n)
    int N = 1 << n;
    vector<vector<int>> overlap(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            int m = min(words[i].size(), words[j].size());
            for (int k = m; k >= 0; --k) {
                if (words[i].substr(words[i].size() - k) == words[j].substr(0, k)) {
                    overlap[i][j] = k;
                    break;
                }
            }
        }
    }

    vector<vector<int>> dp(N, vector<int>(n, INT_MAX));
    vector<vector<int>> parent(N, vector<int>(n, -1));
    for (int i = 0; i < n; ++i) dp[1 << i][i] = words[i].size();

    for (int mask = 1; mask < N; ++mask) {
        for (int last = 0; last < n; ++last) {
            if (!(mask & (1 << last))) continue;
            for (int next = 0; next < n; ++next) {
                if (mask & (1 << next)) continue;
                int new_mask = mask | (1 << next);
                int cost = dp[mask][last] + words[next].size() - overlap[last][next];
                if (cost < dp[new_mask][next]) {
                    dp[new_mask][next] = cost;
                    parent[new_mask][next] = last;
                }
            }
        }
    }

    int min_len = INT_MAX, last_index = -1;
    for (int i = 0; i < n; ++i) {
        if (dp[N - 1][i] < min_len) {
            min_len = dp[N - 1][i];
            last_index = i;
        }
    }

    string superstring;
    int mask = N - 1;
    while (last_index != -1) {
        superstring = words[last_index] + superstring;
        int prev_index = parent[mask][last_index];
        if (prev_index != -1) {
            superstring = superstring.substr(overlap[prev_index][last_index]);
        }
        mask ^= (1 << last_index);
        last_index = prev_index;
    }
    return superstring;
}

/* 
    Variant 4: Set Cover / Universe DP
    Q: Given a universe of elements and a collection of sets, find the minimum number of sets 
       needed to cover all elements in the universe.

    state: dp[mask] = min number of sets to cover elements in set = mask.
    recurrence: dp[mask | set] = min(dp[mask | set], dp[mask] + 1) for each set in the collection.
    base case: dp[0] = 0 (no elements to cover)
    final answer: dp[(1<<n)-1] (all elements covered)
*/

int solve(int n, vector<vector<int>>& sets) { // O(m * 2^n)
    int N = 1 << n;
    vector<int> dp(N, INT_MAX);
    dp[0] = 0; // no elements to cover

    for (const auto& set : sets) {
        int set_mask = 0;
        for (int elem : set) {
            set_mask |= (1 << elem);
        }
        for (int mask = 0; mask < N; ++mask) {
            if (dp[mask] == INT_MAX) continue; // skip unreachable states
            int new_mask = mask | set_mask;
            dp[new_mask] = min(dp[new_mask], dp[mask] + 1);
        }
    }

    return dp[N - 1]; // all elements covered
}

/*
    Variant 5: Partition / Subset Property DP
    Q: Partition to K equal sum subsets

    state: dp[mask] = true if subset represented by mask can be partitioned into subsets with equal sum.
    recurrence: dp[mask] = true if there exists a submask of mask such that
                sum(submask) == target and dp[mask ^ submask] is true.
    base case: dp[0] = true (empty set can be partitioned)
    final answer: dp[(1<<n)-1] (all elements covered)
*/

int solve(int n, vector<int>& nums, int k) { // O(k * 2^n)
    int sum = accumulate(nums.begin(), nums.end(), 0);
    if (sum % k != 0) return false;
    int target = sum / k;
    int N = 1 << n;
    vector<bool> dp(N, false);
    vector<int> subset_sum(N, 0);
    dp[0] = true;

    for (int mask = 1; mask < N; ++mask) {
        for (int i = 0; i < n; ++i) {
            if (!(mask & (1 << i))) continue;
            
            int prev_mask = mask ^ (1 << i);
            if (dp[prev_mask] && subset_sum[prev_mask] + nums[i] <= target) {
                dp[mask] = true;
                subset_sum[mask] = (subset_sum[prev_mask] + nums[i]) % target;
                break;
            }
        }
    }

    return dp[N - 1];
}

/*
    Variant 6: SOS DP - Sum Over Subsets
    Q: Given a fixed array A of 2^n integers, we need to calculate 
        ∀ x function F(x) = Sum of all A[i] such that x&i = i, i.e., i is a subset of x.

    state: dp[mask] = sum of A[i] for all i that are subsets of mask.
    recurrence: dp[mask] = dp[mask] + dp[mask ^ (1 << j)] for all j such that (mask & (1 << j)) != 0.
    base case: dp[mask] = A[mask] for all mask.
    final answer: dp[mask] for all mask.
*/

int solve(int n, vector<int>& A) { // O(n * 2^n)
    int N = 1 << n;
    vector<int> dp(A); // initialize dp with A

    for (int j = 0; j < n; ++j) {
        for (int mask = 0; mask < N; ++mask) {
            if (mask & (1 << j)) {
                dp[mask] += dp[mask ^ (1 << j)];
            }
        }
    }

    return dp; // dp[mask] contains the sum of all A[i] such that i is a subset of mask
}