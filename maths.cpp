#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;

int MOD = 1e9 + 7;

long long GCD(long long a, long long b) {
    // recursive
    if(b == 0) return a;
    return GCD(b, a%b);

    // iterative
    /*
    long long tmp;
    while(b) {
        tmp = a;
        a = b;
        b = tmp % b;
    }
    return a;
    */
}

long long LCM(long long a, long long b) {
    return a / GCD(a, b) * b;
}

long long power(int x, int n){
    if(n == 0) return 1;
    long long ans = power(x, n / 2);
    ans *= ans; ans %= MOD;
    if(n % 2 != 0){ ans *= x; ans %= MOD;}
    return ans;
}

long long modInverse(long long n) {
    return power(n, MOD - 2);
}

long long fact(int n){
    long long ans = 1;
    for(int i=1; i<=n; i++){ ans *= i; ans %= MOD;}
    return ans;
}

long long nCr(int n, int r){
    /*
    nCr = n!/(r!*(n-r)!)
    A = n!
    B = r!*(n-r)!
    nCr = (A / B) % MOD
        = (A * B^-1) % MOD
        
        // Fermat's Little Theorem
        // a^(MOD−1) ≡ 1 (mod MOD)
        // a^(MOD−2) ≡ a^−1 (mod MOD)
        
        = (A * B^(MOD - 2)) % MOD
    */
    long long nume = fact(n);
    long long deno = (fact(r) * fact(n-r)) % MOD;

    return (nume * power(deno, MOD-2)) % MOD;
}

long long nPr(int n, int r) {
    /*
    nPr = n!/(n-r)!
    A = n!
    B = (n-r)!
    nPr = (A / B) % MOD
        = (A * B^-1) % MOD
        = (A * B^(MOD - 2)) % MOD
    */
    long long nume = fact(n);
    long long deno = fact(n-r);

    return (nume * power(deno, MOD-2)) % MOD;
}

// FACTORIAL + nCr + nPr
ll fact[MAX + 1];
ll invFact[MAX + 1];

void precomputeFactorial() {
    fact[0] = 1;

    for (int i = 1; i <= MAX; i++) fact[i] = (fact[i - 1] * i) % MOD;
    invFact[MAX] = modInverse(fact[MAX]);
    for (int i = MAX - 1; i >= 0; i--) invFact[i] = (invFact[i + 1] * (i + 1)) % MOD;
}

ll nCr(ll n, ll r) {
    if (r < 0 || r > n) return 0;

    return (((fact[n] * invFact[r]) % MOD) * invFact[n - r]) % MOD;
}

ll nPr(ll n, ll r) {
    if (r < 0 || r > n) return 0;

    return (fact[n] * invFact[n - r]) % MOD;
}

// Pascal's Triangle
const int N = 1000;
ll C[N + 1][N + 1]; // C[n][r] = nCr(n, r)

void buildPascal() {
    for (int n = 0; n <= N; n++) {
        C[n][0] = C[n][n] = 1;

        for (int r = 1; r < n; r++)
            C[n][r] = (C[n - 1][r - 1] + C[n - 1][r]) % MOD;
    }
}

// Catalan Number = (2n)! / ((n + 1)! * n!) = nCr(2n, n) / (n + 1)
ll catalan(int n) {
    return (nCr(2 * n, n) * modInverse(n + 1)) % MOD;
}

/*
    Extended Euclidean Algorithm
    a*x + b*y = gcd(a, b);
    a*s + b*t = gcd(a, b);
*/
long long extEuclid(long long a, long long b, long long &x, long long &y) {      // pass x and y by ref
    long long xx = y = 0;
    long long yy = x = 1;
    long long q, t;
    while (b) {                                    // repeats until b == 0
        q = a / b;
        t = b; b = a%b; a = t;                     // r
        t = xx; xx = x-q*xx; x = t;                // s
        t = yy; yy = y-q*yy; y = t;                // t
    }
    return a;                                      // returns gcd(a, b)
}

long long modInverse(long long A, long long M) {
    long long x, y;
    extEuclid(A, M, x, y);
    return (x % M + M) % M;
}

