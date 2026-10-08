/**
 * 2D BIT (Fenwick Tree) - Range Update & Range Query
 * Logic: Sum(r,c) = t1*r*c - t2*c - t3*r + t4
 * Internal Indexing: 1-based.
 * O(log n * log m) per update/query.
 */
struct BIT2D {
  int n, m;
  vector<vector<int>> t1, t2, t3, t4;

  BIT2D(int _n, int _m) : n(_n), m(_m) {
    t1.assign(n + 2, vector<int>(m + 2, 0));
    t2.assign(n + 2, vector<int>(m + 2, 0));
    t3.assign(n + 2, vector<int>(m + 2, 0));
    t4.assign(n + 2, vector<int>(m + 2, 0));
  }

  void upd(int r, int c, int v) {
    for (int i = r; i <= n; i += i & -i)
      for (int j = c; j <= m; j += j & -j) {
        t1[i][j] += v;
        t2[i][j] += v * (r - 1);
        t3[i][j] += v * (c - 1);
        t4[i][j] += v * (r - 1) * (c - 1);
      }
  }

  // Adds 'v' to submatrix [r1, c1] to [r2, c2]
  void range_add(int r1, int c1, int r2, int c2, int v) {
    upd(r1, c1, v);
    upd(r1, c2 + 1, -v);
    upd(r2 + 1, c1, -v);
    upd(r2 + 1, c2 + 1, v);
  }

  int query_pref(int r, int c) {
    int res = 0;
    for (int i = r; i > 0; i -= i & -i)
      for (int j = c; j > 0; j -= j & -j) {
        res += t1[i][j] * r * c;
        res -= t2[i][j] * c;
        res -= t3[i][j] * r;
        res += t4[i][j];
      }
    return res;
  }

  // Sum of submatrix [r1, c1] to [r2, c2]
  int range_sum(int r1, int c1, int r2, int c2) {
    return query_pref(r2, c2) - query_pref(r1 - 1, c2) -
           query_pref(r2, c1 - 1) + query_pref(r1 - 1, c1 - 1);
  }

  // Returns value of a single cell (r, c)
  int point_get(int r, int c) {
    int res = 0;
    for (int i = r; i > 0; i -= i & -i)
      for (int j = c; j > 0; j -= j & -j)
        res += t1[i][j];
    return res;
  }

  void clear() {
    for (int i = 0; i <= n + 1; i++) {
      fill(t1[i].begin(), t1[i].end(), 0);
      fill(t2[i].begin(), t2[i].end(), 0);
      fill(t3[i].begin(), t3[i].end(), 0);
      fill(t4[i].begin(), t4[i].end(), 0);
    }
  }
};

/* --- HOW TO USE THIS IN CP ---
    int N = 5, M = 5;
    BIT2D ft(N, M);

    // 1. Initializing from a matrix:
    // If you have an input grid, treat each cell as a 1x1 range update
    // ft.range_add(i, j, i, j, val);

    // 2. Range Update: Add 10 to submatrix from (2,2) to (4,4)
    ft.range_add(2, 2, 4, 4, 10);

    // 3. Range Query: Get sum of submatrix (1,1) to (3,3)
    // In our case, only (2,2), (2,3), (3,2), (3,3) were updated with 10.
    // So the sum should be 4 * 10 = 40.
    int s = ft.range_sum(1, 1, 3, 3);

    // 4. Point Query: Get actual value at (2,2)
    int p = ft.point_get(2, 2); // Should be 10

    // 5. Cleanup for next test case:
    ft.clear();

*/