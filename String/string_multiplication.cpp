// string multiplication for large numbers
// O(|a|+|b|)

string mul(string a, string b) {
    int n = a.size(), m = b.size();
    vector<int> r(n + m);
    for (int i = n - 1; i >= 0; --i) {
        int ai = a[i] - '0', c = 0;
        for (int j = m - 1; j >= 0; --j) {
            int bi = b[j] - '0';
            int sum = r[i + j + 1] + ai * bi + c;
            r[i + j + 1] = sum % 10;
            c = sum / 10;
        }
        r[i] += c;
    }
    string s;
    for (int x : r) if (!(s.empty()&& x==0)) s += char('0' + x);
    return s.empty() ? "0" : s;
}