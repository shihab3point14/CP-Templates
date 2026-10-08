struct Totient {
    // Member variable to store precomputed phi values
    vector<int> phi;

    // 1. Constructor: Standard Sieve in O(N log log N)
    // Initializes the struct and precomputes phi for all numbers up to N.
    Totient(int N) {
        phi.resize(N + 1);
        for (int i = 0; i <= N; i++) {
            phi[i] = i;
        }
        
        for (int i = 2; i <= N; i++) {
            if (phi[i] == i) { // i is prime
                for (int j = i; j <= N; j += i) {
                    phi[j] -= phi[j] / i;
                }
            }
        }
    }

    // 2. Get Phi Value in O(1)
    // Fetch the precomputed phi value for n.
    int get(int n) const {
        // Optional: Add bounds checking if you aren't strictly controlling input
        // if (n < 0 || n >= phi.size()) return -1; 
        return phi[n];
    }

    // 3. Single Phi in O(sqrt(n))
    // Use this when you only need phi for a few isolated, large numbers.
    static long long single(long long n) {
        long long result = n;
        for (long long i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                while (n % i == 0) n /= i;
                result -= result / i;
            }
        }
        if (n > 1) result -= result / n;
        return result;
    }

    // 4. Segmented Sieve for range [L, R]
    // Use this when R is huge (up to 10^12) but the range (R - L) is small (<= 10^6).
    static vector<long long> segmented_sieve(long long L, long long R) {
        int range_size = R - L + 1;
        vector<long long> phi_arr(range_size);
        vector<long long> rem_arr(range_size);
        
        for (int i = 0; i < range_size; i++) {
            phi_arr[i] = L + i;
            rem_arr[i] = L + i;
        }

        long long limit = sqrt(R);
        vector<bool> is_prime(limit + 1, true);
        
        for (long long p = 2; p <= limit; p++) {
            if (is_prime[p]) {
                for (long long j = p * p; j <= limit; j += p) {
                    is_prime[j] = false;
                }

                // Find the first multiple of p that is >= L
                long long start = max(p, (L + p - 1) / p * p);
                
                for (long long j = start; j <= R; j += p) {
                    int idx = j - L;
                    phi_arr[idx] -= phi_arr[idx] / p;
                    while (rem_arr[idx] % p == 0) {
                        rem_arr[idx] /= p;
                    }
                }
            }
        }

        for (int i = 0; i < range_size; i++) {
            if (rem_arr[i] > 1) {
                phi_arr[i] -= phi_arr[i] / rem_arr[i];
            }
        }
        return phi_arr;
    }

    // --- BONUS CP HELPERS ---

    // 5. Modular Exponentiation: (base^exp) % mod in O(log exp)
    static long long power(long long base, long long exp, long long mod) {
        long long res = 1;
        base %= mod;
        while (exp > 0) {
            if (exp % 2 == 1) res = (long long)((__int128)res * base % mod); // Cast back to avoid warnings
            base = (long long)((__int128)base * base % mod);
            exp /= 2;
        }
        return res;
    }

    // 6. Modular Inverse using Euler's Theorem
    // Only works if gcd(n, mod) == 1. Very useful when 'mod' is NOT prime.
    // Inverse is n^(phi(mod) - 1) % mod
    static long long mod_inverse(long long n, long long mod) {
        // Note: If you do this often for the same mod, precompute single(mod) outside this function!
        return power(n, single(mod) - 1, mod);
    }
};