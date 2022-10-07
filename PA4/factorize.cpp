// reference materials:
// Miller-Rabin primality test: https://cp-algorithms.com/algebra/primality_tests.html
//                              https://www.geeksforgeeks.org/primality-test-set-3-miller-rabin/
// Pollard’s Rho Algorithm: https://cp-algorithms.com/algebra/factorization.html
//                          https://www.geeksforgeeks.org/pollards-rho-algorithm-prime-factorization/

#include <iostream>
#include <numeric>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

vector<long long> prime_list = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};

long long power(long long base, long long exp, long long mod)
{
    long long result = 1;
    base %= mod;
    while (exp)
    {
        if (exp & 1)
            result *= base % mod;
        base *= base % mod;
        exp >>= 1;
    }
    return result;
}

bool MillerRabin(long long n, long long d)
{
    // generate #iter times random number

    int a = 2 + rand() % (n - 4);

    long long x = power(a, d, n);
    if (x == 1 || x == n - 1)
        return true;
    while (d != n - 1)
    {
        x *= x % n;
        d <<= 1;
        if (x == n - 1)
            return true;
        if (x == 1)
            return false;
    }
    return false;
}

// returns true if n is probably prime, else returns false. Using Miller Rabin method
bool isPrime(long long n, int iter = 5)
{
    // quick check for numbers is even
    if ((n & 1) == 0)
        return false;

    // quick check for numbers having prime divisors less than 100
    for (long long i : prime_list)
    {
        if (!n % i)
            return false;
    }

    long long d = n - 1;
    while ((d & 1) == 0)
    {
        d >>= 1;
    }

    for (int i = 0; i < iter; i++)
    {
        if (!MillerRabin(n, d))
            return false;
    }
    return true;
}

vector<long long> wheel_division(long long n)
{
    vector<long long> output;
    for (int d : {2, 3, 5})
    {
        while (n % d == 0)
        {   
            output.push_back(d);
            n /= d;
        }
    }
    int increments[] = {4, 2, 4, 2, 4, 6, 2, 6};
    int i = 0;
    for (long long d = 7; d * d <= n; d += increments[i++])
    {   
        
        while (n % d == 0)
        {
            output.push_back(d);
            n /= d;
        }
        if (i == 8)
            i = 0;
    }
    if (n > 1)
        output.push_back(n);
    return output;
}


long long brent(long long n, long long x0 = 2, long long c = 1)
{
    if (isPrime(n))
    {
        return n;
    }
    long long x = x0;
    long long g = 1;
    long long q = 1;
    long long xs, y;

    int m = 128;
    int l = 1;
    while (g == 1)
    {
        y = x;
        for (int i = 1; i < l; i++)
            x = (x * x % n + c) % n;
        int k = 0;
        while (k < l && g == 1)
        {
            xs = x;
            for (int i = 0; i < m && i < l - k; i++)
            {
                x = (x * x % n + c) % n;
                q *= abs(y - x) % n;
            }
            g = __gcd(q, n);
            k += m;
        }
        l *= 2;
    }
    if (g == n)
    {
        do
        {
            xs = (xs * xs % n + c) % n;
            g = __gcd(abs(xs - y), n);
        } while (g == 1);
    }
    return g;
}

int main(int argc, char *argv[])
{
    long long start = atoll(argv[1]);
    long long end = atoll(argv[2]);
    vector<long long> output;
    vector<long long> temp;

    for (long long i = start; i <= end; i++)
    {
        output.clear();
        if (i >> 16)
        {
            output = wheel_division(i);
        }
        else
        {
            long long j;
            while (i >> 16)
            {
                j = brent(i);
                output.push_back(j);
                i /= j;
            }
            vector<long long> temp = wheel_division(i);
            output.insert(output.end(), temp.begin(), temp.end());
        }

        cout << i << "=";
        vector<long long>::iterator it = output.begin();
        for (; it != std::prev(output.end(), 1); it++)
        {
            cout << *it << "*";
        }
        cout << *it << endl;
        if (i == numeric_limits<long long>::max())
            break;
    }
}