

// Heavy-Light Decomposition (HLD) with Segment Tree
// Supports path and subtree updates/queries on trees.
// This version uses range-add and range-sum queries.
// Complexity: O((N + Q) log N)
// Space: O(N)

struct SegmentTree {
  vector<long long> t, lazy;
  int n;

  SegmentTree(int _n) {
    n = _n;
    t.resize(4 * n + 9);
    lazy.resize(4 * n + 9);
  }

  //-----------------------------------------------------------------------------
  // Build the tree for the initial array a[0..n-1]
  // v     = current node index in t[]
  // tl,tr = segment covered by node v
  //-----------------------------------------------------------------------------
  void build(const vector<long long> &a, int v, int tl, int tr) {
    if (tl == tr) {
      // leaf: store the array value
      t[v] = a[tl];
    } else {
      int tm = (tl + tr) / 2;
      build(a, v * 2, tl, tm);
      build(a, v * 2 + 1, tm + 1, tr);
      // **COMBINE step for SUM**
      t[v] = t[v * 2] + t[v * 2 + 1];
      // To change to MAX:   t[v] = max(t[v*2], t[v*2+1]);
      //             MIN:   t[v] = min(t[v*2], t[v*2+1]);
      //             GCD:   t[v] = gcd(t[v*2], t[v*2+1]);
      //             LCM:   t[v] = lcm(t[v*2], t[v*2+1]);
    }
  }

  //-----------------------------------------------------------------------------
  // Push the pending update in lazy[v] down to children of v
  //-----------------------------------------------------------------------------
  void push(int v, int tl, int tr) {
    if (lazy[v] != 0) {
      // apply lazy to current node
      t[v] += lazy[v] * (tr - tl + 1);
      // for sum, we add (increment * length)
      // if you were doing max/min you would do t[v] += lazy[v];
      // (and for assignment-type lazy you’d overwrite instead)

      if (tl != tr) {
        // propagate to children
        lazy[v * 2] += lazy[v];
        lazy[v * 2 + 1] += lazy[v];
      }
      lazy[v] = 0;
    }
  }

  //-----------------------------------------------------------------------------
  // Range‐add update: add 'add_val' to each element in [l..r]
  // v,tl,tr = as before; l,r = query range
  //-----------------------------------------------------------------------------
  void update_range(int v, int tl, int tr, int l, int r, long long add_val) {
    push(v, tl, tr);
    if (l > r)
      return;
    if (l == tl && r == tr) {
      // mark lazy and push immediately
      lazy[v] += add_val;
      push(v, tl, tr);
    } else {
      int tm = (tl + tr) / 2;
      update_range(v * 2, tl, tm, l, min(r, tm), add_val);
      update_range(v * 2 + 1, tm + 1, tr, max(l, tm + 1), r, add_val);
      // after children updated, recompute this node
      t[v] = t[v * 2] + t[v * 2 + 1];
      // **Also change this combine** if you swap sum for max/min/gcd/lcm
    }
  }

  //-----------------------------------------------------------------------------
  // Query over [l..r]
  //-----------------------------------------------------------------------------
  long long query_sum(int v, int tl, int tr, int l, int r) {
    if (l > r)
      return 0; // identity for sum
    push(v, tl, tr);
    if (l == tl && r == tr) {
      return t[v];
    }
    int tm = (tl + tr) / 2;
    return query_sum(v * 2, tl, tm, l, min(r, tm)) +
           query_sum(v * 2 + 1, tm + 1, tr, max(l, tm + 1), r);
    // Again, change the '+' and return‐0 above if you switch to
    // max/min/gcd/lcm:
    // - identity for MAX = -(1LL<<60),   combine = max(...)
    // - identity for MIN = +(1LL<<60),   combine = min(...)
    // - identity for GCD = 0,             combine = gcd(...)
    // - identity for LCM = 1,             combine = lcm(...)
  }
};

/**
 * HLD STRUCT
 * Decomposes the tree into heavy chains and maps nodes to a linear array.
 */
struct HLD {
  int n;
  vector<vector<int>> adj; // Adjacency list
  vector<int> parent;      // Parent of node
  vector<int> depth;       // Depth of node (root = 0)
  vector<int> heavy;       // Heavy child of node (-1 if none)
  vector<int> head;        // Top of the heavy chain the node belongs to
  vector<int> pos;         // Position in the flattened Segment Tree array
  vector<int> sz;          // Subtree size
  int cur_pos;             // Counter for setting positions
  SegmentTree st;          // Internal Segment Tree

