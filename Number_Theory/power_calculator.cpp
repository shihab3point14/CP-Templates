////////////////////////////////////////////////////////////////////////////

/// p/q % mod == (p % mod * q^(-1) % mod) == p % mod * binpow(q,mod-2) % mod
long long binpow(long long a,long long b)
{
    if(b==0)
    {
        return 1;
    }
    if(b%2)
    {
        return (a*binpow(a,b-1))%MOD;
    }
    return binpow((a*a)%MOD,b/2);
}

////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

// powmod function to calculate (x^n) % P 
// if power n is large, it uses the method of exponentiation by squaring
// to compute the result efficiently in O(log n) time complexity.
int powmod(int x, int n, int P) {
    x %= P;
    int res = 1 % P;
    while (n) {
        if ((n & 1)) {
            res = (res * x) % P;
            n--;
        }
        else {
            x = (x * x) % P;
            n >>= 1;
        }
    }
    return res;
}

int myPow(int x, string s, int P) {
    x %= P;
    int res = 1 % P;
    int now = x;
    for (int i = (int)s.size() - 1; i >= 0; i--) {
        res = (res * powmod(now, s[i] - '0', P)) % P;
        now = powmod(now, 10, P);
    }
    return res;
}

///////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////

// calculate large power of a number without MOD
// Function to multiply two large numbers represented as vectors
// power (x,n)
// complexity : O(n^2 log(x)^2)

vector<int> mul(const vector<int>& a, const vector<int>& b) {
    vector<int> r(a.size() + b.size());
    for (int i = 0; i < (int)a.size(); ++i) {
        int c = 0;
        for (int j = 0; j < (int)b.size() || c; ++j) {
            long long cur = r[i + j] + (long long)a[i] * (j < (int)b.size() ? b[j] : 0) + c;
            r[i + j] = cur % 10;
            c = cur / 10;
        }
    }
    while (r.size() > 1 && r.back() == 0) r.pop_back();
    return r;
}

vector<int> pv(int x, int n) {
    if (n == 0) return {1};
    if (n == 1) {
        vector<int> v;
        while (x) { v.push_back(x % 10); x /= 10; }
        return v;
    }
    auto h = pv(x, n / 2);
    auto r = mul(h, h);
    if (n & 1) {
        vector<int> v;
        int t = x;
        while (t) { v.push_back(t % 10); t /= 10; }
        r = mul(r, v);
    }
    return r;
}

string power(int x, int n) {
    auto v = pv(x, n);
    string s;
    for (int i = v.size() - 1; i >= 0; --i) s += char('0' + v[i]);
    return s;
}

/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////