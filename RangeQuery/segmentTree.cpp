#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<ll> vl;
typedef pair<ll, ll> pll;

#define Left(p) ((p)<<1)+1
#define Right(p) ((p)<<1)+2

const ll MOD = 1e9 + 7;

/*
    operation based segment tree
    eg: sum, min, max, gcd, etc
*/
class segmentTreeSum {
    /*
    Segment Tree Sum version
    
    */

    vl sum;
    int size;

    void build(int p, int L, int R, vl& a){
        if(L == R){
            if(L < (int)a.size()) sum[p] = a[L]; 
            return;
        }

        int mid = L + ((R-L)>>1);
        build(Left(p), L, mid, a);
        build(Right(p), mid+1, R, a);

        sum[p] = sum[Left(p)] + sum[Right(p)];
    }

    void set(int p, int L, int R, int ind, int v){
        if(L == R) {sum[p] = v; return;}

        int mid = L + ((R-L)>>1);
        if(ind <= mid) set(Left(p), L, mid, ind, v);
        else set(Right(p), mid+1, R, ind, v);

        sum[p] = sum[Left(p)] + sum[Right(p)];
    }

    /*
    L, R, l, r

    case 1: bounded 
        l                 r
        |-----------------|
           |--------------|
           L              R
    case 2: unbounded
        l                 r
        |-----------------|
                            |------|
                            L      R
        OR
                  l                 r
                  |-----------------|
        |------|
        L      R
    case 3: some bounded and some unbounded
                l             r
                |-------------|
        |-----------------------|
        L                       R
    */
    long long rangeSum(int p, int L, int R, int l, int r){
        // 3 case -> to get sum
        if(L>=l && R<=r) return sum[p];        // case 1: bounded
        if(R < l || L > r) return 0;           // case 2: unbound

        int mid = L + ((R-L)>>1);

        // case 3: some portion bounded and some unbound
        return rangeSum(Left(p), L, mid, l, r) + rangeSum(Right(p), mid+1, R, l, r);
    }

public:
    segmentTreeSum(int n){
        size = 1;
        while(size < n) size *= 2;
        sum.assign(2*size, 0LL);
    }

    segmentTreeSum(vl& a) : segmentTreeSum((int)a.size()) {
        build(0, 0, size-1, a);
    }

    void set(int ind, int v){ set(0, 0, size-1, ind, v); }
    long long rangeSum(int l, int r){ return rangeSum(0, 0, size-1, l, r);}
};

class segmentTreeMin {
    /*
    segment Tree Minimum version
    */

    int size;
    vl minArr;

    void build(int p, int L, int R, vl& a){
        if(L == R){
            if(L < (int)a.size()) minArr[p] = a[L];
            return;
        }

        int mid = L + ((R-L)>>1);
        build(Left(p), L, mid, a);
        build(Right(p), mid+1, R, a);

        minArr[p] = min(minArr[Left(p)], minArr[Right(p)]);
    }

    void set(int p, int L, int R, int ind, int v){
        if(L == R) {minArr[p] = v; return;}

        int mid = L + ((R-L)>>1);
        if(ind <= mid) set(Left(p), L, mid, ind, v);
        else set(Right(p), mid+1, R, ind, v);

        minArr[p] = min(minArr[Left(p)], minArr[Right(p)]);
    }

    long long rangeMin(int p, int L, int R, int l, int r){
        // 3 case -> to get sum
        if(L >= l && R<=r) return minArr[p];        // case 1: bounded
        if(R < l || L > r) return INT64_MAX;        // case 2: unbound

        int mid = L + ((R-L)>>1);

        // case 3: some portion bounded and some unbound
        return min(rangeMin(Left(p), L, mid, l, r), rangeMin(Right(p), mid+1, R, l, r));
    }

public:
    segmentTreeMin(int n){
        size = 1;
        while(size < n) size *= 2;
        minArr.assign(size * 2, INT64_MAX);
    }

    segmentTreeMin(vl &a) : segmentTreeMin((int)a.size()){
        build(0, 0, size-1, a);
    }

    void set(int ind, int v){
        set(0, 0, size-1, ind, v);
    }

    long long rangeMin(int l, int r){
        return rangeMin(0, 0, size-1, l, r);
    }
};

