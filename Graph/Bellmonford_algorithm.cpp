
struct Edge {
    int u, v, weight;
};

/**
 * Bellman-Ford Algorithm
 * Time Complexity: O(V * E)
 * Space Complexity: O(V)
 */
struct BellmanFord {
    int n;
    vector<int> dist;
    bool hasNegativeCycle = false;

    BellmanFord(int n) : n(n), dist(n + 1, inf) {}

    // source: starting node
    // edges: list of all edges in the graph
    void solve(int source, const vector<Edge>& edges) {
        dist[source] = 0;

        // Relax all edges N-1 times
        for (int i = 1; i <= n - 1; i++) {
            bool changed = false;
            for (const auto& e : edges) {
                if (dist[e.u] != inf && dist[e.u] + e.weight < dist[e.v]) {
                    dist[e.v] = dist[e.u] + e.weight;
                    changed = true;
                }
            }
            if (!changed) break; // Optimization: stop if no updates
        }

        // N-th relaxation to check for negative cycles
        for (const auto& e : edges) {
            if (dist[e.u] != inf && dist[e.u] + e.weight < dist[e.v]) {
                hasNegativeCycle = true;
                break;
            }
        }
    }
};