// Represents a specific edge in the flow network
struct FlowEdge {
  int v, u; // v -> u
  long long cap, flow = 0;
  FlowEdge(int v, int u, long long cap) : v(v), u(u), cap(cap) {}
};

struct Dinic {
  const long long flow_inf = 1e18; // Use a value larger than sum of all capacities
  vector<FlowEdge> edges;
  vector<vector<int>> adj;
  int n, m = 0;
  int s, t;
  vector<int> level; // Distance from S (for Level Graph)
  vector<int> ptr; // Pointer for next edge to explore (Current Arc Optimization)
  queue<int> q;

  // Constructor: Initialize with Number of nodes, Source, and Sink
  Dinic(int n, int s, int t) : n(n), s(s), t(t) {
    adj.resize(n);
    level.resize(n);
    ptr.resize(n);
  }

  // === CHANGE 1: EDGE DIRECTION ===
  // Use this to add edges.
  // Standard MaxFlow problems are DIRECTED.
  void add_edge(int v, int u, long long cap) {
    edges.emplace_back(v, u, cap); // Forward edge with capacity

    // FOR DIRECTED GRAPH (Standard): Backward edge has 0 capacity
    edges.emplace_back(u, v, 0);

    // FOR UNDIRECTED GRAPH: Backward edge has 'cap' capacity
    // edges.emplace_back(u, v, cap); // <--- UNCOMMENT THIS for Undirected

    adj[v].push_back(m);
    adj[u].push_back(m + 1);
    m += 2;
  }

  // BFS to build the Level Graph
  // Returns true if T is reachable from S in residual graph
  bool bfs() {
    while (!q.empty()) {
      int v = q.front();
      q.pop();
      for (int id : adj[v]) {
        // If residual capacity is 0 or node already visited, skip
        if (edges[id].cap - edges[id].flow == 0)
          continue;
        if (level[edges[id].u] != -1)
          continue;

        level[edges[id].u] = level[v] + 1;
        q.push(edges[id].u);
      }
    }
    return level[t] != -1;
  }

  // DFS to find blocking flow in the Level Graph
  long long dfs(int v, long long pushed) {
    if (pushed == 0)
      return 0;
    if (v == t)
      return pushed;

    // 'ptr[v]' implements the "Current Arc" optimization
    // We don't restart scanning neighbors from index 0 every time
    for (int &cid = ptr[v]; cid < (int)adj[v].size(); cid++) {
      int id = adj[v][cid];
      int u = edges[id].u;

      // Only move to next level in level graph
      if (level[v] + 1 != level[u] || edges[id].cap - edges[id].flow == 0)
        continue;

      long long tr = dfs(u, min(pushed, edges[id].cap - edges[id].flow));
      if (tr == 0)
        continue;

      edges[id].flow += tr;
      edges[id ^ 1].flow -= tr; // Update backward edge residual
      return tr;
    }
    return 0;
  }

  // Main function to calculate Max Flow
  long long max_flow() {
    long long f = 0;
    while (true) {
      fill(level.begin(), level.end(), -1);
      level[s] = 0;
      q.push(s);
      if (!bfs())
        break; // No more augmenting paths

      fill(ptr.begin(), ptr.end(), 0);
      while (long long pushed = dfs(s, flow_inf)) {
        f += pushed;
      }
    }
    return f;
  }

  // === EXTRA FEATURE 1: MIN-CUT ===
  // Returns a boolean vector where true means the node is on the S-side of the
  // cut
  vector<bool> get_min_cut() {
    // Run max_flow() first!
    // After max_flow, the BFS/Level array marks all nodes reachable from S
    // in the residual graph. These form the S-set of the Min-Cut.
    vector<bool> is_on_source_side(n, false);
    for (int i = 0; i < n; i++) {
      if (level[i] != -1)
        is_on_source_side[i] = true;
    }
    return is_on_source_side;
  }

  // === EXTRA FEATURE 2: GET FLOW EDGES ===
  // Useful for printing the actual solution (which edges are used)
  struct ResultEdge {
    int v, u;
    long long flow;
  };
  vector<ResultEdge> get_flow_edges() {
    vector<ResultEdge> res;
    for (int i = 0; i < m; i += 2) { // Iterate only forward edges
      if (edges[i].flow > 0) {
        res.push_back({edges[i].v, edges[i].u, edges[i].flow});
      }
    }
    return res;
  }
};

/*

    n = node, m = edges;
    // Step 2: Define Source (s) and Sink (t)
    // usually 1 and n in problem statements, but 0 and n-1 in our 0-based implementation
    int s = 0;
    int t = n - 1; 

    // Step 3: Initialize Dinic Structure
    // This resizes vectors and prepares the graph
    Dinic dinic(n, s, t);

    // Step 4: Read Edges
    for (int i = 0; i < m; i++) {
        int u, v;
        long long cap;
        cin >> u >> v >> cap;
        // === IMPORTANT: INDEXING ADJUSTMENT ===
        // Most competitive programming problems give 1-based indices (1 to N).
        // Since C++ vectors are 0-based, we usually decrement u and v.
        u--; 
        v--; 
        // Add the edge to the graph
        dinic.add_edge(u, v, cap);
    }

    // Step 5: Compute and Print Max Flow
    cout << dinic.max_flow() << endl;

    // Optional: Print the Min-Cut (Nodes on Source side)
    // vector<bool> cut = dinic.get_min_cut();
    // for(int i=0; i<n; i++) if(cut[i]) cout << i+1 << " "; cout << endl;
*/