// Chinese Remainder Theorem
// Returns x such that x = a[i] (mod m[i]) for all i
long long CRT(const vector<long long>& a, const vector<long long>& m) {
    long long M = 1;
    for (long long mod : m) {
        M *= mod;
    }

    long long x = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        long long M_i = M / m[i];
        long long y_i = modInverse(M_i, m[i]);
        
        // Use (__int128) to prevent overflow if M is near 10^18
        long long term = (M_i * y_i) % M;
        term = (term * a[i]) % M;
        x = (x + term) % M;
    }

    return (x % M + M) % M;
}

/*
    Prime Number

    1. bool isPrime(long long n):  
        O(n) : 2..(n-1)
        O(n^0.5) : 2..(n^0.5)
        O(n^0.5 / 2) : 2, 3, 5, 7 .. (n^0.5), only odd number except 2
        O(n^0.5 / ln(n^0.5)):  if list of prime number < n^0.5 known, O(m/ln(m)) number of primes less than m.
        
    2. sieve(n): generate list of prime numbers < n
        O(nlnln(n))

        to cheak if a number is prime: O(nlnln(n) + (n^0.5 / ln(n^0.5)))
*/

bool isPrime(long long n) {
    long long n_sqrt = sqrt(n);
    if(n == 2) return true;
    if(n < 2 || n % 2 == 0) return false;
    
    for(long long i=3; i<=n_sqrt; i+=2) {
        if(n % i == 0) return false;
    }

    return true;
}

// global variable for sieve
long long sieve_size = 10000001;
bitset<10000010> bs;                     // 10^7 limit
vector<long long> primes;

void sieve() {
    bs.set();
    bs[0] = bs[1] = 0;

    for(long long i=2; i<sieve_size; i++) {
        if(bs[i]) {
            for(long long j=i*i; j<sieve_size; j+=i) bs[j] = 0;
            primes.push_back(i);
        }
    }
}

// uses sieve
bool isPrime(long long n) {
    if(n < sieve_size) return bs[n];

    int m = primes.size();
    for(int i=0; i<m && (primes[i]*primes[i] <= n); i++)
        if(n % primes[i] == 0)
            return false;
    return true;
}

vector<long long> primeFactor(long long n) {
    vector<long long> factors;
    int m = primes.size();
    for(int i=0; i<m && (primes[i]*primes[i] <= n); i++) {
        while(n % primes[i] == 0) {
            n /= primes[i];
            factors.push_back(primes[i]);
        }
    }

    if(n != 1) factors.push_back(n);
    return factors;
}

long long CountDivisors(long long n) {
    int m = primes.size();
    long long ans = 1, cnt;
    for(int i=0; i<m && (primes[i]*primes[i] <= n); i++) {
        cnt = 0;
        while(n % primes[i] == 0) {
            n /= primes[i]; cnt++;
        }
        ans *= (cnt + 1);
    }

    if(n != 1) ans *= 2;
    return ans;
}

/*
    Segmented Sieve
    Q: number of prime in [L, R], R < 10^12 , (R-L)<10^6)

    sol:
    step1: Generate all base primes up to sqrt(R) using a standard sieve. 
    step2: Create a boolean array of size R - L + 1 (representing the range).
    step3: Use the base primes to cross out their multiples specifically within the [L, R] window.

    space: O(R^0.5 + (R-L))
    time: O((R-L)lnlnR)
*/

// Returns the number of primes in the range [L, R]
long long count_primes_in_range(long long L, long long R) {
    if (R < 2) return 0;
    long long limit = sqrt(R); sieve()
    vector<char> is_prime(R - L + 1, true);

    if (L == 1) {
        is_prime[0] = false; 
    }

    for (long long p : primes) {
        long long start = (L + p - 1) / p * p;
        start = max(start, p * p);
        for (long long j = start; j <= R; j += p) {
            is_prime[j - L] = false;
        }
    }

    long long prime_count = 0;
    for (long long i = 0; i <= R - L; ++i) {
        if (is_prime[i]) {
            prime_count++;
        }
    }

    return prime_count;
}

