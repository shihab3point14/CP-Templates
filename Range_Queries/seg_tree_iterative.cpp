/**
 * Iterative Segment Tree with Lazy Propagation
 * Supports: Range Update (Add) and Range Query (Sum)
 * Complexity: Build O(N), Update O(log N), Query O(log N)
 * Space: O(2 * MAXN)
 */
struct SegmentTree {
    int n, h;
    vector<long long> t, lazy;

    SegmentTree(int _n) {
        n = _n;
        h = 32 - __builtin_clz(n); // Height of the tree
        t.assign(2 * n, 0);
        lazy.assign(n, 0); // Lazy array only needed for parent nodes
    }

    // Combines two nodes. Change this for different operations (e.g., max, min).
    long long combine(long long a, long long b) {
        return a + b;
    }

    // Applies a lazy value to a node.
    // k is the size of the segment represented by node p.
    void apply(int p, long long value, int k) {
        t[p] += value * k;
        // For parent nodes, also update the lazy array.
        if (p < n) {
            lazy[p] += value;
        }
    }

    // Updates parent nodes by combining children values, from `p` up to the root.
    void build_up(int p) {
        int k = 2;
        while (p > 1) {
            p >>= 1;
            // Combined value of children + lazy value of the parent segment
            t[p] = combine(t[p << 1], t[p << 1 | 1]) + lazy[p] * k;
            k <<= 1;
        }
    }

    // Pushes lazy values down from a parent `p` to its children.
    // We go from the root down to the level above the leaves.
    void push(int p) {
        for (int s = h; s > 0; --s) {
            int i = p >> s;
            if (lazy[i] != 0) {
                int k = 1 << (s - 1); // Size of the children segments
                apply(i << 1, lazy[i], k);
                apply(i << 1 | 1, lazy[i], k);
                lazy[i] = 0;
            }
        }
    }

    // Build the segment tree from an initial array.
    void build(const vector<long long>& a) {
        for (int i = 0; i < n; ++i) t[n + i] = a[i];
        for (int i = n - 1; i > 0; --i) t[i] = combine(t[i << 1], t[i << 1 | 1]);
    }

    // Add `value` to all elements in the range [l, r). Note: r is exclusive.
    void update_range(int l, int r, long long value) {
        l += n; r += n;
        int l0 = l, r0 = r;
        int k = 1;
        for (; l < r; l >>= 1, r >>= 1, k <<= 1) {
            if (l & 1) apply(l++, value, k);
            if (r & 1) apply(--r, value, k);
        }
        // After updating, we need to rebuild the parent nodes from the changed leaves.
        build_up(l0);
        build_up(r0 - 1);
    }

    // Query the range [l, r). Note: r is exclusive.
    long long query_range(int l, int r) {
        l += n; r += n;
        // Before querying, push all lazy values that affect our range.
        push(l);
        push(r - 1);

        long long res = 0; // Identity for sum (0). Change for MAX (-INF) or MIN (INF).
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) res = combine(res, t[l++]);
            if (r & 1) res = combine(res, t[--r]);
        }
        return res;
    }
};

/**
 * --- How to use this struct ---
 * 1. Initialize: SegmentTree st(n);
 * 2. Build: st.build(initial_vector);
 * 3. Range Update [L, R]: st.update_range(L, R + 1, val); 
 * 4. Range Query [L, R]: st.query_range(L, R + 1);
 * (Note: Uses 0-based indexing for the logic, r is exclusive).
 */