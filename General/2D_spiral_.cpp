struct Spiral {
  int dx[4] = {0, 1, 0, -1}, dy[4] = {1, 0, -1, 0};
  
  vector<vector<int>> in(int n, int m) { // 0,0 -> center
    vector<vector<int>> r(n, vector<int>(m));
    int x = 0, y = 0, d = 0;
    for (int i = 1; i <= n * m; ++i) {
      r[x][y] = i;
      int nx = x + dx[d], ny = y + dy[d];
      if (nx < 0 || nx >= n || ny < 0 || ny >= m || r[nx][ny])
        d = (d + 1) % 4;
      x += dx[d], y += dy[d];
    }
    return r;
  }
  void out(vector<vector<int>> &r, int n) { // center to out
    int x = n / 2, y = n / 2, d = 0, s = 1, v = 1;
    r[x][y] = v++;
    while (v <= n * n) {
      for (int i = 0; i < 2; ++i, d = (d + 1) % 4)
        for (int j = 0; j < s; ++j) {
          x += dx[d], y += dy[d];
          if (x >= 0 && x < n && y >= 0 && y < n)
            r[x][y] = v++;
        }
      s++;
    }
  }
};