// good way
class segmentTreeMinCount {
    /*
    segment Tree Minimum and Minimum count version
    */
public:
    struct item {
        int m, c;
    };

private:
    int size;
    vector<item> values;

    item NEUTRAL_ELEMENT = {INT_MAX, 0};

    item merge(item a, item b) {
        if(a.m < b.m) return a;
        if(a.m > b.m) return b;

        return {a.m, a.c + b.c};
    }

    item single(int v){
        return {v, 1};
    }

    void build(int p, int L, int R, vl& a){
        // single operation
        if(L == R){
            if(L < (int)a.size()) values[p] = single(a[L]);
            return;
        }

        int mid = L + ((R-L)>>1);
        build(Left(p), L, mid, a);
        build(Right(p), mid+1, R, a);

        // merge
        values[p] = merge(values[Left(p)], values[Right(p)]);
    }

    void set(int p, int L, int R, int ind, int v){
        // single operation
        if(L == R) {values[p] = single(v); return;}

        int mid = L + ((R-L)>>1);
        if(ind <= mid) set(Left(p), L, mid, ind, v);
        else set(Right(p), mid+1, R, ind, v);

        // merge
        values[p] = merge(values[Left(p)], values[Right(p)]);
    }

    item calc(int p, int L, int R, int l, int r){
        // 3 case -> to get sum
        if(L >= l && R<=r) return values[p];              // case 1: bounded
        if(R < l || L > r) return NEUTRAL_ELEMENT;        // case 2: unbound

        int mid = L + ((R-L)>>1);

        // case 3: some portion bounded and some unbound
        item a = calc(Left(p), L, mid, l, r);
        item b = calc(Right(p), mid+1, R, l, r);

        return merge(a, b);
    }

public:
    segmentTreeMinCount(int n){
        size = 1;
        while(size < n) size *= 2;
        values.resize(size * 2);
    }

    segmentTreeMinCount(vl &a) : segmentTreeMinCount((int)a.size()){
        build(0, 0, size-1, a);
    }

    void set(int ind, int v){
        set(0, 0, size-1, ind, v);
    }

    item calc(int l, int r){
        return calc(0, 0, size-1, l, r);
    }
};

/*
    other type
*/

class segmentTreeMaxSegSum {
    /*
    segment Tree Max Sum Segment version
    */
public:
    struct item {
        ll seg, pref, suf, sum;
    };

private:
    int size;
    vector<item> values;

    item NEUTRAL_ELEMENT = {0, 0, 0, 0};

    item merge(item a, item b) {
        return {
            max(a.seg, max(b.seg, a.suf + b.pref)),
            max(a.pref, a.sum + b.pref),
            max(b.suf, b.sum + a.suf),
            a.sum + b.sum
        };
    }

    item single(int v){
        if(v > 0) return {v, v, v, v};
        return {0, 0, 0, v};
    }

    void build(int p, int L, int R, vl& a){
        // single operation
        if(L == R){
            if(L < (int)a.size()) values[p] = single(a[L]);
            return;
        }

        int mid = L + ((R-L)>>1);
        build(Left(p), L, mid, a);
        build(Right(p), mid+1, R, a);

        // merge
        values[p] = merge(values[Left(p)], values[Right(p)]);
    }

    void set(int p, int L, int R, int ind, int v){
        // single operation
        if(L == R) {values[p] = single(v); return;}

        int mid = L + ((R-L)>>1);
        if(ind <= mid) set(Left(p), L, mid, ind, v);
        else set(Right(p), mid+1, R, ind, v);

        // merge
        values[p] = merge(values[Left(p)], values[Right(p)]);
    }

    item calc(int p, int L, int R, int l, int r){
        // 3 case -> to get sum
        if(L >= l && R<=r) return values[p];              // case 1: bounded
        if(R < l || L > r) return NEUTRAL_ELEMENT;        // case 2: unbound

        int mid = L + ((R-L)>>1);

        // case 3: some portion bounded and some unbound
        item a = calc(Left(p), L, mid, l, r);
        item b = calc(Right(p), mid+1, R, l, r);

        return merge(a, b);
    }

public:
    segmentTreeMaxSegSum(int n){
        size = 1;
        while(size < n) size *= 2;
        values.resize(size * 2);
    }

