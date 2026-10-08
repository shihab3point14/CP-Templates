/**
 * LCS Struct (Longest Common Subsequence)
 * Time Complexity: O(N * M)
 * Space Complexity: O(N * M)
 */
struct LCS {
    string s1, s2;
    int n, m;
    vector<vector<int>> dp;

    LCS(string _s1, string _s2) : s1(_s1), s2(_s2) {
        n = s1.size();
        m = s2.size();
        // dp[i][j] stores LCS length of s1[0...i-1] and s2[0...j-1]
        dp.assign(n + 1, vector<int>(m + 1, 0));
        solve();
    }

    // Fills the DP table
    void solve() {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (s1[i - 1] == s2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
    }

    // Returns the length of the LCS
    int get_length() {
        return dp[n][m];
    }

    // Reconstructs and returns the actual LCS string
    string get_string() {
        string res = "";
        int i = n, j = m;
        while (i > 0 && j > 0) {
            if (s1[i - 1] == s2[j - 1]) {
                res += s1[i - 1];
                i--; j--;
            } else if (dp[i - 1][j] > dp[i][j - 1]) {
                i--;
            } else {
                j--;
            }
        }
        reverse(res.begin(), res.end());
        return res;
    }
};