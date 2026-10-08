// =========================================================
// DSU with Rollback
// Complexity: O(log N) per operation
// Use for: Dynamic Connectivity, Divide & Conquer on Queries
// =========================================================
struct DSU_Rollback {
    vector<int> parent;
    vector<int> size;
    int num_components;
    vector<pair<int, int>> history; // Stores {child, parent}

    DSU_Rollback(int n) {
        parent.resize(n + 1);
        size.assign(n + 1, 1);
        num_components = n;
        iota(parent.begin(), parent.end(), 0);
    }

    // Find WITHOUT Path Compression (Preserves structure for rollback)
    int find(int x) {
        if (x == parent[x]) return x;
        return find(parent[x]);
    }

    bool unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX != rootY) {
            if (size[rootX] < size[rootY]) swap(rootX, rootY);
            
            // Save state before modifying
            history.push_back({rootY, rootX}); 
            
            parent[rootY] = rootX;
            size[rootX] += size[rootY];
            num_components--;
            return true;
        }
        // Add dummy entry to keep history synced with queries if needed
        history.push_back({-1, -1}); 
        return false;
    }

    // Undo the last unite operation
    void rollback() {
        if (history.empty()) return;
        pair<int, int> last = history.back();
        history.pop_back();
        
        if (last.first != -1) {
            int child = last.first;
            int root = last.second;
            parent[child] = child;     // Revert parent
            size[root] -= size[child]; // Revert size
            num_components++;
        }
    }
    
    // Save current state (returns index)
    int snapshot() {
        return history.size();
    }
    
    // Revert to a previous snapshot
    void rollback_to(int snap_idx) {
        while(history.size() > snap_idx) {
            rollback();
        }
    }
};