    segmentTreeMaxSegSum(vl &a) : segmentTreeMaxSegSum((int)a.size()){
        build(0, 0, size-1, a);
    }

    void set(int ind, int v){
        set(0, 0, size-1, ind, v);
    }

    item calc(int l, int r){
        return calc(0, 0, size-1, l, r);
    }
};

class segmentTreeKthOne {
    /*
    segment Tree Kth one
    */
public:
    typedef int item;

private:
    int size;
    vector<item> values;

    item NEUTRAL_ELEMENT = 0;

    item merge(item a, item b) {
        return a + b;
    }

    item single(int v){
        return v;
    }

    void build(int p, int L, int R, vl& a){
        // single operation
        if(L == R){
            if(L < (int)a.size()) values[p] = single(a[L]);
            return;
        }

        int mid = L + ((R-L)>>1);
        build(Left(p), L, mid, a);
        build(Right(p), mid+1, R, a);

        // merge
        values[p] = merge(values[Left(p)], values[Right(p)]);
    }

    void set(int p, int L, int R, int ind, int v){
        // single operation
        if(L == R) {values[p] = single(v); return;}

        int mid = L + ((R-L)>>1);
        if(ind <= mid) set(Left(p), L, mid, ind, v);
        else set(Right(p), mid+1, R, ind, v);

        // merge
        values[p] = merge(values[Left(p)], values[Right(p)]);
    }

    item calc(int p, int L, int R, int l, int r){
        // 3 case -> to get sum
        if(L >= l && R<=r) return values[p];              // case 1: bounded
        if(R < l || L > r) return NEUTRAL_ELEMENT;        // case 2: unbound

        int mid = L + ((R-L)>>1);

        // case 3: some portion bounded and some unbound
        item a = calc(Left(p), L, mid, l, r);
        item b = calc(Right(p), mid+1, R, l, r);

        return merge(a, b);
    }

    int find(int p, int L, int R, int k){
        if(R == L) return L;

        int mid = L + ((R-L)>>1);

        int sl = values[Left(p)];
        if(k < sl) return find(Left(p), L, mid, k);
        return find(Right(p), mid+1, R, k - sl);
    }

public:
    segmentTreeKthOne(int n){
        size = 1;
        while(size < n) size *= 2;
        values.resize(size * 2);
    }

    segmentTreeKthOne(vl &a) : segmentTreeKthOne((int)a.size()){
        build(0, 0, size-1, a);
    }

    void set(int ind, int v){
        set(0, 0, size-1, ind, v);
    }

    item calc(int l, int r){
        return calc(0, 0, size-1, l, r);
    }

    int find(int k){
        return find(0, 0, size-1, k);
    }
};

/*
    also use in inversion, inverse inversion, etc problem
*/


// range update segment tree
class segmentTreeAddSeg {
    /*
        Segment tree Addition to segment
        Range Update Point Query segment tree
    */

    vl operations;
    int size;

    void build(int p, int L, int R, vl& a){
        if(L == R){
            if(L < (int)a.size()) operations[p] = a[L]; 
            return;
        }

        int mid = L + ((R-L)>>1);
        build(Left(p), L, mid, a);
        build(Right(p), mid+1, R, a);
    }

    void add(int p, int L, int R, int l, int r, int v){
        // 3 case -> to get sum
        if(L >= l && R<=r){operations[p] += v; return;}        // case 1: bounded
        if(R < l || L > r) return;             // case 2: unbound

        int mid = L + ((R-L)>>1);

        // case 3: some portion bounded and some unbound
        add(Left(p), L, mid, l, r, v);
        add(Right(p), mid+1, R, l, r, v);
    }

    ll get(int p, int L, int R, int ind){
        if(L == R) return operations[p];

        int mid = L + ((R-L)>>1);
        ll ans;
        if(ind <= mid) ans = get(Left(p), L, mid, ind);
        else ans = get(Right(p), mid+1, R, ind);

        return ans + operations[p];
    }

public:
    segmentTreeAddSeg(int n){
        size = 1;
        while(size < n) size *= 2;
        operations.assign(2*size, 0LL);
    }

