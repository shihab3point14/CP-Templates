
// =========================================================
// SPARSE TABLE SNIPPET 
// =========================================================

template <typename T>
struct SparseTable {
    int n;
    int max_log;
    vector<vector<T>> st;
    vector<int> logs;

    // -----------------------------------------------------
    // 1. CONFIGURATION: CHANGE THIS FUNCTION FOR DIFFERENT PROBLEMS
    // -----------------------------------------------------
    // Common operations:
    // min(a, b) -> Range Minimum Query (RMQ)
    // max(a, b) -> Range Maximum Query
    // __gcd(a, b) -> Range GCD Query
    // (a | b)     -> Range Bitwise OR
    // (a & b)     -> Range Bitwise AND
    T merge(T a, T b) {
        return min(a, b); // <--- CHANGE OPERATION HERE
    }

    // Identity element for cascade queries (only needed for query_cascade)
    // For min -> use INF (2e9 or 1e18)
    // For max -> use -INF
    // For sum -> use 0
    // For gcd -> use 0
    T identity_element = 2e18; // <---  CHANGE HERE

    // Constructor
    SparseTable(const vector<T>& arr) {
        n = arr.size();
        if (n == 0) return;
        max_log = floor(log2(n));
        st.assign(n, vector<T>(max_log + 1));
        logs.assign(n + 1, 0);

        // Precompute logs for O(1) lookup
        logs[1] = 0;
        for (int i = 2; i <= n; i++)
            logs[i] = logs[i / 2] + 1;

        // Base case: intervals of length 1 (level 0)
        for (int i = 0; i < n; i++)
            st[i][0] = arr[i];

        // Build the table: intervals of length 2^j
        for (int j = 1; j <= max_log; j++) {
            for (int i = 0; i + (1 << j) <= n; i++) {
                st[i][j] = merge(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]);
            }
        }
    }

    // -----------------------------------------------------
    // TYPE A: IDEMPOTENT QUERY - O(1)
    // Use this for: Min, Max, GCD, OR, AND
    // -----------------------------------------------------
    T query(int L, int R) {
        if (L > R) return identity_element; // Handle empty/invalid range
        int j = logs[R - L + 1];
        T res = merge(st[L][j], st[R - (1 << j) + 1][j]);
        return res;
    }

    // -----------------------------------------------------
    // TYPE B: CASCADE QUERY - O(log N)
    // Use this for: Sum, Product, XOR, Matrix Multiplication
    // (Operations where overlapping ranges would ruin the result)
    // -----------------------------------------------------
    T query_cascade(int L, int R) {
        if (L > R) return identity_element;
        T res = identity_element; 
        bool first = true; // Helper to handle the very first element

        for (int j = max_log; j >= 0; j--) {
            if ((1 << j) <= R - L + 1) {
                if (first) {
                    res = st[L][j];
                    first = false;
                } else {
                    res = merge(res, st[L][j]);
                }
                L += (1 << j);
            }
        }
        return res;
    }
};
// =========================================================
// END SNIPPET
// =========================================================

/*
// Example Usage
int main() {
    // 1. Setup Data
    vector<long long> arr = {1, 3, 5, 2, 4, 8, 1};

    // 2. Build Table
    SparseTable<long long> st(arr);

    // 3. Example 1: Range Minimum (Standard)
    // Range [1, 4] -> {3, 5, 2, 4} -> Min is 2
    cout << "Min Query: " << st.query(1, 4) << endl;

    // 4. Example 2: Cascade Query
    // If you changed merge() to Sum, this would give sum of range
    // Since merge is min, it still gives min, but calculates it differently.
    cout << "Cascade Query: " << st.query_cascade(1, 4) << endl;

    return 0;
}
*/