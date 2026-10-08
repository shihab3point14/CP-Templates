
// =========================================================
// DSU (Disjoint Set Union)
// Complexity: O(alpha(N)) ~ O(1) per operation
// Features: Path Compression, Union by Size, Component Count
// =========================================================
struct DSU {
    vector<int> parent;
    vector<int> size;
    int num_components; // Tracks total number of disjoint sets

    // Constructor: 1-based or 0-based compatible
    DSU(int n) {
        parent.resize(n + 1);
        size.assign(n + 1, 1);
        num_components = n;
        
        // Initialize parent[i] = i
        iota(parent.begin(), parent.end(), 0);
    }

    // Find with Path Compression
    int find(int x) {
        if (x == parent[x])
            return x;
        return parent[x] = find(parent[x]);
    }

    // Unite two sets. Returns true if they were merged, false if already same.
    bool unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX != rootY) {
            // Union by Size: Attach smaller tree to larger tree
            if (size[rootX] < size[rootY])
                swap(rootX, rootY);

            parent[rootY] = rootX;
            size[rootX] += size[rootY];
            num_components--;
            return true;
        }
        return false;
    }

    // Check if x and y are in the same component
    bool same(int x, int y) {
        return find(x) == find(y);
    }

    // Get the size of the component containing x
    int getSize(int x) {
        return size[find(x)];
    }
    
    // Optional: Reset DSU without reallocating memory
    void reset() {
        iota(parent.begin(), parent.end(), 0);
        fill(size.begin(), size.end(), 1);
        num_components = parent.size() - 1; // Adjust if 0-based
    }
};