    segmentTreeAddSeg(vl& a) : segmentTreeAddSeg((int)a.size()) {
        build(0, 0, size-1, a);
    }

    void add(int l, int r, int v){ add(0, 0, size-1, l, r, v);}
    ll get(int i) {return get(0, 0, size-1, i);}
};

class segmentTreeAtLeastX {
    vl tree;
    int size;
 
    void build(int p, int L, int R, vl& a){
        if(L == R){
            if(L < (int)a.size()) tree[p] = a[L]; 
            return;
        }
 
        int mid = L + ((R-L)>>1);
        build(Left(p), L, mid, a);
        build(Right(p), mid+1, R, a);
 
        tree[p] = max(tree[Left(p)], tree[Right(p)]);
    }
 
    void pointUpdate(int p, int L, int R, int idx, int v){
        if(L == R){tree[p] = v; return;}
 
        int mid = L + ((R-L)>>1);
        if(idx <= mid) pointUpdate(Left(p), L, mid, idx, v);
        else pointUpdate(Right(p), mid+1, R, idx, v);
 
        tree[p] = max(tree[Left(p)], tree[Right(p)]);
    }
 
    int find(int p, int L, int R, ll v){
        if(tree[p] < v) return -1;
        if(L == R) return L;
 
        int mid = L + ((R-L)>>1);
        if(v <= tree[Left(p)]) return find(Left(p), L, mid, v);
        else return find(Right(p), mid+1, R, v);
    }
 
public:
    segmentTreeAtLeastX(vl &a){
        int n = a.size();
        size = 1;
        while(size < n) size *= 2;
        tree.assign(2*size, INT64_MIN);
        build(0, 0, size-1, a);
    }
 
    void pointUpdate(int idx, ll v){ pointUpdate(0, 0, size-1, idx, v);}
    int find(ll x) {return find(0, 0, size-1, x);}
};

class segmentTreeAtLeastXAndJGreaterL {
    vl tree;
    int size;

    void build(int p, int L, int R, vl& a){
        if(L == R){
            if(L < (int)a.size()) tree[p] = a[L]; 
            return;
        }

        int mid = L + ((R-L)>>1);
        build(Left(p), L, mid, a);
        build(Right(p), mid+1, R, a);

        tree[p] = max(tree[Left(p)], tree[Right(p)]);
    }

    void pointUpdate(int p, int L, int R, int idx, int v){
        if(L == R){tree[p] = v; return;}

        int mid = L + ((R-L)>>1);
        if(idx <= mid) pointUpdate(Left(p), L, mid, idx, v);
        else pointUpdate(Right(p), mid+1, R, idx, v);

        tree[p] = max(tree[Left(p)], tree[Right(p)]);
    }

    int find(int p, int L, int R, ll x, int l){
        if(R < l || tree[p] < x) return -1;

        if(L == R) return L;

        int mid = L + ((R - L) >> 1);

        int res = find(Left(p), L, mid, x, l);
        if(res != -1) return res;

        return find(Right(p), mid+1, R, x, l);
    }


public:
    segmentTreeAtLeastX(vl &a){
        int n = a.size();
        size = 1;
        while(size < n) size *= 2;
        tree.assign(2*size, INT64_MIN);
        build(0, 0, size-1, a);
    }

    void pointUpdate(int idx, ll v){ pointUpdate(0, 0, size-1, idx, v);}
    int find(ll x, int l) {return find(0, 0, size-1, x, l);}
};

class segmentTreeRURM {
    /*
        Segment Tree Range Update Range Minimum
    */

    int size;
    vl operations, mins;

    void add(int p, int L, int R, int l, int r, int val){
        if(L > r || R < l) return;
        if(L >= l && R<=r){operations[p] += val; mins[p] += val; return;}

        int mid = L + ((R-L)>>1);
        add(Left(p), L, mid, l, r, val);
        add(Right(p), mid+1, R, l, r, val);

        mins[p] = min(mins[Left(p)], mins[Right(p)]) + operations[p];
    }

