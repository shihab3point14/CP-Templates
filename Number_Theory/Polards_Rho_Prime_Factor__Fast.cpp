// [Assume power() and miller_rabin() and is_prime() are pasted here from earlier]

// Core Pollard's Rho to find a single non-trivial divisor
ll pollard_rho(ll n) {
    if (n % 2 == 0) return 2;
    if (is_prime(n)) return n;

    ll x = 2, y = 2, d = 1, c = 1;
    
    // Pseudo-random polynomial: f(x) = (x^2 + c) % n
    auto f = [&](ll x, ll n, ll c) {
        return (ll)(((__int128)x * x % n + c) % n);
    };

    while (d == 1) {
        x = f(x, n, c);          // Tortoise moves 1 step
        y = f(f(y, n, c), n, c); // Hare moves 2 steps
        d = std::gcd(abs(x - y), n);
        
        if (d == n) {
            // Cycle detected without finding a factor, change 'c' and retry
            x = rand() % (n - 2) + 2;
            y = x;
            c = rand() % (n - 1) + 1;
            d = 1;
        }
    }
    return d;
}

// Wrapper to get all prime factors
void factorize_recursive(ll n, vector<ll>& factors) {
    if (n == 1) return;
    if (is_prime(n)) {
        factors.push_back(n);
        return;
    }
    
    ll divisor = pollard_rho(n);
    factorize_recursive(divisor, factors);
    factorize_recursive(n / divisor, factors);
}

// Main function to call from your CP template
vector<ll> get_prime_factors(ll n) {
    vector<ll> factors;
    factorize_recursive(n, factors);
    sort(factors.begin(), factors.end()); // Return sorted factors
    return factors;
}