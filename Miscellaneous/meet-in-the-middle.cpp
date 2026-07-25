#include <bits/stdc++.h>
using namespace std;

using ll = long long;

/*
    meet in the middle
    n = 40
    Time complexity: O(2^(n/2) * log(2^(n/2)))
*/

vector<ll> generateSubsetSums(const vector<ll>& arr) {
    int n = arr.size();
    vector<ll> sums;

    for (int mask = 0; mask < (1 << n); mask++) {
        ll sum = 0;
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i))
                sum += arr[i];
        }
        sums.push_back(sum);
    }

    return sums;
}

int main() {

    int n;
    cin >> n;

    vector<ll> a(n);

    for (auto &x : a)
        cin >> x;

    int mid = n / 2;

    vector<ll> left(a.begin(), a.begin() + mid);
    vector<ll> right(a.begin() + mid, a.end());

    vector<ll> leftSum = generateSubsetSums(left);
    vector<ll> rightSum = generateSubsetSums(right);

    sort(rightSum.begin(), rightSum.end());

    // Example:
    // Count subset sums equal to target

    ll target;
    cin >> target;

    ll ans = 0;

    for (ll x : leftSum) {
        auto l = lower_bound(rightSum.begin(), rightSum.end(), target - x);
        auto r = upper_bound(rightSum.begin(), rightSum.end(), target - x);
        ans += (r - l);
    }

    cout << ans;
}