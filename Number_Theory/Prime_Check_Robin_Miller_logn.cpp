// O(K*log^3(n)) worst O(1000)
// 1 to 1e18
// add 3 more prime for 18*1e18

typedef long long ll;

ll power(ll base, ll exp, ll mod) {
    ll res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (__int128)res * base % mod;
        base = (__int128)base * base % mod;
        exp /= 2;
    }
    return res;
}

bool miller_rabin(ll n, ll a) {
    if (a % n == 0) return true;
    ll d = n - 1;
    int s = 0;
    while (d % 2 == 0) {
        d /= 2;
        s++;
    }
    ll x = power(a, d, n);
    if (x == 1 || x == n - 1) return true;
    for (int r = 1; r < s; r++) {
        x = (__int128)x * x % n;
        if (x == n - 1) return true;
    }
    return false;
}

bool is_prime(ll n) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;
    // Bases for deterministic test up to 3e18
    static const vector<ll> bases = {2, 3, 5, 7, 11, 13, 17, 19, 23};
    for (ll a : bases) {
        if (n == a) return true;
        if (!miller_rabin(n, a)) return false;
    }
    return true;
}