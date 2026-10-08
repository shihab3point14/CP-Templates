// dijkstra 
// ElogV
// V = number of nodes

pair<vector<int>, vector<int>> dijkstra(vector<list<pair<int, int>>> &adj,
                                        int n, int src) {
  vector<int> dist(n, inf), par(n, -1);
  set<pair<int, int>> st;
  dist[src] = 0;
  st.insert({0, src});
  while (!st.empty()) {
    auto [d, u] = *st.begin();
    st.erase(st.begin());
    for (auto [v, w] : adj[u]) {
      if (dist[v] > d + w) {
        auto it = st.find({dist[v], v});
        if (it != st.end())
          st.erase(it);
        dist[v] = d + w;
        par[v] = u;
        st.insert({dist[v], v});
      }
    }
  }
  return {dist, par};
}