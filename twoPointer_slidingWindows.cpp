#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl '\n'

/*
========================================================
                TWO POINTER TECHNIQUE
========================================================

ROOT IDEA:
-----------
Instead of checking all pairs/subarrays using O(N²),
we maintain TWO moving pointers.

Usually:
l = left pointer
r = right pointer

We intelligently move them based on conditions.

COMMON USES:
------------
1. Pair sum in sorted array
2. Remove duplicates
3. Merge arrays
4. Longest/Shortest valid subarray
5. Sliding Window problems
6. Subarray conditions
7. String window problems

IMPORTANT:
------------
Most sliding window problems are actually
a special case of two pointers.

========================================================
*/

/*
========================================================
            TEMPLATE 1: BASIC TWO POINTER
========================================================

WHEN TO USE:
-------------
- Sorted array
- Pair/triplet problems
- Opposite direction movement

TIME:
------
O(N)

EXAMPLE:
---------
Find if pair sum exists
*/

bool pairSum(vector<int>& a, int target) {
    int l = 0;
    int r = a.size() - 1;
    sort(a.begin(), a.end());

    while(l < r) {
        int sum = a[l] + a[r];
        if(sum == target) return true;
        else if(sum < target) l++;
        else r--;
    }

    return false;
}

/*
========================================================
        TEMPLATE 2: SAME DIRECTION TWO POINTER
========================================================

WHEN TO USE:
-------------
- Subarrays
- Dynamic windows
- Expand/shrink window

PATTERN:
---------
Move r to expand
Move l to shrink

This is foundation of sliding window.

========================================================
*/

void sameDirectionTemplate(vector<int>& a) {
    int n = a.size();
    int l = 0;

    for(int r = 0; r < n; r++) {

        // EXPAND WINDOW USING r

        while(/* invalid condition */) {

            // SHRINK WINDOW

            l++;
        }

        // WINDOW [l...r] IS VALID
    }
}

/*
========================================================
        TEMPLATE 3: FIXED SIZE SLIDING WINDOW
========================================================

WINDOW SIZE = K

WHEN TO USE:
-------------
- Fixed length subarray
- Maximum/minimum sum of size K
- Average of K elements

TIME:
------
O(N)

========================================================
*/

ll fixedWindowSum(vector<int>& a, int k) {
    int n = a.size();
    ll windowSum = 0;

    // FIRST WINDOW
    for(int i = 0; i < k; i++) {
        windowSum += a[i];
    }

    ll ans = windowSum;

    // SLIDE WINDOW
    for(int i = k; i < n; i++) {

        // ADD NEW ELEMENT
        windowSum += a[i];

        // REMOVE OLD ELEMENT
        windowSum -= a[i - k];

        ans = max(ans, windowSum);
    }

    return ans;
}

/*
========================================================
    TEMPLATE 4: VARIABLE SIZE SLIDING WINDOW
========================================================

MOST IMPORTANT TEMPLATE

WHEN TO USE:
-------------
- Longest valid subarray
- Smallest valid subarray
- At most K distinct
- Sum <= K
- Sum >= K
- String window problems

CORE IDEA:
------------
Expand with r
Shrink with l until valid

========================================================
*/

int variableWindow(vector<int>& a, int k) {
    int n = a.size(), l = 0, ans = 0;
    long long sum = 0;

    for(int r = 0; r < n; r++) {
        // EXPAND WINDOW
        sum += a[r];

        // SHRINK WINDOW
        while(sum > k) {
            sum -= a[l];
            l++;
        }

        // VALID WINDOW
        ans = max(ans, r - l + 1);
    }

    return ans;
}



/*
========================================================
    TEMPLATE 5: LONGEST SUBARRAY WITH SUM <= K
========================================================
*/

int longestSubarrayAtMostK(vector<int>& a, int k) {
    int l = 0, ans = 0;
    long long sum = 0;

    for(int r = 0; r < a.size(); r++) {
        sum += a[r];
        while(sum > k) {
            sum -= a[l];
            l++;
        }
        ans = max(ans, r - l + 1);
    }

    return ans;
}

