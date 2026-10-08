/**
 * Mo's Algorithm Struct
 * Offline range queries in O((N+Q) * sqrt(N))
 */
struct Query {
    int l, r, id, block;
    bool operator<(const Query& other) const {
        if (block != other.block) return block < other.block;
        // Optimization: Even blocks sort ascending, odd blocks sort descending
        return (block & 1) ? (r < other.r) : (r > other.r);
    }
};

struct Mo {
    int n, q, block_size;
    vector<int> arr;
    vector<long long> answers;
    long long current_ans = 0;

    // --- CHANGE STATE VARIABLES HERE ---
    // Example: Frequency array for "number of distinct elements"
    // vector<int> freq; 
    
    Mo(const vector<int>& input, int num_queries) : arr(input), q(num_queries) {
        n = arr.size();
        block_size = max(1.0, n / sqrt(max(1.0, (double)q)));
        answers.resize(q);
        // freq.assign(MAX_VAL, 0); 
    }

    // --- CHANGE LOGIC HERE ---
    void add(int idx) {
        int val = arr[idx];
        // Example: if (++freq[val] == 1) current_ans++;
    }

    void remove(int idx) {
        int val = arr[idx];
        // Example: if (--freq[val] == 0) current_ans--;
    }

    void solve(vector<Query>& queries) {
        for (int i = 0; i < q; i++) queries[i].block = queries[i].l / block_size;
        sort(queries.begin(), queries.end());

        int cur_l = 0, cur_r = -1;
        for (const auto& qry : queries) {
            while (cur_l > qry.l) add(--cur_l);
            while (cur_r < qry.r) add(++cur_r);
            while (cur_l < qry.l) remove(cur_l++);
            while (cur_r > qry.r) remove(--cur_r);
            answers[qry.id] = current_ans;
        }
    }
};