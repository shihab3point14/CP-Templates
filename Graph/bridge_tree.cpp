// O(V + E)
const int N = 3e6 + 5;
int n, m, timer;

// Find Bridge
vector<vector<pair<int,int>>> adj(N); // adj[u] = {v, edge_id}
vector<bool> visited, is_bridge;
vector<int> tin, low, comp;
vector<pair<int, int>> bridges;

void dfs(int u, int p_edge = -1) {
    visited[u] = true;
    tin[u] = low[u] = timer++;
    for (auto [v, eid] : adj[u]) {
        if (eid == p_edge) continue;
        if (visited[v]) {
            low[u] = min(low[u], tin[v]);
        } else {
            dfs(v, eid);
            low[u] = min(low[u], low[v]);
            if (low[v] > tin[u]) {
                is_bridge[eid] = true;
                bridges.push_back({u, v});
            }
        }
    }
}

void find_bridges() {
    timer = 0;
    visited.assign(n, false);
    tin.assign(n, -1);
    low.assign(n, -1);
    is_bridge.assign(m, false);
    for (int i = 0; i < n; ++i)
        if (!visited[i])
            dfs(i);
}

void dfs_comp(int u, int c) {
    comp[u] = c;
    for (auto [v, eid] : adj[u]) {
        if (comp[v] != -1 || is_bridge[eid]) continue;
        dfs_comp(v, c);
    }
}

// Build Bridge-Tree
vector<vector<int>> tree;

void build_bridge_tree() {
    comp.assign(n, -1);
    int cid = 0;
    for (int i = 0; i < n; ++i) {
        if (comp[i] == -1)
            dfs_comp(i, cid++);
    }

    tree.resize(cid);
    for (auto [u, v] : bridges) {
        int cu = comp[u], cv = comp[v];
        if (cu != cv) {
            tree[cu].push_back(cv);
            tree[cv].push_back(cu);
        }
    }
}

// input
// int eid = 0;
// for (int i = 0; i < m; ++i) {
//     int u, v;
//     cin >> u >> v;
//     adj[u].push_back({v, eid});
//     adj[v].push_back({u, eid});
//     eid++;
// }