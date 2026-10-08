struct FloydWarshall {
  int n;
  vector<vector<int>> dist;

  FloydWarshall(int _n) : n(_n) {
    dist.assign(n + 1, vector<int>(n + 1, inf));
    for (int i = 1; i <= n; i++)
      dist[i][i] = 0;
  }

  void add_edge(int u, int v, int w, bool bidirectional = false) {
    dist[u][v] = min(dist[u][v], (int)w);
    if (bidirectional)
      dist[v][u] = min(dist[v][u], (int)w);
  }

  void solve() {
    for (int k = 1; k <= n; k++) {
      for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
          if (dist[i][k] < inf && dist[k][j] < inf)
            dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
        }
      }
    }
  }

  // Helper to get distance between u and v
  // Returns INF if unreachable, -1 if part of a negative cycle
  int get_dist(int u, int v) {
    if (dist[u][u] < 0 || dist[v][v] < 0)
      return -1; // Negative cycle check
    return dist[u][v];
  }

  bool hasNegativeCycle() {
    for (int i = 1; i <= n; i++)
      if (dist[i][i] < 0)
        return true;
    return false;
  }
};