// ==========================================
// CONFIGURATION & MEMORY
// ==========================================
// Q: How much memory? 
// A: Estimate Q * log(Range) * 2. 
//    For Q=2e5, Range=1e9, use ~6,000,000.
const int MAX_NODES = 6000000; 

struct DynamicSegmentTree {
    // -----------------------------------------------------------
    // GLOBAL DATA POOL
    // 0 is reserved as the "NULL" node.
    // -----------------------------------------------------------
    long long tree[MAX_NODES], lazy[MAX_NODES];
    int L[MAX_NODES], R[MAX_NODES];
    int ptr; // Tracks the next free index
    long long n;

    DynamicSegmentTree(long long _n) {
        n = _n;
        ptr = 1; // Start allocating from index 1
        
        // NOTE: If using multiple test cases, you MUST clear the arrays here.
        // memset(L, 0, sizeof(L)); 
        // memset(R, 0, sizeof(R));
        // memset(tree, 0, sizeof(tree)); 
        // memset(lazy, 0, sizeof(lazy));
    }

    // -----------------------------------------------------------
    // MERGE TWO TREES (u <- u + v)
    // Merges tree 'v' into tree 'u' and returns the new root.
    // -----------------------------------------------------------
    int merge(int u, int v, long long tl, long long tr) {
        // CASE 1: If one node is missing, return the other.
        // This preserves the sparse structure (O(1)).
        if (!u || !v) return u ? u : v;

        // CASE 2: Leaf Node - Combine values
        if (tl == tr) {
            // [CHANGE HERE] How to combine two overlapping leaves?
            
            tree[u] += tree[v]; 
            // For MAX: tree[u] = max(tree[u], tree[v]);
            // For SET: tree[u] = tree[v]; // (if v dominates u)
            
            return u;
        }

        // WARNING: If you use Lazy Propagation, you technically need to 
        // push(u, tl, tr) and push(v, tl, tr) here. 
        // However, Segment Tree Merging is SLOW with Range Updates.
        // It is strongly recommended to use this only with POINT UPDATES.

        long long tm = tl + (tr - tl) / 2;

        // Recursively merge children
        L[u] = merge(L[u], L[v], tl, tm);
        R[u] = merge(R[u], R[v], tm + 1, tr);

        // [CHANGE HERE] Pull Up (Maintenance)
        tree[u] = tree[L[u]] + tree[R[u]];
        // tree[u] = max(tree[L[u]], tree[R[u]]);
        
        return u;
    }

    // -----------------------------------------------------------
    // PUSH (LAZY PROPAGATION)
    // -----------------------------------------------------------
    void push(int v, long long tl, long long tr) {
        if (lazy[v] == 0) return;

        long long tm = tl + (tr - tl) / 2;

        // 1. DYNAMIC CREATION: Create children if they don't exist
        if (!L[v]) L[v] = ++ptr;
        if (!R[v]) R[v] = ++ptr;

        // 2. APPLY LAZY TO CHILDREN
        // [CHANGE HERE] based on problem type:
        
        // --- CASE: Range Add + Range Sum ---
        tree[L[v]] += lazy[v] * (tm - tl + 1); // Add value * length
        tree[R[v]] += lazy[v] * (tr - tm);     // Add value * length
        
        // --- CASE: Range Add + Range Max/Min ---
        // tree[L[v]] += lazy[v]; // Length doesn't matter for Max/Min
        // tree[R[v]] += lazy[v]; 

        // --- CASE: Range Set (Assignment) ---
        // tree[L[v]] = lazy[v] * (tm - tl + 1);
        // tree[R[v]] = lazy[v] * (tr - tm);
        // bool is_set[MAX_NODES]; // You might need a separate flag for "is_set"
        
        // 3. PROPAGATE LAZY VALUE
        lazy[L[v]] += lazy[v];
        lazy[R[v]] += lazy[v];
        // For Range Set: lazy[L[v]] = lazy[v];

        // Reset current node lazy
        lazy[v] = 0;
    }

    // -----------------------------------------------------------
    // UPDATE RANGE
    // -----------------------------------------------------------
    void update_range(int v, long long tl, long long tr, long long l, long long r, long long add_val) {
        if (l > r) return;

        // Leaf / Fully covered range
        if (l == tl && r == tr) {
            // [CHANGE HERE] How the value is applied
            
            // For SUM: increment by val * length
            tree[v] += add_val * (tr - tl + 1); 
            
            // For MAX/MIN: increment by val
            // tree[v] += add_val; 

            lazy[v] += add_val;
        } else {
            push(v, tl, tr);
            long long tm = tl + (tr - tl) / 2;

            if (!L[v]) L[v] = ++ptr;
            if (!R[v]) R[v] = ++ptr;

            update_range(L[v], tl, tm, l, min(r, tm), add_val);
            update_range(R[v], tm + 1, tr, max(l, tm + 1), r, add_val);

            // [CHANGE HERE] PULL UP (Combine logic)
            tree[v] = tree[L[v]] + tree[R[v]];       // For SUM
            // tree[v] = max(tree[L[v]], tree[R[v]]); // For MAX
            // tree[v] = min(tree[L[v]], tree[R[v]]); // For MIN
            // tree[v] = __gcd(tree[L[v]], tree[R[v]]); // For GCD
        }
    }

    // -----------------------------------------------------------
    // QUERY
    // -----------------------------------------------------------
    long long query_sum(int v, long long tl, long long tr, long long l, long long r) {
        // [CHANGE HERE] Identity Element (Base Case)
        // If node doesn't exist (v==0) or range invalid
        if (l > r || v == 0) {
            return 0;         // For SUM
            // return -1e18;  // For MAX
            // return 1e18;   // For MIN
        }

        if (l == tl && r == tr) return tree[v];

        push(v, tl, tr);
        long long tm = tl + (tr - tl) / 2;

        // [CHANGE HERE] Combine Logic
        return query_sum(L[v], tl, tm, l, min(r, tm)) + 
               query_sum(R[v], tm + 1, tr, max(l, tm + 1), r);
               
        // return max(query(...), query(...)); // For MAX
        // return min(query(...), query(...)); // For MIN
    }

    // -----------------------------------------------------------
    // HELPER FUNCTIONS (User Interface)
    // -----------------------------------------------------------
    void update(long long l, long long r, long long val) {
        update_range(1, 0, n - 1, l, r, val);
    }

    long long query(long long l, long long r) {
        return query_sum(1, 0, n - 1, l, r);
    }
};

// ==========================================
// HOW TO USE IT
// ==========================================

// 1. Declare Globally to avoid Stack Overflow
//    Range is 0 to 10^9 (1 Billion)
DynamicSegmentTree st(1000000000); 

int main() {
    // 2. Update Range [4, 10] adding 5 to each index
    //    Indices: 4, 5, 6, 7, 8, 9, 10 (7 indices)
    //    Total Sum added: 7 * 5 = 35
    st.update(4, 10, 5);

    // 3. Update Range [8, 20000] adding 2
    st.update(8, 20000, 2);

    // 4. Query Sum [1, 10]
    //    Index 4-7: 5
    //    Index 8-10: 5 + 2 = 7
    //    Sum: (4 * 5) + (3 * 7) = 20 + 21 = 41
    cout << "Sum 1-10: " << st.query(1, 10) << endl;

    return 0;
}