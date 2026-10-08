
/**
 * FFT (Fast Fourier Transform) Struct
 * Time Complexity: O(N log N)
 * Usage: Polynomial Multiplication, BigInt Multiplication
 */
struct FFT {
    using cd = complex<double>;
    const double PI = acos(-1);

    // Bit-reversal permutation for in-place FFT
    void bit_reverse(vector<cd>& a) {
        int n = a.size();
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (i < j) swap(a[i], a[j]);
        }
    }

    // In-place FFT computation
    void compute(vector<cd>& a, bool invert) {
        int n = a.size();
        bit_reverse(a);

        for (int len = 2; len <= n; len <<= 1) {
            double ang = 2 * PI / len * (invert ? -1 : 1);
            cd wlen(cos(ang), sin(ang));
            for (int i = 0; i < n; i += len) {
                cd w(1);
                for (int j = 0; j < len / 2; j++) {
                    cd u = a[i + j], v = a[i + j + len / 2] * w;
                    a[i + j] = u + v;
                    a[i + j + len / 2] = u - v;
                    w *= wlen;
                }
            }
        }

        if (invert) {
            for (cd& x : a) x /= n;
        }
    }

    /**
     * Standard Polynomial Multiplication: C(x) = A(x) * B(x)
     * Useful for: Counting pairs, string matching with wildcards
     */
    vector<long long> multiply(const vector<int>& a, const vector<int>& b) {
        vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
        int n = 1;
        while (n < a.size() + b.size()) n <<= 1;
        fa.resize(n);
        fb.resize(n);

        compute(fa, false);
        compute(fb, false);
        for (int i = 0; i < n; i++) fa[i] *= fb[i];
        compute(fa, true);

        vector<long long> result(n);
        for (int i = 0; i < n; i++) result[i] = round(fa[i].real());
        
        // Trim trailing zeros to keep the polynomial size minimal
        while (result.size() > 1 && result.back() == 0) result.pop_back();
        return result;
    }

    /**
     * Polynomial Exponentiation: A(x)^k
     * Useful for: Combinatorics (ways to pick items with replacement)
     */
    vector<long long> power(vector<int> a, int k) {
        vector<long long> res = {1};
        vector<long long> base(a.begin(), a.end());
        while (k > 0) {
            if (k & 1) res = multiply_long(res, base);
            base = multiply_long(base, base);
            k >>= 1;
        }
        return res;
    }

};