  // Constructor: initializes arrays and Segment Tree
  HLD(int _n)
      : n(_n), adj(_n), parent(_n), depth(_n), heavy(_n, -1), head(_n), pos(_n),
        sz(_n), st(_n) {}

  // Add edge (undirected)
  void add_edge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  // DFS 1: Computes subtree size, depth, parent, and finds heavy edges
  void dfs_sz(int v, int p = -1, int d = 0) {
    sz[v] = 1;
    parent[v] = p;
    depth[v] = d;
    int max_sz = 0;

    for (int &c : adj[v]) {
      if (c != p) {
        dfs_sz(c, v, d + 1);
        sz[v] += sz[c];
        // If this child is bigger than current max, it becomes heavy
        if (sz[c] > max_sz) {
          max_sz = sz[c];
          heavy[v] = c;
        }
      }
    }
  }

  // DFS 2: Decomposes the tree and assigns positions (Heavy edges first)
  // This ensures heavy chains form contiguous ranges in the Segment Tree
  void dfs_hld(int v, int h) {
    head[v] = h;
    pos[v] = cur_pos++; // Assign position in flattened array

    // Go to heavy child first (if exists)
    if (heavy[v] != -1) {
      dfs_hld(heavy[v], h); // Heavy child continues the chain
    }

    // Go to light children
    for (int c : adj[v]) {
      if (c != parent[v] && c != heavy[v]) {
        dfs_hld(c, c); // Light child starts a new chain
      }
    }
  }

  // Initialization function to be called after adding edges
  void init(int root = 0) {
    cur_pos = 0;
    dfs_sz(root);        // First pass
    dfs_hld(root, root); // Second pass
  }

  // --- MAIN OPERATIONS ---

  // Update Path between u and v
  void update_path(int u, int v, long long val) {
    // Climb up until u and v are on the same chain
    while (head[u] != head[v]) {
      if (depth[head[u]] < depth[head[v]])
        swap(u, v); // Ensure u is deeper
      // Update the segment of the chain from head[u] to u
      st.update_range(1, 0, n - 1, pos[head[u]], pos[u], val);
      u = parent[head[u]]; // Move u to the parent of the chain head
    }
    // Now u and v are on the same chain
    if (depth[u] > depth[v])
      swap(u, v);
    // Update the remaining segment
    st.update_range(1, 0, n - 1, pos[u], pos[v], val);
  }

  // Query Path between u and v
  long long query_path(int u, int v) {
    long long res = 0; // Identity value
    // Climb up until u and v are on the same chain
    while (head[u] != head[v]) {
      if (depth[head[u]] < depth[head[v]])
        swap(u, v);
      // Query the segment of the chain
      res += st.query_sum(1, 0, n - 1, pos[head[u]], pos[u]);
      u = parent[head[u]];
    }
    // Final segment on the same chain
    if (depth[u] > depth[v])
      swap(u, v);
    res += st.query_sum(1, 0, n - 1, pos[u], pos[v]);
    return res;
  }

  /*
  // --- MODIFIED OPERATIONS FOR EDGE WEIGHTS ---

    // Update Path (Edges) between u and v
    void update_path_edge(int u, int v, int val) {
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) swap(u, v);
            // Standard update for the heavy chain part
            st.update(1, 0, n - 1, pos[head[u]], pos[u], val);
            u = parent[head[u]];
        }

        if (depth[u] > depth[v]) swap(u, v);

        // EDGE LOGIC CHANGE:
        // u is now the LCA. The value at u corresponds to edge (parent[u]-u).
        // This edge is NOT part of the u-v path.
        // So we update from pos[u] + 1 to pos[v].
        // If u == v, this range is invalid (start > end), which Segment Tree
  ignores. st.update(1, 0, n - 1, pos[u] + 1, pos[v], val);
    }

    // Query Path (Edges) between u and v
    int query_path_edge(int u, int v) {
        int res = 0;
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) swap(u, v);
            res = st.merge(res, st.query(1, 0, n - 1, pos[head[u]], pos[u]));
            u = parent[head[u]];
        }

        if (depth[u] > depth[v]) swap(u, v);

        // EDGE LOGIC CHANGE: Skip the LCA (u)
        res = st.merge(res, st.query(1, 0, n - 1, pos[u] + 1, pos[v]));
        return res;
    }
  */

  // --- EXTRA FUNCTIONS ---

  // Query Subtree of u
  long long query_subtree(int u) {
    return st.query_sum(1, 0, n - 1, pos[u], pos[u] + sz[u] - 1);
  }

  // Update Subtree of u
  void update_subtree(int u, long long val) {
    st.update_range(1, 0, n - 1, pos[u], pos[u] + sz[u] - 1, val);
  }