/*
========================================================
    TEMPLATE 6: SMALLEST SUBARRAY SUM >= K
========================================================
*/

int smallestSubarrayAtLeastK(vector<int>& a, int k) {
    int l = 0, ans = INT_MAX;
    long long sum = 0;

    for(int r = 0; r < a.size(); r++) {
        sum += a[r];
        while(sum >= k) {
            ans = min(ans, r - l + 1);
            sum -= a[l];
            l++;
        }
    }

    return (ans == INT_MAX ? -1 : ans);
}

/*
========================================================
        TEMPLATE 7: AT MOST K DISTINCT
========================================================

VERY IMPORTANT STRING/ARRAY TEMPLATE

USED IN:
---------
- Longest substring
- Distinct character problems
- Frequency problems

========================================================
*/

int atMostKDistinct(vector<int>& a, int k) {
    unordered_map<int,int> freq;
    int l = 0, ans = 0;

    for(int r = 0; r < a.size(); r++) {
        freq[a[r]]++;
        while(freq.size() > k) {
            freq[a[l]]--;
            if(freq[a[l]] == 0) { freq.erase(a[l]); }
            l++;
        }

        ans = max(ans, r - l + 1);
    }

    return ans;
}



/*
========================================================
        TEMPLATE 8: EXACTLY K DISTINCT
========================================================

TRICK:
-------
exactly(K) =
atMost(K) - atMost(K-1)

VERY IMPORTANT INTERVIEW PATTERN

========================================================
*/

int atMost(vector<int>& a, int k) {
    unordered_map<int,int> freq;
    int l = 0, ans = 0;

    for(int r = 0; r < a.size(); r++) {
        freq[a[r]]++;
        while(freq.size() > k) {
            freq[a[l]]--;
            if(freq[a[l]] == 0) { freq.erase(a[l]); }
            l++;
        }

        ans += (r - l + 1);
    }

    return ans;
}

int exactlyKDistinct(vector<int>& a, int k) {
    return atMost(a, k) - atMost(a, k - 1);
}

/*
========================================================
        TEMPLATE 9: LONGEST UNIQUE SUBSTRING
========================================================

VERY IMPORTANT STRING TEMPLATE

========================================================
*/

int longestUniqueSubstring(string s) {
    vector<int> freq(256, 0);
    int l = 0, ans = 0;

    for(int r = 0; r < s.size(); r++) {
        freq[s[r]]++;
        while(freq[s[r]] > 1) {
            freq[s[l]]--;
            l++;
        }

        ans = max(ans, r - l + 1);
    }

    return ans;
}

/*
========================================================
        TEMPLATE 10: MINIMUM WINDOW SUBSTRING
========================================================

HARD + VERY IMPORTANT

PATTERN:
---------
Expand until valid
Shrink while still valid

========================================================
*/

string minWindow(string s, string t) {
    vector<int> need(256, 0);
    for(char c : t) need[c]++;

    int required = t.size(), l = 0, start = 0, minLen = INT_MAX;

    for(int r = 0; r < s.size(); r++) {
        if(need[s[r]] > 0) required--;
        need[s[r]]--;
        while(required == 0) {
            if(r - l + 1 < minLen) {
                minLen = r - l + 1;
                start = l;
            }

            need[s[l]]++;
            if(need[s[l]] > 0) required++;
            l++;
        }
    }

    return minLen == INT_MAX ? "" : s.substr(start, minLen);
}

/*
========================================================
            MASTER RECOGNITION GUIDE
========================================================

USE TWO POINTER WHEN:
----------------------
1. Array/string contiguous
2. Pair/triplet
3. Sorted array
4. Window/subarray
5. Need O(N)

USE FIXED WINDOW WHEN:
-----------------------
- Window size constant

USE VARIABLE WINDOW WHEN:
--------------------------
- Need longest/smallest valid subarray

COMMON SIGNAL WORDS:
--------------------
- longest substring
- shortest subarray
- at most K
- exactly K
- consecutive
- contiguous
- window
- subarray
- substring

========================================================
*/