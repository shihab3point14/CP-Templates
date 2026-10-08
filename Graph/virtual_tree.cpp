#include <bits/stdc++.h>
using namespace std;

const int MAXN = 300005;
const int LOG = 20;

// LCA and Tree Data
int timer_dfs = 0;
int tin[MAXN], tout[MAXN], depth[MAXN];
int up[MAXN][LOG];

// Graphs
vector<int> adj[MAXN];         // Original tree
vector<int> vadj[MAXN];        // Virtual tree (directed top-down)
vector<int> active_nodes;      // Tracks nodes to clear in O(K)

// 1. Precompute LCA (Call once from main)
void dfs_lca(int u, int p = 1, int d = 0) {
    tin[u] = ++timer_dfs;
    depth[u] = d;
    up[u][0] = p;
    for (int i = 1; i < LOG; i++) {
        up[u][i] = up[up[u][i - 1]][i - 1];
    }
    for (int v : adj[u]) {
        if (v != p) dfs_lca(v, u, d + 1);
    }
    tout[u] = timer_dfs;
}

bool is_ancestor(int u, int v) {
    return tin[u] <= tin[v] && tout[u] >= tout[v];
}

int get_lca(int u, int v) {
    if (is_ancestor(u, v)) return u;
    if (is_ancestor(v, u)) return v;
    for (int i = LOG - 1; i >= 0; i--) {
        if (!is_ancestor(up[u][i], v)) {
            u = up[u][i];
        }
    }
    return up[u][0];
}

// 2. Safely clear the virtual tree in O(K)
void clear_vt() {
    for (int u : active_nodes) {
        vadj[u].clear();
    }
    active_nodes.clear();
}

// 3. Build the virtual tree and return its root
int build_vt(vector<int> nodes) {
    clear_vt(); // Clean up from the previous query
    if (nodes.empty()) return 0;

    // Sort by DFS entry time
    auto cmp = [](int a, int b) { return tin[a] < tin[b]; };
    sort(nodes.begin(), nodes.end(), cmp);

    // Insert LCAs of adjacent nodes
    int k = nodes.size();
    for (int i = 0; i < k - 1; i++) {
        nodes.push_back(get_lca(nodes[i], nodes[i + 1]));
    }

    // Sort again and remove duplicates
    sort(nodes.begin(), nodes.end(), cmp);
    nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());
    
    // Save these nodes to clear them on the next query
    active_nodes = nodes; 

    // Build directed edges using a stack
    vector<int> stk;
    stk.push_back(nodes[0]);
    for (int i = 1; i < nodes.size(); i++) {
        int u = nodes[i];
        // Pop while stack top is NOT an ancestor of u
        while (stk.size() >= 2 && !is_ancestor(stk.back(), u)) {
            stk.pop_back();
        }
        vadj[stk.back()].push_back(u);
        stk.push_back(u);
    }

    // Return the root of the virtual tree
    return nodes[0];
}