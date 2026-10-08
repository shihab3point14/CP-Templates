/**
 * LCA using Binary Lifting
 * Build: O(N log N)
 * Query: O(log N)
 */
struct LCA {
    int n, l, timer;
    vector<int> tin, tout, depth;
    vector<vector<int>> up;

    LCA(int _n) : n(_n) {
        l = ceil(log2(n + 1));
        tin.resize(n + 1);
        tout.resize(n + 1);
        depth.assign(n + 1, 0);
        up.assign(n + 1, vector<int>(l + 1));
        timer = 0;
    }

    void dfs(const vector<vector<int>>& adj, int v, int p, int d = 0) {
        tin[v] = ++timer;
        depth[v] = d;
        up[v][0] = p;
        for (int i = 1; i <= l; ++i)
            up[v][i] = up[up[v][i - 1]][i - 1];

        for (int u : adj[v]) {
            if (u != p) dfs(adj, u, v, d + 1);
        }
        tout[v] = ++timer;
    }

    bool is_ancestor(int u, int v) {
        return tin[u] <= tin[v] && tout[u] >= tout[v];
    }

    int get_lca(int u, int v) {
        if (is_ancestor(u, v)) return u;
        if (is_ancestor(v, u)) return v;
        for (int i = l; i >= 0; --i) {
            if (!is_ancestor(up[u][i], v))
                u = up[u][i];
        }
        return up[u][0];
    }

    // Returns the distance between nodes u and v
    int dist(int u, int v) {
        return depth[u] + depth[v] - 2 * depth[get_lca(u, v)];
    }

    // Returns the k-th ancestor of node u
    int get_kth_ancestor(int u, int k) {
        for (int i = 0; i <= l; i++) {
            if ((k >> i) & 1) u = up[u][i];
        }
        return u;
    }
};