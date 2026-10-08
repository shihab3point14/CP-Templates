struct PersistentDSU {
    vector<int> parent, sz, time_joined;
    int current_time;

    PersistentDSU(int n) {
        parent.resize(n);
        iota(parent.begin(), parent.end(), 0);
        sz.assign(n, 1);
        time_joined.assign(n, 1e9); // Infinity
        current_time = 0;
    }

    // Find the root at a specific point in time
    int find(int i, int t) {
        while (i != parent[i] && time_joined[i] <= t) {
            i = parent[i];
        }
        return i;
    }

    void unite(int u, int v) {
        current_time++; // Increment global timer
        u = find(u, current_time);
        v = find(v, current_time);
        if (u == v) return;

        if (sz[u] < sz[v]) swap(u, v);
        
        parent[v] = u;
        sz[u] += sz[v];
        time_joined[v] = current_time; // Record when v attached to u
    }

    // Check if u and v were in the same component at time t
    bool same_set_at_time(int u, int v, int t) {
        return find(u, t) == find(v, t);
    }
};