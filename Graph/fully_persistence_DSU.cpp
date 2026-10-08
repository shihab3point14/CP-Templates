#include <bits/stdc++.h>
using namespace std;

struct FullyPersistentDSU {
    struct Node {
        int l, r;
        int par, sz;
    };
    
    vector<Node> tree;
    int n;

    FullyPersistentDSU(int n) : n(n) {
        // Reserve memory to prevent reallocations (N log N approx)
        tree.reserve(n * 40); 
    }

    // Build the initial segment tree (Version 0)
    int build(int l, int r) {
        int id = tree.size();
        tree.push_back({-1, -1, 0, 1});
        if (l == r) {
            tree[id].par = l;
            return id;
        }
        int mid = l + (r - l) / 2;
        tree[id].l = build(l, mid);
        tree[id].r = build(mid + 1, r);
        return id;
    }
    
    // Helper to start the base version
    int init() {
        return build(0, n - 1);
    }

    // Point update in persistent segment tree
    int update(int prev_id, int l, int r, int pos, int new_par, int new_sz) {
        int id = tree.size();
        tree.push_back(tree[prev_id]); // Copy previous state
        
        if (l == r) {
            tree[id].par = new_par;
            tree[id].sz = new_sz;
            return id;
        }
        
        int mid = l + (r - l) / 2;
        if (pos <= mid) {
            tree[id].l = update(tree[prev_id].l, l, mid, pos, new_par, new_sz);
        } else {
            tree[id].r = update(tree[prev_id].r, mid + 1, r, pos, new_par, new_sz);
        }
        return id;
    }

    // Point query to get {parent, size}
    pair<int, int> query(int id, int l, int r, int pos) {
        if (l == r) {
            return {tree[id].par, tree[id].sz};
        }
        int mid = l + (r - l) / 2;
        if (pos <= mid) return query(tree[id].l, l, mid, pos);
        return query(tree[id].r, mid + 1, r, pos);
    }

    // O(log^2 N) find without path compression
    int find(int version_root, int u) {
        while (true) {
            int p = query(version_root, 0, n - 1, u).first;
            if (p == u) return u;
            u = p;
        }
    }

    // O(log^2 N) unite. Returns the new root version of the persistent seg tree.
    int unite(int prev_root, int u, int v) {
        u = find(prev_root, u);
        v = find(prev_root, v);
        
        if (u == v) {
            return prev_root; // No structural change
        }

        int sz_u = query(prev_root, 0, n - 1, u).second;
        int sz_v = query(prev_root, 0, n - 1, v).second;

        // Union by size
        if (sz_u < sz_v) {
            swap(u, v);
            swap(sz_u, sz_v);
        }

        // 1. Attach v's root to u's root
        int new_root = update(prev_root, 0, n - 1, v, u, sz_v);
        
        // 2. Update the size of u's root
        new_root = update(new_root, 0, n - 1, u, u, sz_u + sz_v);
        
        return new_root;
    }
};

// Example usage:
// FullyPersistentDSU dsu(N);
// vector<int> version;
// version.push_back(dsu.init()); // Version 0
// 
// // Merge 1 and 2, create Version 1
// version.push_back(dsu.unite(version[0], 1, 2)); 
//
// // Check if 1 and 2 are connected in Version 0 (returns false)
// bool in_v0 = (dsu.find(version[0], 1) == dsu.find(version[0], 2));