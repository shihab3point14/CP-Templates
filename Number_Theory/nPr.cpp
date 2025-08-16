int nPr(int n, int r) {
    if (n < r) return 0;
    int res = 1;
    for (int i = 0; i < r; i++) {
        res *= (n - i);
        res %= MOD;
    }
    return res;
}