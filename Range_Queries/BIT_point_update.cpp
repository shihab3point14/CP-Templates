
// =========================================================
// FENWICK TREE (Binary Indexed Tree)
// 0-based API: Pass indices from 0 to N-1.
// Complexity: Point Update O(log N), Range Query O(log N)
// =========================================================
template <typename T>
struct FenwickTree {
    int n;
    vector<T> tree;

    // 1. Empty Constructor: Initializes with zeros
    FenwickTree(int _n) {
        n = _n;
        tree.assign(n + 1, 0); // 1-based internally
    }

    // 2. O(N) Constructor: Builds the tree from an array in linear time
    FenwickTree(const vector<T>& a) {
        n = a.size();
        tree.assign(n + 1, 0);
        for (int i = 0; i < n; i++) {
            tree[i + 1] += a[i];
            int parent = (i + 1) + ((i + 1) & -(i + 1));
            if (parent <= n) {
                tree[parent] += tree[i + 1];
            }
        }
    }

    // Point Update: Adds 'delta' to element at index 'i' (0-based)
    void add(int i, T delta) {
        for (++i; i <= n; i += i & -i) {
            tree[i] += delta;
        }
    }

    // Prefix Query: Returns sum of elements from index 0 to 'i' (inclusive)
    T query(int i) {
        T sum = 0;
        for (++i; i > 0; i -= i & -i) {
            sum += tree[i];
        }
        return sum;
    }

    // Range Query: Returns sum of elements from index 'l' to 'r' (inclusive)
    T query(int l, int r) {
        if (l > r || l < 0) return 0;
        return query(r) - query(l - 1);
    }

    // Binary Lifting: Finds the smallest 0-based index where the prefix sum >= 'target'
    // Requires all array elements to be >= 0. Complexity: O(log N)
    int lower_bound(T target) {
        int pos = 0;
        // Get the highest power of 2 less than or equal to n
        int max_pw = 1 << (31 - __builtin_clz(n)); 
        
        for (int pw = max_pw; pw > 0; pw >>= 1) {
            if (pos + pw <= n && tree[pos + pw] < target) {
                pos += pw;
                target -= tree[pos];
            }
        }
        return pos; // Returns 0-based index. If sum is never reached, returns n.
    }
};

// =========================================================
// EXAMPLE USAGE
// =========================================================
/*
int main() {
    vector<long long> a = {1, 3, 5, 2, 8}; // Size 5
    FenwickTree<long long> bit(a); // O(N) build

    // 1. Range Sum
    cout << bit.query(1, 3) << "\n"; // Sum of a[1..3] -> 3+5+2 = 10

    // 2. Point Update
    bit.add(2, 4); // a[2] becomes 5 + 4 = 9
    
    // 3. Find K-th element (if BIT stores frequencies)
    // Example: bit.lower_bound(target) 
}
*/