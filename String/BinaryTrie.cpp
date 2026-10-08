const int MAX_BITS = 30; // 30 for up to 10^9, 60 for 10^18
const int MAX_NODES = 100005 * (MAX_BITS + 1);

struct BinaryTrie {
    int trie[MAX_NODES][2];
    int cnt[MAX_NODES];
    int node_cnt;

    BinaryTrie() {
        node_cnt = 0;
        clear_node(0);
    }

    inline void clear_node(int v) {
        trie[v][0] = trie[v][1] = 0;
        cnt[v] = 0;
    }

    void reset() {
        for (int i = 0; i <= node_cnt; i++) {
            clear_node(i);
        }
        node_cnt = 0;
    }

    void insert(int num) {
        int node = 0;
        cnt[node]++; // Track total elements in root
        for (int i = MAX_BITS; i >= 0; i--) {
            int bit = (num >> i) & 1;
            if (!trie[node][bit]) {
                trie[node][bit] = ++node_cnt;
                clear_node(node_cnt);
            }
            node = trie[node][bit];
            cnt[node]++;
        }
    }

    void erase(int num) {
        int node = 0;
        cnt[node]--;
        for (int i = MAX_BITS; i >= 0; i--) {
            int bit = (num >> i) & 1;
            node = trie[node][bit];
            cnt[node]--;
        }
    }

    int get_max_xor(int num) {
        if (cnt[0] == 0) return -1; // Empty trie check
        int node = 0, ans = 0;
        for (int i = MAX_BITS; i >= 0; i--) {
            int bit = (num >> i) & 1;
            int desired = 1 - bit;
            
            if (trie[node][desired] && cnt[trie[node][desired]] > 0) {
                ans |= (1 << i);
                node = trie[node][desired];
            } else {
                node = trie[node][bit];
            }
        }
        return ans;
    }

    int get_min_xor(int num) {
        if (cnt[0] == 0) return -1;
        int node = 0, ans = 0;
        for (int i = MAX_BITS; i >= 0; i--) {
            int bit = (num >> i) & 1;
            int desired = bit; // For min XOR, we want the SAME bit
            
            if (trie[node][desired] && cnt[trie[node][desired]] > 0) {
                node = trie[node][desired];
            } else {
                ans |= (1 << i); // Forced to take opposite bit, XOR gets a 1
                node = trie[node][1 - desired];
            }
        }
        return ans;
    }

    // Finds the K-th LARGEST XOR value possible with 'num'
    int get_kth_max_xor(int num, int k) {
        if (k > cnt[0]) return -1; // K exceeds available elements
        int node = 0, ans = 0;
        for (int i = MAX_BITS; i >= 0; i--) {
            int bit = (num >> i) & 1;
            int desired = 1 - bit; // Prefer opposite bit for MAX
            
            int next_node = trie[node][desired];
            int count_in_desired = next_node ? cnt[next_node] : 0;
            
            if (k <= count_in_desired) {
                ans |= (1 << i);
                node = next_node;
            } else {
                k -= count_in_desired; // Skip these elements
                node = trie[node][bit];
            }
        }
        return ans;
    }

    // Finds the K-th SMALLEST XOR value possible with 'num'
    int get_kth_min_xor(int num, int k) {
        if (k > cnt[0]) return -1;
        int node = 0, ans = 0;
        for (int i = MAX_BITS; i >= 0; i--) {
            int bit = (num >> i) & 1;
            int desired = bit; // Prefer same bit for MIN
            
            int next_node = trie[node][desired];
            int count_in_desired = next_node ? cnt[next_node] : 0;
            
            if (k <= count_in_desired) {
                node = next_node;
            } else {
                k -= count_in_desired;
                ans |= (1 << i);
                node = trie[node][1 - bit];
            }
        }
        return ans;
    }

    // Counts how many elements 'x' in the Trie satisfy (num ^ x) < limit
    // Crucial for problems asking for pairs with XOR condition
    int count_xor_less_than(int num, int limit) {
        int node = 0, count = 0;
        for (int i = MAX_BITS; i >= 0; i--) {
            if (!node) break;
            
            int bit = (num >> i) & 1;
            int limit_bit = (limit >> i) & 1;
            
            if (limit_bit == 1) {
                // If limit bit is 1, choosing 'bit' makes XOR 0, which is strictly less.
                // We add all elements from the 'bit' branch.
                if (trie[node][bit]) count += cnt[trie[node][bit]];
                // Then move down the '1 - bit' branch to keep matching the limit prefix.
                node = trie[node][1 - bit];
            } else {
                // If limit bit is 0, we MUST choose 'bit' to keep XOR at 0.
                node = trie[node][bit];
            }
        }
        return count;
    }
};