  // Find Lowest Common Ancestor (LCA)
  int lca(int u, int v) {
    while (head[u] != head[v]) {
      if (depth[head[u]] < depth[head[v]])
        swap(u, v);
      u = parent[head[u]];
    }
    if (depth[u] > depth[v])
      swap(u, v);
    return u;
  }
};

/*

    // ------------------------------------------
    // STEP 1: READ N AND INITIALIZE STRUCT
    // ------------------------------------------
    int n; cin >> n;

    // Create the HLD object.
    // NOTE: This struct uses 0-based indexing (nodes 0 to n-1).
    HLD hld(n);

    // ------------------------------------------
    // STEP 2: READ INITIAL NODE VALUES
    // ------------------------------------------
    // Most problems give you an array A where A[i] is the value of node i.
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    // ------------------------------------------
    // STEP 3: BUILD TREE TOPOLOGY (EDGES)
    // ------------------------------------------
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;

        // IMPORTANT: If input is 1-based (1 to N), convert to 0-based.
        u--; v--;

        hld.add_edge(u, v);
    }

    // ------------------------------------------
    // STEP 4: PRE-COMPUTE HLD
    // ------------------------------------------
    // This runs DFS, finds heavy edges, and sets up positions.
    // MUST be called before any updates/queries.
    hld.init();

    // ------------------------------------------
    // STEP 5: LOAD INITIAL VALUES INTO SEGMENT TREE
    // ------------------------------------------
    // The segment tree is currently all 0s. We need to fill it with array `a`.
    // The safest way is to update every node `i` with its value.
    for (int i = 0; i < n; i++) {
        // "Update path from i to i" is effectively a point update on node i
        hld.update_path(i, i, a[i]);
    }

    // ------------------------------------------
    // STEP 6: HANDLE QUERIES
    // ------------------------------------------

    // OPERATION: Path Update
    // "Add x to all nodes on the path between u and v"
    u--; v--; // Convert to 0-based
    hld.update_path(u, v, x);

    // OPERATION: Path Query
    // "Find sum of nodes on path between u and v"
    u--; v--; // Convert to 0-based
    cout << hld.query_path(u, v) << "\n";

    // OPERATION: Subtree Update
    // "Add x to all nodes in subtree of u"
    int u, x;
    cin >> u >> x;
    u--;
    hld.update_subtree(u, x);

    // OPERATION: Subtree Query
    // "Find sum of nodes in subtree of u"
    u--;

    cout << hld.query_subtree(u) << "\n";

    // OPERATION: Lowest Common Ancestor
    u--; v--;
    // +1 because we output 1-based index
    cout << hld.lca(u, v) + 1 << "\n";


*/

/*

// --- MODIFIED OPERATIONS FOR EDGE WEIGHTS ---

struct EdgeInput {
    int u, v, w, id;
};

    HLD hld(n);

    // Store edges to handle weights later
    vector<EdgeInput> edges;

    for (int i = 0; i < n - 1; i++) {
        int u, v, w;
        cin >> u >> v >> w; // Read edge with weight
        u--; v--; // 0-based
        hld.add_edge(u, v);
        edges.push_back({u, v, w, i});
    }

    // 1. Build HLD structure FIRST (Calculates depths)
    hld.init();

    // 2. Assign Initial Weights to Nodes
    // We must assign weight 'w' to the node that is deeper (the child).
    for (auto& e : edges) {
        if (hld.depth[e.u] > hld.depth[e.v]) {
            // u is deeper, so u represents this edge
            hld.update_path_edge(e.u, e.u, e.w);
        } else {
            // v is deeper, so v represents this edge
            hld.update_path_edge(e.v, e.v, e.w);
        }
    }


    // Update specific edge index 'k' to value 'x'
    // "Change weight of the i-th edge given in input"
    int k, x;
    cin >> k >> x;
    k--; // 0-based index of edge list

    int u = edges[k].u;
    int v = edges[k].v;

    // Find which node holds the weight (the deeper one)
    int node = (hld.depth[u] > hld.depth[v]) ? u : v;

    // Update that node. Note: If we are REPLACING value,
    // the segment tree update logic might need to be "set" not "add".
    // If "add", use update_path_edge(node, node, x).
    // If "replace", you might need a different SegTree function or:
    // hld.update_path_edge(node, node, x - current_val);

    // Assuming "Add x" for this snippet:
    hld.update_path_edge(node, node, x);
}

    // Path Query
    int u, v;
    cin >> u >> v;
    u--; v--;
    cout << hld.query_path_edge(u, v) << "\n";

*/