// Derangement: number of permutations of n elements with no fixed points
ll derangement(int n) {
    vector<ll> der(n + 1); der[0] = 1;

    if (n >= 1) der[1] = 0;

    for (int i = 2; i <= n; i++) 
        der[i] = ((i - 1) * (der[i - 1] + der[i - 2])) % MOD;

    return der[n];
}

// Stars and Bars: distribute n identical items into k groups
ll starsAndBarsEmpty(int n, int k) {
    return nCr(n + k - 1, k - 1);
}

ll starsAndBarsNonEmpty(int n, int k) {
    if (n < k) return 0;
    return nCr(n - 1, k - 1);
}

// Inclusion-Exclusion Principle
// Count numbers from 1 to n that are divisible by at least one of the given integers in array a
ll inclusionExclusion(vector<int>& a, int n) {
    int m = a.size();

    ll ans = 0;
    for (int mask = 1; mask < (1 << m); mask++) {
        ll lcmValue = 1;
        int bits = 0;
        bool overflow = false;

        for (int i = 0; i < m; i++) {
            if (mask & (1 << i)) {
                bits++;
                lcmValue = LCM(lcmValue, (ll)a[i]);

                if (lcmValue > n) { overflow = true; break; }
            }
        }

        if (overflow) continue;

        ll cnt = n / lcmValue;
        if (bits & 1) ans += cnt;
        else ans -= cnt;
    }

    return ans;
}

// Burnside's Lemma: count distinct objects under group actions
// distinct necklaces example: n beads, k colors, rotations only
ll burnsideNecklace(int n, int k) {
    ll sum = 0;

    for (int rot = 0; rot < n; rot++) {
        int cycles = GCD(rot, n);
        sum += power(k, cycles);
    }

    return sum / n;
}

// Expected Value and Variance
ld expectedValue(vector<ld>& value, vector<ld>& prob) {
    ld ans = 0;
    for (int i = 0; i < value.size(); i++)
        ans += value[i] * prob[i];

    return ans;
}

ld variance(vector<ld>& value, vector<ld>& prob) {
    ld EX = 0, EX2 = 0;
    for (int i = 0; i < value.size(); i++) {
        EX += value[i] * prob[i];
        EX2 += value[i] * value[i] * prob[i];
    }

    return EX2 - EX * EX;
}

// Binomial Probability: P(X = k) = nCr(n, k) * p^k * (1-p)^(n-k)
ld binomialProbability(int n, int k, ld p) {
    ld comb = nCr(n, k);
    return comb * pow(p, k) * pow(1 - p, n - k);
}

// geometric probability: P(X = k) = (1-p)^(k-1) * p
// first success on kth trial
ld geometricProbability(int k, ld p) {
    return pow(1 - p, k - 1) * p;
}

ld poissonProbability(int k, ld lambda) {
    return (pow(lambda, k) * exp(-lambda)) / fact[k];
}

// Matrix Exponentiation: A^n mod M
struct Matrix {
    int r, c;
    const ll M = 1e9 + 7;
    vector<vector<ll>> mat;

    Matrix(int r, int c) : r(r), c(c), mat(r, vector<ll>(c, 0)) {}

    static Matrix identity(int n) {
        Matrix I(n, n);
        for (int i = 0; i < n; i++) I.mat[i][i] = 1;
        return I;
    }

    Matrix operator*(const Matrix& other) const {
        Matrix res(r, other.c);
        for (int i = 0; i < r; i++) {
            for (int k = 0; k < c; k++) {
                if (mat[i][k] == 0) continue;
                for (int j = 0; j < other.c; j++) {
                    res.mat[i][j] = (res.mat[i][j] + mat[i][k] * other.mat[k][j]) % M;
                }
            }
        }
        return res;
    }

    Matrix pow(ll exp) const {
        Matrix base = *this;
        Matrix res = identity(r);
        while (exp > 0) {
            if (exp & 1) res = res * base;
            base = base * base;
            exp >>= 2;
        }
        return res;
    }
};