    ll rangeMin(int p, int L, int R, int l, int r){
        if(L > r || R < l) return LLONG_MAX;
        if(L >= l && R<=r) return mins[p];

        int mid = L + ((R-L)>>1);
        ll a = rangeMin(Left(p), L, mid, l, r);
        ll b = rangeMin(Right(p), mid+1, R, l, r);

        return min(a, b) + operations[p];
    }

public:
    segmentTreeRURM(int n){
        size = 1;
        while(size < n) size *= 2;
        operations.assign(2 * size, 0LL);
        mins.assign(2 * size, 0LL);
    }

    void add(int l, int r, int v){add(0, 0, size-1, l, r, v);}
    ll rangeMin(int l, int r){return rangeMin(0, 0, size-1, l, r);}
};

int main(){
    segmentTreeRURM st(8);

    // add +5 to range [2, 6]
    st.add(2, 6, 5);

    // add -3 to range [4, 7]
    st.add(4, 7, -3);

    cout << st.rangeMin(0, 7) << '\n'; // expected: 0
    cout << st.rangeMin(2, 3) << '\n'; // expected: 5
    cout << st.rangeMin(4, 6) << '\n'; // expected: 2
    cout << st.rangeMin(7, 7) << '\n'; // expected: -3
}

class segmentTreeRURQ {
    /*
        Segment Tree Range Update Range Query
        two type of operation in one

        eg: this involve multiple * and addition +
        modify: multiple each values in (l, r) range, hence modify_op
        query: add each value in (l, r) range, hence query_op

        another example: bitwise OR and AND operation
        modify: Bitwise OR each values in (l, r) range, hence modify_op
        query: Bitwise AND each value in (l, r) range, hence query_op
    */

    int size;
    vl operations, values;

    ll NEUTRAL_ELEMENT = 0;

    ll modify_op(ll a, ll b){
        return (a * b) % MOD;
    }

    ll query_op(ll a, ll b){
        return (a + b) % MOD;
    }

    void apply_mod_op(ll &a, ll b){
        a = modify_op(a, b);
    }

    void modify(int p, int L, int R, int l, int r, int val){
        if(L > r || R < l) return;
        if(L >= l && R<=r){
            apply_mod_op(operations[p], val);
            apply_mod_op(values[p], val);
            return;
        }

        int mid = L + ((R-L)>>1);
        modify(Left(p), L, mid, l, r, val);
        modify(Right(p), mid+1, R, l, r, val);

        values[p] = query_op(values[Left(p)], values[Right(p)]);
        apply_mod_op(values[p], operations[p]);
    }

    ll query(int p, int L, int R, int l, int r){
        if(L > r || R < l) return NEUTRAL_ELEMENT;
        if(L >= l && R<=r) return values[p];

        int mid = L + ((R-L)>>1);
        ll a = query(Left(p), L, mid, l, r);
        ll b = query(Right(p), mid+1, R, l, r);
        ll ans = query_op(a, b);
        apply_mod_op(ans, operations[p]);
        return ans;
    }

    // void build(int p, int L, int R){
    //     if(L == R) {values[p] = 1; return;}

    //     int mid = L + ((R-L)>>1);
    //     build(Left(p), L, mid);
    //     build(Right(p), mid+1, R);

    //     values[p] = query_op(values[Left(p)], values[Right(p)]);
    // }

public:
    segmentTreeRURQ(int n){
        size = 1;
        while(size < n) size *= 2;
        operations.assign(2 * size, 1LL);
        values.assign(2 * size, 0LL);
        // build(0, 0, size-1);
    }

    void modify(int l, int r, int v){modify(0, 0, size-1, l, r, v);}
    ll query(int l, int r){return query(0, 0, size-1, l, r);}
};

class segmentTreeRURQLazy {
    // Segment Tree Range Update Range Query Lazy Propagation

    int size;
    vl operations, values;

    ll NEUTRAL_ELEMENT = LLONG_MAX;
    ll NO_OPERATION = LLONG_MAX - 1;

    ll modify_op(ll a, ll b){
        if(b == NO_OPERATION) return a;
        return b;
    }

    ll query_op(ll a, ll b){
        return min(a, b);
    }

    void apply_mod_op(ll &a, ll b){
        a = modify_op(a, b);
    }

