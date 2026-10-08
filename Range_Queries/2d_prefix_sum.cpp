// 0-base index
struct Pref2D {
  int n, m;
  vector<vector<long long>> p;
  Pref2D(const vector<vector<int>> &a)
      : n(a.size()), m(a[0].size()), p(n + 1, vector<long long>(m + 1, 0)) {
    for (int i = 1; i <= n; ++i)
      for (int j = 1; j <= m; ++j)
        p[i][j] = a[i - 1][j - 1] + p[i - 1][j] + p[i][j - 1] - p[i - 1][j - 1];
  }
  long long sum(int r1, int c1, int r2, int c2) {
    if (r1 > r2 || c1 > c2)
      return 0;
    return p[++r2][++c2] - p[r1][c2] - p[r2][c1] + p[r1][c1];
  }
  double avg(int r1, int c1, int r2, int c2) {
    long long area = 1LL * (r2 - r1 + 1) * (c2 - c1 + 1);
    return area <= 0 ? 0 : (double)sum(r1, c1, r2, c2) / area;
  }
};
