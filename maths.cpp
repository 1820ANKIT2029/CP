#include <bits/stdc++.h>
using namespace std;

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

long long modInverse(long long n) {
    return power(n, MOD - 2);
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

