// O(DlogD) D-> Divisor

vector<ll> generate_divisors(const vector<ll>& prime_factors) {
    // Step 1: Compress {2, 2, 3} into {(2, 2), (3, 1)}
    vector<pair<ll, int>> pf_counts;
    for (ll p : prime_factors) {
        if (pf_counts.empty() || pf_counts.back().first != p) {
            pf_counts.push_back({p, 1});
        } else {
            pf_counts.back().second++;
        }
    }

    vector<ll> divisors;

    // Step 2: DFS to build combinations
    // 'idx' tracks which prime we are on, 'curr' is the divisor we are building
    auto dfs = [&](auto& self, int idx, ll curr) -> void {
        if (idx == pf_counts.size()) {
            divisors.push_back(curr);
            return;
        }

        ll p = pf_counts[idx].first;
        int max_power = pf_counts[idx].second;
        ll p_pow = 1;

        // Try multiplying by p^0, p^1, ..., p^max_power
        for (int i = 0; i <= max_power; i++) {
            self(self, idx + 1, curr * p_pow);
            if (i < max_power) {
                p_pow *= p; 
            }
        }
    };

    dfs(dfs, 0, 1);

    // Step 3: Sort the divisors
    sort(divisors.begin(), divisors.end());
    
    return divisors;
}