/**
 * Euler's Totient Function (Phi)
 * counts integers 1 <= k <= n such that gcd(k, n) == 1.
 */
struct EulerPhi {
  vector<int> phi_table;

  // 1. Single Value Calculation: O(sqrt(n))
  static long long single_phi(long long n) {
    long long res = n;
    for (long long i = 2; i * i <= n; i++) {
      if (n % i == 0) {
        while (n % i == 0)
          n /= i;
        res -= res / i;
      }
    }
    if (n > 1)
      res -= res / n;
    return res;
  }

  // 2. Range Precomputation (Sieve): O(N log log N)
  // Use this if you need to call phi many times for values up to n.
  void precompute(int n) {
    phi_table.resize(n + 1);
    iota(phi_table.begin(), phi_table.end(), 0); // phi_table[i] = i

    for (int i = 2; i <= n; i++) {
      if (phi_table[i] == i) { // i is prime
        for (int j = i; j <= n; j += i)
          phi_table[j] -= phi_table[j] / i;
      }
    }
  }

  // Helper for precomputed access
  int get(int n) { return phi_table[n]; }
};