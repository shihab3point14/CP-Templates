/**
 * Prim's Algorithm for Minimum Spanning Tree (MST)
 * Time Complexity: O(E log E) or O(E log V)
 * Space Complexity: O(V + E)
 */
struct PrimMST {
    int V;
    // Standard adjacency list: adj[u] = { {v, weight}, ... }
    vector<list<pair<int, int>>> adj;

    PrimMST(int n) : V(n), adj(n) {}

    void add_edge(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); // Prim's is typically for undirected graphs
    }

    /**
     * Returns a pair containing:
     * 1. Total weight of the MST
     * 2. List of edges in the MST as {u, v, weight}
     */
    pair<int, vector<tuple<int, int, int>>> get_mst(int start_node = 0) {
        priority_queue<tuple<int, int, int>, 
                       vector<tuple<int, int, int>>, 
                       greater<tuple<int, int, int>>> pq;
        
        vector<bool> visited(V, false);
        vector<tuple<int, int, int>> mst_edges;
        int total_weight = 0;

        // {weight, current_node, parent}
        pq.emplace(0, start_node, -1);

        while (!pq.empty()) {
            auto [w, u, par] = pq.top();
            pq.pop();

            if (visited[u]) continue;
            visited[u] = true;
            total_weight += w;

            // Don't add the dummy parent of the root
            if (par != -1) {
                mst_edges.emplace_back(par, u, w);
            }

            for (auto& edge : adj[u]) {
                int v = edge.first;
                int weight = edge.second;
                if (!visited[v]) {
                    pq.emplace(weight, v, u);
                }
            }
        }

        // Optional: If MST edges count < V-1, the graph is disconnected
        return {total_weight, mst_edges};
    }

    /**
     * Specialized: Only returns the total weight
     */
    int get_weight(int start_node = 0) {
        return get_mst(start_node).first;
    }

    /**
     * Specialized: Only returns the parent array
     * parent[i] is the node that connected i to the MST
     */
    vector<int> get_parents(int start_node = 0) {
        auto [weight, edges] = get_mst(start_node);
        vector<int> parent(V, -1);
        for (auto& [u, v, w] : edges) {
            parent[v] = u;
        }
        return parent;
    }
};