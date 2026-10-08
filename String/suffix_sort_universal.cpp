
// Universal int-32bit
// O(nlogn)

struct SuffixArray {
    vector<int> s;       // Compressed Input
    int n;               // Length + Sentinel
    vector<int> p;       // SA: p[i] = Start index of i-th smallest suffix
    vector<int> c;       // Rank: c[i] = Rank of suffix starting at i
    vector<int> lcp;     // LCP: lcp[i] = LCP between SA[i] and SA[i-1]
    
    // --- EXTRA: SPARSE TABLE FOR O(1) LCP QUERIES ---
    // table[k][i] stores min value starting at i with length 2^k
    vector<vector<int>> rmq; 
    vector<int> logs;    // Precomputed logs for fast lookup

    // Constructor 1: String
    SuffixArray(string const& input_s) {
        for(char ch : input_s) s.push_back(ch);
        construct();
    }

    // Constructor 2: Vector
    SuffixArray(vector<int> input_v) {
        s = input_v;
        construct();
    }

    void construct() {
        // 1. Coordinate Compression (Safe for 10^9 values)
        {
            vector<int> sorted_s = s;
            sort(sorted_s.begin(), sorted_s.end());
            sorted_s.erase(unique(sorted_s.begin(), sorted_s.end()), sorted_s.end());
            for(int &x : s) x = lower_bound(sorted_s.begin(), sorted_s.end(), x) - sorted_s.begin() + 1;
        }

        // 2. Append Sentinel
        s.push_back(0); 
        n = s.size();

        // 3. Build SA (O(N log N) Counting Sort)
        p.resize(n); c.resize(n);
        vector<int> cnt(max(n, 256), 0), p_new(n), c_new(n);

        for (int i = 0; i < n; i++) cnt[s[i]]++;
        for (int i = 1; i < cnt.size(); i++) cnt[i] += cnt[i-1];
        for (int i = n - 1; i >= 0; i--) p[--cnt[s[i]]] = i;

        c[p[0]] = 0; int classes = 1;
        for (int i = 1; i < n; i++) {
            if (s[p[i]] != s[p[i-1]]) classes++;
            c[p[i]] = classes - 1;
        }

        int k = 0;
        while ((1 << k) < n) {
            int len = (1 << k);
            for (int i = 0; i < n; i++) {
                p_new[i] = p[i] - len;
                if (p_new[i] < 0) p_new[i] += n;
            }
            fill(cnt.begin(), cnt.begin() + classes, 0);
            for (int i = 0; i < n; i++) cnt[c[p_new[i]]]++;
            for (int i = 1; i < classes; i++) cnt[i] += cnt[i-1];
            for (int i = n - 1; i >= 0; i--) p[--cnt[c[p_new[i]]]] = p_new[i];

            c_new[p[0]] = 0; classes = 1;
            for (int i = 1; i < n; i++) {
                pair<int, int> cur = {c[p[i]], c[(p[i] + len) % n]};
                pair<int, int> prev = {c[p[i-1]], c[(p[i-1] + len) % n]};
                if (cur != prev) classes++;
                c_new[p[i]] = classes - 1;
            }
            c = c_new; k++;
        }
        
        build_lcp();
        build_rmq(); // Build Sparse Table immediately
    }

    void build_lcp() {
        lcp.assign(n, 0);
        int k = 0;
        for (int i = 0; i < n - 1; i++) {
            int pi = c[i];
            int j = p[pi - 1];
            while (s[i + k] == s[j + k]) k++;
            lcp[pi] = k;
            k = max(k - 1, 0);
        }
    }

    // --- EXTRA FUNCTION 1: Build Sparse Table (O(N log N)) ---
    // Allows us to answer Range Minimum Queries on the LCP array
    void build_rmq() {
        logs.resize(n + 1);
        logs[1] = 0;
        for (int i = 2; i <= n; i++) logs[i] = logs[i/2] + 1;

        int K = logs[n];
        rmq.assign(K + 1, vector<int>(n));

        // Initialize level 0
        for (int i = 0; i < n; i++) rmq[0][i] = lcp[i];

        // Fill table
        for (int j = 1; j <= K; j++) {
            for (int i = 0; i + (1 << j) <= n; i++) {
                rmq[j][i] = min(rmq[j-1][i], rmq[j-1][i + (1 << (j-1))]);
            }
        }
    }

    // --- EXTRA FUNCTION 2: Arbitrary LCP Query (O(1)) ---
    // Returns LCP of suffix starting at i and suffix starting at j
    int get_lcp(int i, int j) {
        if (i == j) return n - 1 - i; // Same suffix
        
        // Map original indices to their ranks in the SA
        int l = c[i];
        int r = c[j];
        if (l > r) swap(l, r);

        // We want RMQ on interval [l+1, r] in the LCP array
        // Why l+1? Because lcp[x] stores LCP(p[x], p[x-1]). 
        // The LCP of a range is the minimum LCP value between them.
        l++; 
        
        int k = logs[r - l + 1];
        return min(rmq[k][l], rmq[k][r - (1 << k) + 1]);
    }

    // --- EXTRA FUNCTION 3: Count Distinct Substrings (O(N)) ---
    long long count_distinct_substrings() {
        long long total = 0;
        // Iterate 1 to n-1 (skipping sentinel)
        for(int i = 1; i < n; i++) {
            // Length of suffix p[i] is (n - 1 - p[i])
            // We subtract lcp[i] because those prefixes are repeated from previous suffix
            total += (n - 1 - p[i]) - lcp[i];
        }
        return total;
    }
    
    // Clean getters
    vector<int> get_sa_clean() {
        vector<int> res;
        for(int i = 1; i < n; i++) res.push_back(p[i]);
        return res;
    }
};

/*
    // --- HOW TO USE EXTRA FUNCTIONS ---

    string s = "ababba";
    SuffixArray sa(s);

    // 1. Get Distinct Substrings
    cout << "Distinct Substrings: " << sa.count_distinct_substrings() << endl;

    // 2. Find LCP of two arbitrary suffixes
    // Index 0: "ababba"
    // Index 2: "abba"
    // They share "ab...", so LCP should be 2.
    int lcp_val = sa.get_lcp(0, 2);
    cout << "LCP(0, 2): " << lcp_val << endl;

    // Index 1: "babba"
    // Index 4: "ba"
    // They share "ba...", LCP should be 2.
    cout << "LCP(1, 4): " << sa.get_lcp(1, 4) << endl;
*/
