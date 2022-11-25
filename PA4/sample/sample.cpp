// reference - https://cp-algorithms.com/algebra/primality_tests.html and https://cp-algorithms.com/algebra/factorization.html
#include "bits/stdc++.h"
using namespace std;
 
#define ffor(n)  for(int i = 0; i < n; i++)
#define fffor(n) for(int j = 0; j < n; j++)
#define uwu ios::sync_with_stdio(false);cin.tie(NULL);
 
const int INF = 1e9 + 7;
const long long INF2 = 1e17;

vector <int> bases = {2, 325, 9375, 28178, 450775, 9780504, 1795265022}; // based on https://cp-algorithms.com/algebra/primality_tests.html
vector <int> smol_primes = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 193, 407521, 299210837}; 

long long powmod(long long x, long long k, long long mod){
	__int128 res = 1;
	x %= mod;
	for(int i = 0; (1LL << i) <= k; i++) {
		if (k & (1LL << i)) {
			res = ((__int128)res * x) % mod;
		}
		x = ((__int128)(x) * x) % mod;
	}
	return res;
}

int is_composite(long long n, long long a, long long d, long long s) {
	long long x = powmod(a, d, n);
	if (x == 1 || x == n - 1) return false;
	for(int r = 1; r < s; r++) {
		x = powmod(x, 2, n);
		if (x == n - 1) return false;
	}
	return true;
}

int is_prime(long long n) {
	if (n <= 1) return false;
	for(int p: smol_primes) {
		if (n % p == 0) return (n == p);
	}
	
	long long r = 0, d = n - 1;
	while(d % 2 == 0) {
		r++; d /= 2;
	}
	
	for(int a: bases) {
		if(is_composite(n, a, d, r)) {
			return false;
		}
	}
	return true;
}

long long pollard_rho(long long x, long long n) {
	long long res = 1, y = x;
	while(res == 1) {
		x = (powmod(x, 2, n) + 1) % n;
		y = (powmod(y, 2, n) + 1) % n;
		y = (powmod(y, 2, n) + 1) % n;
		res = __gcd(abs(x - y), n);
	}
	return res;
}

// based on https://cp-algorithms.com/algebra/factorization.html
map <long long, long long> dp;
long long get_factor(long long n) {
	if (dp[n]) return dp[n];
	for(int p: smol_primes) {
		if(n % p == 0) {
			dp[n] = p;
			return p;
		}
	}
	for(long long x = 1; ; x++) {
		long long res = pollard_rho(x, n);
		if (1 < res && res < n) {
			dp[n] = res;
			return res;
		}
	}
}

void factor(long long n) {
	vector <long long> primes, factors;
	factors.push_back(n);
	while((int)factors.size()) {
		long long x = factors.back(); factors.pop_back();
		if (is_prime(x) || x == 1) primes.push_back(x);
		else {
			long long y = get_factor(x);
			factors.push_back(y);
			factors.push_back(x / y);
		}
	}
	sort(primes.begin(), primes.end());
	cout << n << '=';
	for(int i = 0; i < (int)primes.size() - 1; i++) cout << primes[i] << '*';
	cout << primes.back() << '\n';
}

int main(int argc, char** argv) {
	long long left = stoll(argv[1]), right = stoll(argv[2]);
	for(unsigned long long n = left; n <= (unsigned long long)right; n++) factor(n);
}

