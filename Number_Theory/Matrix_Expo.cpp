// Ensure MOD is defined in your template
// const int MOD = 998244353; 

// Optimized Matrix Multiplication
vector<vector<int>> matmul(const vector<vector<int>>& a, const vector<vector<int>>& b) {
    int n = a.size();
    vector<vector<int>> res(n, vector<int>(n, 0));
    
    // Loop order i -> k -> j ensures cache-friendly sequential memory access
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            if (a[i][k] == 0) continue; // Skips inner loop if multiplier is zero
            for (int j = 0; j < n; j++) {
                res[i][j] = (res[i][j] + a[i][k] * b[k][j]) % MOD;
            }
        }
    }
    return res;
}

// Iterative Fast Matrix Exponentiation
vector<vector<int>> matexpo(vector<vector<int>> a, int k) {
    int n = a.size();
    vector<vector<int>> res(n, vector<int>(n, 0));
    
    // Initialize res as the Identity Matrix
    for (int i = 0; i < n; i++) {
        res[i][i] = 1;
    }

    // Binary exponentiation loop
    while (k > 0) {
        if (k % 2 == 1) {
            res = matmul(res, a);
        }
        a = matmul(a, a);
        k /= 2;
    }
    return res;
}