    void propagate(int p, int L, int R){
        if(L == R) return;

        int mid = L +  ((R-L)>>1);
        apply_mod_op(operations[Left(p)], operations[p]);
        apply_mod_op(values[Left(p)], operations[p]);
        apply_mod_op(operations[Right(p)], operations[p]);
        apply_mod_op(values[Right(p)], operations[p]);
        operations[p] = NO_OPERATION;
    }

    void modify(int p, int L, int R, int l, int r, int val){
        propagate(p, L, R);
        if(L > r || R < l) return;
        if(L >= l && R<=r){
            apply_mod_op(operations[p], val);
            apply_mod_op(values[p], val);
            return;
        }

        int mid = L + ((R-L)>>1);
        modify(Left(p), L, mid, l, r, val);
        modify(Right(p), mid+1, R, l, r, val);

        values[p] = query_op(values[Left(p)], values[Right(p)]);
        apply_mod_op(values[p], operations[p]);
    }

    ll query(int p, int L, int R, int l, int r){
        propagate(p, L, R);
        if(L > r || R < l) return NEUTRAL_ELEMENT;
        if(L >= l && R<=r) return values[p];

        int mid = L + ((R-L)>>1);
        ll a = query(Left(p), L, mid, l, r);
        ll b = query(Right(p), mid+1, R, l, r);
        ll ans = query_op(a, b);
        apply_mod_op(ans, operations[p]);
        return ans;
    }

public:
    segmentTreeRURQLazy(int n){
        size = 1;
        while(size < n) size *= 2;
        operations.assign(2 * size, NO_OPERATION);
        values.assign(2 * size, 0LL);
    }

    void modify(int l, int r, int v){modify(0, 0, size-1, l, r, v);}
    ll query(int l, int r){return query(0, 0, size-1, l, r);}
};

/* 
    example usage of segmentTreeRURQLazy
    1. Range Minimum Query + Range Assignment (Current Setup)
        NEUTRAL_ELEMENT: LLONG_MAX
        NO_OPERATION: LLONG_MAX - 1 (or any impossible assignment value)
        query_op(a, b): return min(a, b);
        modify_op(a, b): return b; (Assigns the new value, overwriting the old).
    2. Range Maximum Query + Range Addition
        NEUTRAL_ELEMENT: LLONG_MIN
        NO_OPERATION: 0 (Adding 0 changes nothing)
        query_op(a, b): return max(a, b);
        modify_op(a, b): return a + b; (Adds the lazy value to the current max).
    3. Range GCD Query + Range Assignment
        NEUTRAL_ELEMENT: 0 (Since GCD(x, 0) = x)
        NO_OPERATION: -1 (Assuming valid array elements are >= 0)
        query_op(a, b): return gcd(a, b);
        modify_op(a, b): return b;
    4. Bitwise OR Query + Range Assignment
        NEUTRAL_ELEMENT: 0 (Because X | 0 = X)
        NO_OPERATION: -1 (Assuming array values are non-negative. If values can be negative, use a distinct flag like LLONG_MAX)
        query_op(a, b): return a | b;
        modify_op(a, b): return b; (Overwrites the existing bits)
    5. Bitwise AND Query + Range Bitwise OR Update
        NEUTRAL_ELEMENT: ~0LL or LLONG_MAX (All bits set to 1, because X & 1...1 = X)
        NO_OPERATION: 0 (OR-ing with 0 changes nothing)
        query_op(a, b): return a & b;
        modify_op(a, b): return a | b; (Applies the new bits to the existing bits)
    6. Range Sum Query + Range Addition (Requires Segment Length)
        NEUTRAL_ELEMENT: 0
        NO_OPERATION: 0
        query_op(a, b): return a + b;
        ll modify_op(ll a, ll b, ll len) {
            if(b == NO_OPERATION) return a;
            return a + (b * len); 
        }
        void apply_mod_op(ll &a, ll b, ll len){
            a = modify_op(a, b, len);
        }   
    7. Range Sum Query + Range Assignment (Requires Segment Length)
        NEUTRAL_ELEMENT: 0
        NO_OPERATION: LLONG_MAX
        query_op(a, b): return a + b;
        modify_op(a, b, len): return b * len; (Completely ignores a and calculates the new sum)
*/

