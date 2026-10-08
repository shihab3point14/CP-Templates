/**
 * Find Articulation Points (Cutpoints) and Bridges
 * Complexity: O(V + E)
 * tin: Time in (when the node was first visited)
 * low: Lowest tin reachable via back-edges in DFS tree
 */
struct GraphAnalysis {
  int n, timer;
  vector<vector<int>> adj;
  vector<int> tin, low;
  vector<bool> is_cutpoint;
  vector<pair<int, int>> bridges; // Store bridges as pairs of vertices

  GraphAnalysis(int _n) : n(_n) {
    adj.resize(n + 1);
    tin.assign(n + 1, -1);
    low.assign(n + 1, -1);
    is_cutpoint.assign(n + 1, false);
  }

  void add_edge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  void dfs(int v, int p = -1) {
    tin[v] = low[v] = timer++;
    int children = 0;
    for (int to : adj[v]) {
      if (to == p)
        continue;
      if (tin[to] != -1) { // Already visited, this is a back-edge
        low[v] = min(low[v], tin[to]);
      } else { // Tree-edge
        dfs(to, v);
        low[v] = min(low[v], low[to]);

        // Condition for Bridge
        if (low[to] > tin[v])
          bridges.push_back({v, to});

        // Condition for Articulation Point
        if (low[to] >= tin[v] && p != -1)
          is_cutpoint[v] = true;

        children++;
      }
    }
    // Root special case
    if (p == -1 && children > 1)
      is_cutpoint[v] = true;
  }

  void find_all() {
    timer = 0;
    bridges.clear();
    fill(is_cutpoint.begin(), is_cutpoint.end(), false);
    fill(tin.begin(), tin.end(), -1);
    for (int i = 1; i <= n; i++) {
      if (tin[i] == -1)
        dfs(i);
    }
  }

  // Helper to get articulation points as a list
  vector<int> get_articulation_points() {
    vector<int> res;
    for (int i = 1; i <= n; i++)
      if (is_cutpoint[i])
        res.push_back(i);
    return res;
  }
};